#include "core.hpp"

void help() {
  std::cout << "ultrabench - program benchmarking tool\n"
            << "\n"
            << "USAGE:\n"
            << "  ultrabench <command> [args]\n"
            << "\n"
            << "COMMANDS:\n"
            << "  --run <program>    Benchmark a program or shell command\n"
  << "  --http <url>       Benchmark an HTTP endpoint\n"
  << "  --compare <p1> <p2> [...]  Compare two or more programs side by side\n"
  << "  --file <path>      Benchmark file I/O throughput and latency\n"
  << "  --db <target>      Benchmark database query performance\n"
  << "  --load <target>    Run a sustained load test against a target\n"
            << "\n"
            << "OPTIONS:\n"
            << "  -h, --help       Show this help message\n";
}

void help(std::string cmd) {
  if (cmd == "--run") {
    std::cout << "ultrabench --run - benchmark a program or shell command\n"
              << "\n"
              << "USAGE:\n"
              << "  ultrabench --run <program>\n"
              << "\n"
              << "ARGS:\n"
              << "  <program>    The program or command to benchmark\n";
  } else if (cmd == "--http") {
    std::cout << "ultrabench --http - benchmark an HTTP endpoint\n"
              << "\n"
              << "USAGE:\n"
              << "  ultrabench --http <url>\n"
              << "\n"
              << "ARGS:\n"
              << "  <url>        The HTTP URL to benchmark\n";
  } else if (cmd == "--compare") {
    std::cout << "ultrabench --compare - compare two or more programs side by side\n"
              << "\n"
              << "USAGE:\n"
              << "  ultrabench --compare <program1> <program2> [...]\n"
              << "\n"
              << "ARGS:\n"
              << "  <program>    Two or more programs or commands to compare\n";
  } else if (cmd == "--file") {
    std::cout << "ultrabench --file - benchmark file I/O throughput and latency\n"
              << "\n"
              << "USAGE:\n"
              << "  ultrabench --file <path>\n"
              << "\n"
              << "ARGS:\n"
              << "  <path>       Path to the file or directory to benchmark\n";
  } else if (cmd == "--db") {
    std::cout << "ultrabench --db - benchmark database query performance\n"
              << "\n"
              << "USAGE:\n"
              << "  ultrabench --db <target>\n"
              << "\n"
              << "ARGS:\n"
              << "  <target>     Database connection string or DSN\n";
  } else if (cmd == "--load") {
    std::cout << "ultrabench --load - run a sustained load test against a target\n"
              << "\n"
              << "USAGE:\n"
              << "  ultrabench --load <target>\n"
              << "\n"
              << "ARGS:\n"
              << "  <target>     Program, URL, or endpoint to load test\n";
  } else {
    help();
  }
}
