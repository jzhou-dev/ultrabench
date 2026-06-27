#include "../core/core.hpp"
#include <cstring>
#include <string>
#include <vector>

int main(int argc, char **argv) {
  if (argc < 2 || !std::strcmp(argv[1], "--help") ||
      !std::strcmp(argv[1], "-h")) {
    help();
  } else if (!std::strcmp(argv[1], "--run")) {
    if (argc < 3) {
      help("--run");
    } else {
      run_cmd(argv[2]);
    }
  } else if (!std::strcmp(argv[1], "--http")) {
    if (argc < 3) {
      help("--http");
    } else {
      http_cmd(argv[2]);
    }
  } else if (!std::strcmp(argv[1], "--compare")) {
    if (argc < 4) {
      help("--compare");
    } else {
      std::vector<std::string> programs;
      for (int i = 2; i < argc; ++i)
        programs.push_back(argv[i]);
      compare_cmd(programs);
    }
  } else if (!std::strcmp(argv[1], "--file")) {
    if (argc < 3) {
      help("--file");
    } else {
      file_cmd(argv[2]);
    }
  } else if (!std::strcmp(argv[1], "--db")) {
    if (argc < 3) {
      help("--db");
    } else {
      db_cmd(argv[2]);
    }
  } else if (!std::strcmp(argv[1], "--load")) {
    if (argc < 3) {
      help("--load");
    } else {
      load_cmd(argv[2]);
    }
  } else {
    std::cerr << "error: unknown command '" << argv[1] << "'\n";
    help();
    return 1;
  }
}
