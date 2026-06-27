#include "core.hpp"

void http_cmd(std::string url) {
  std::cout << "[ultrabench] target   : " << url << "\n";
  std::cout << "[ultrabench] method   : GET\n";
  std::cout << "[ultrabench] requests : 100\n";
  std::cout << "[ultrabench] concurrency: 10\n";
  std::cout << "\n";
  std::cout << "Running benchmark...\n";
  std::cout << "\n";
  std::cout << "  Total requests   : 100\n";
  std::cout << "  Succeeded        : 98\n";
  std::cout << "  Failed           : 2\n";
  std::cout << "  Throughput       : 312.4 req/s\n";
  std::cout << "\n";
  std::cout << "Latency:\n";
  std::cout << "  min              : 4.2 ms\n";
  std::cout << "  mean             : 31.8 ms\n";
  std::cout << "  p50              : 28.1 ms\n";
  std::cout << "  p95              : 74.6 ms\n";
  std::cout << "  p99              : 112.3 ms\n";
  std::cout << "  max              : 198.7 ms\n";
  std::cout << "\n";
  std::cout << "[ultrabench] (example output — HTTP benchmarking not yet implemented)\n";
}
