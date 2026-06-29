#include "core.hpp"
#include "runner.hpp"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <numeric>
#include <sstream>
#include <fcntl.h>
#include <stdexcept>
#include <sys/resource.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

static std::string fmt(double val, int precision) {
  std::ostringstream ss;
  ss << std::fixed << std::setprecision(precision) << val;
  return ss.str();
}

struct RunResult {
  double wall_ms, user_ms, sys_ms, cpu_pct, peak_mb;
  long   nvcsw, nivcsw;
  int    exit_code;
  bool   signaled;
  int    signum;
};

class RunCmd : public Runner {
public:
  RunCmd(std::string target, int repeat)
      : Runner(std::move(target)), repeat_(repeat) {}

  std::string name() const override { return "run"; }

  void execute() override {
    std::vector<RunResult> results;
    results.reserve(repeat_);

    for (int i = 0; i < repeat_; ++i) {
      struct timespec start, end;
      clock_gettime(CLOCK_MONOTONIC, &start);

      pid_t pid = fork();
      if (pid < 0) throw std::runtime_error("fork failed");

      if (pid == 0) {
        int devnull = open("/dev/null", O_WRONLY);
        dup2(devnull, STDOUT_FILENO);
        dup2(devnull, STDERR_FILENO);
        close(devnull);
        execl("/bin/sh", "sh", "-c", _target.c_str(), (char *)NULL);
        _exit(127);
      }

      int status;
      struct rusage usage;
      wait4(pid, &status, 0, &usage);
      clock_gettime(CLOCK_MONOTONIC, &end);

      RunResult r;
      r.wall_ms = (end.tv_sec  - start.tv_sec)  * 1000.0
                + (end.tv_nsec - start.tv_nsec) / 1e6;
      r.user_ms = usage.ru_utime.tv_sec * 1000.0 + usage.ru_utime.tv_usec / 1000.0;
      r.sys_ms  = usage.ru_stime.tv_sec * 1000.0 + usage.ru_stime.tv_usec / 1000.0;
      r.cpu_pct = r.wall_ms > 0 ? (r.user_ms + r.sys_ms) / r.wall_ms * 100.0 : 0.0;
#ifdef __APPLE__
      r.peak_mb = usage.ru_maxrss / 1024.0 / 1024.0;
#else
      r.peak_mb = usage.ru_maxrss / 1024.0;
#endif
      r.nvcsw    = usage.ru_nvcsw;
      r.nivcsw   = usage.ru_nivcsw;
      r.signaled = WIFSIGNALED(status);
      r.signum   = r.signaled ? WTERMSIG(status) : 0;
      r.exit_code = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
      results.push_back(r);
    }

    print(results);
  }

private:
  int repeat_;

