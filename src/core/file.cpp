#include "core.hpp"

void file_cmd(std::string path) {
  std::cout << "[ultrabench] target   : " << path << "\n";
  std::cout << "[ultrabench] operation: read + write\n";
  std::cout << "[ultrabench] file size: 1.0 GB\n";
  std::cout << "[ultrabench] block size: 4 KB\n";
  std::cout << "\n";
  std::cout << "Running benchmark...\n";
  std::cout << "\n";
  std::cout << "Sequential Read:\n";
  std::cout << "  throughput       : 2.1 GB/s\n";
  std::cout << "  latency (mean)   : 0.18 ms\n";
  std::cout << "  IOPS             : 512,000\n";
  std::cout << "\n";
  std::cout << "Sequential Write:\n";
  std::cout << "  throughput       : 1.4 GB/s\n";
  std::cout << "  latency (mean)   : 0.27 ms\n";
  std::cout << "  IOPS             : 341,000\n";
  std::cout << "\n";
  std::cout << "Random Read:\n";
  std::cout << "  throughput       : 780 MB/s\n";
  std::cout << "  latency (mean)   : 0.52 ms\n";
  std::cout << "  IOPS             : 190,000\n";
  std::cout << "\n";
  std::cout << "[ultrabench] (example output — file benchmarking not yet implemented)\n";
}
