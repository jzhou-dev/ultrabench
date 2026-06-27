#include "core.hpp"
#include <iomanip>
#include <sstream>

void compare_cmd(std::vector<std::string> programs) {
  std::cout << "[ultrabench] comparing " << programs.size() << " programs\n\n";

  // example per-program stats
  struct Stats { double mean; double p50; double p95; double p99; double max; };
  std::vector<Stats> example = {
    {12.4, 11.8, 24.1, 38.7,  55.2},
    {31.8, 28.1, 74.6, 112.3, 198.7},
    {8.9,  8.2,  16.3, 22.1,  34.0},
  };

  const int col = 14;
  std::cout << std::left << std::setw(10) << "metric";
  for (size_t i = 0; i < programs.size(); ++i)
    std::cout << std::setw(col) << programs[i];
  std::cout << "\n" << std::string(10 + col * programs.size(), '-') << "\n";

  auto row = [&](const std::string& label, auto fn) {
    std::cout << std::left << std::setw(10) << label;
    for (size_t i = 0; i < programs.size(); ++i) {
      Stats& s = example[i % example.size()];
      std::ostringstream val;
      val << std::fixed << std::setprecision(1) << fn(s) << " ms";
      std::cout << std::setw(col) << val.str();
    }
    std::cout << "\n";
  };

  row("mean",  [](Stats& s){ return s.mean; });
  row("p50",   [](Stats& s){ return s.p50; });
  row("p95",   [](Stats& s){ return s.p95; });
  row("p99",   [](Stats& s){ return s.p99; });
  row("max",   [](Stats& s){ return s.max; });

  std::cout << "\n";
  std::cout << "[ultrabench] fastest  : " << programs[0] << "\n";
  std::cout << "[ultrabench] (example output — comparison benchmarking not yet implemented)\n";
}
