#include "core.hpp"

void load_cmd(std::string target) {
  std::cout << "[ultrabench] target   : " << target << "\n";
  std::cout << "[ultrabench] duration : 60s\n";
  std::cout << "[ultrabench] ramp-up  : 10s\n";
  std::cout << "[ultrabench] workers  : 50\n";
  std::cout << "\n";
  std::cout << "Running load test...\n";
  std::cout << "\n";
  std::cout << "  t=0s    workers: 0    req/s: 0      p99:   0.0 ms\n";
  std::cout << "  t=10s   workers: 50   req/s: 1,840  p99:  22.4 ms\n";
  std::cout << "  t=20s   workers: 50   req/s: 1,902  p99:  25.1 ms\n";
  std::cout << "  t=30s   workers: 50   req/s: 1,875  p99:  31.8 ms\n";
  std::cout << "  t=40s   workers: 50   req/s: 1,490  p99:  88.3 ms  [degraded]\n";
  std::cout << "  t=50s   workers: 50   req/s: 1,201  p99: 143.7 ms  [degraded]\n";
  std::cout << "  t=60s   workers: 50   req/s: 1,198  p99: 149.2 ms  [degraded]\n";
  std::cout << "\n";
  std::cout << "Summary:\n";
  std::cout << "  peak throughput  : 1,902 req/s (at t=20s)\n";
  std::cout << "  degradation onset: t=35s\n";
  std::cout << "  error rate       : 1.4%\n";
  std::cout << "  total requests   : 98,312\n";
  std::cout << "\n";
  std::cout << "[ultrabench] (example output — load benchmarking not yet implemented)\n";
}