  void print(const std::vector<RunResult>& results) {
    std::cout << std::fixed;

    const int L = 18;
    int failures = 0;
    for (auto& r : results) if (r.exit_code != 0 || r.signaled) ++failures;

    if (results.size() == 1) {
      const RunResult& r = results[0];
      const std::string sep(44, '-');

      std::cout << "\n"
                << "  target  " << _target << "\n"
                << "  " << sep << "\n\n";

      auto row = [&](const std::string& label, const std::string& value) {
        std::cout << "    " << std::left << std::setw(L) << label << value << "\n";
      };

      std::cout << "  TIME\n";
      row("wall",   fmt(r.wall_ms, 2) + " ms");
      row("user",   fmt(r.user_ms, 2) + " ms");
      row("system", fmt(r.sys_ms,  2) + " ms");
      std::cout << "\n";

      std::cout << "  CPU\n";
      row("utilization",  fmt(r.cpu_pct, 1) + "%");
      row("vol switches", std::to_string(r.nvcsw));
      row("inv switches", std::to_string(r.nivcsw));
      std::cout << "\n";

      std::cout << "  MEMORY\n";
      row("peak RSS", fmt(r.peak_mb, 2) + " MB");
      std::cout << "\n";

      std::cout << "  PROCESS\n";
      if (!r.signaled) {
        row("exit code", std::to_string(r.exit_code));
        row("signal",    "none");
      } else {
        row("exit code", "-");
        row("signal",    std::to_string(r.signum));
      }

      std::cout << "\n  " << sep << "\n\n";

    } else {
      const int C = 12;
      const std::string sep(L + 4 + C * 4, '-');
      int n = results.size();

      // returns {min, mean, max, std dev} for a given metric
      auto agg = [&](auto fn) -> std::tuple<double,double,double,double> {
        std::vector<double> v;
        for (auto& r : results) v.push_back(fn(r));
        double mn  = *std::min_element(v.begin(), v.end());
        double mx  = *std::max_element(v.begin(), v.end());
        double avg = std::accumulate(v.begin(), v.end(), 0.0) / n;
        double var = 0.0;
        for (double x : v) var += (x - avg) * (x - avg);
        return {mn, avg, mx, std::sqrt(var / n)};
      };

      // returns the p-th percentile for a given metric
      auto pct = [&](auto fn, double p) -> double {
        std::vector<double> v;
        for (auto& r : results) v.push_back(fn(r));
        std::sort(v.begin(), v.end());
        int idx = std::max(0, (int)std::ceil(p / 100.0 * n) - 1);
        return v[idx];
      };

      double total_ms = 0.0;
      for (auto& r : results) total_ms += r.wall_ms;

      std::cout << "\n"
                << "  target     " << _target << "\n"
                << "  runs       " << n << "\n"
                << "  total time " << fmt(total_ms, 2) << " ms\n"
                << "  " << sep << "\n\n";

      auto header = [&](const std::string& section) {
        std::cout << "  " << std::left << std::setw(L + 4) << section
                  << std::setw(C) << "min"
                  << std::setw(C) << "mean"
                  << std::setw(C) << "max"
                  << "std dev\n";
      };

      auto mrow = [&](const std::string& label,
                      const std::string& mn, const std::string& avg,
                      const std::string& mx, const std::string& sd) {
        std::cout << "    " << std::left << std::setw(L) << label
                  << std::setw(C) << mn
                  << std::setw(C) << avg
                  << std::setw(C) << mx
                  << "± " << sd << "\n";
      };

      auto srow = [&](const std::string& label, const std::string& value) {
        std::cout << "    " << std::left << std::setw(L) << label << value << "\n";
      };

      auto [wn,wa,wx,ws] = agg([](auto& r){ return r.wall_ms; });
      auto [un,ua,ux,us] = agg([](auto& r){ return r.user_ms; });
      auto [sn,sa,sx,ss] = agg([](auto& r){ return r.sys_ms;  });

      header("TIME");
      mrow("wall",   fmt(wn,2)+" ms", fmt(wa,2)+" ms", fmt(wx,2)+" ms", fmt(ws,2)+" ms");
      mrow("user",   fmt(un,2)+" ms", fmt(ua,2)+" ms", fmt(ux,2)+" ms", fmt(us,2)+" ms");
      mrow("system", fmt(sn,2)+" ms", fmt(sa,2)+" ms", fmt(sx,2)+" ms", fmt(ss,2)+" ms");
      std::cout << "\n";

      std::cout << "  TIME PERCENTILES (wall)\n";
      srow("p50", fmt(pct([](auto& r){ return r.wall_ms; }, 50), 2) + " ms");
      srow("p95", fmt(pct([](auto& r){ return r.wall_ms; }, 95), 2) + " ms");
      srow("p99", fmt(pct([](auto& r){ return r.wall_ms; }, 99), 2) + " ms");
      std::cout << "\n";

      auto [cn,ca,cx,csd] = agg([](auto& r){ return r.cpu_pct;       });
      auto [vn,va,vx,vsd] = agg([](auto& r){ return (double)r.nvcsw;  });
      auto [in_,ia,ix,isd] = agg([](auto& r){ return (double)r.nivcsw; });

      header("CPU");
      mrow("utilization",  fmt(cn,1)+"%",  fmt(ca,1)+"%",  fmt(cx,1)+"%",  fmt(csd,1)+"%");
      mrow("vol switches", fmt(vn,0),       fmt(va,1),       fmt(vx,0),       fmt(vsd,1));
      mrow("inv switches", fmt(in_,0),      fmt(ia,1),       fmt(ix,0),       fmt(isd,1));
      std::cout << "\n";

      auto [pn,pa,px,psd] = agg([](auto& r){ return r.peak_mb; });

      header("MEMORY");
      mrow("peak RSS", fmt(pn,2)+" MB", fmt(pa,2)+" MB", fmt(px,2)+" MB", fmt(psd,2)+" MB");
      std::cout << "\n";

      std::cout << "  PROCESS\n";
      srow("failures", std::to_string(failures) + " / " + std::to_string(n));

      std::cout << "\n  " << sep << "\n\n";
    }
  }
};

void run_cmd(std::string cmd, int repeat) {
  RunCmd runner(std::move(cmd), repeat);
  runner.execute();
}
