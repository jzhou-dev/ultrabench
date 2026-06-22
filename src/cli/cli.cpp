#include "../core/core.hpp"
#include <string>
int main (int argc, char** argv) {
  if (argc < 2 || !std::strcmp(argv[1], "--help") || !std::strcmp(argv[1], "-h")) {
    help();
  } else if (!std::strcmp(argv[1], "--run")) {
    if (argc < 3) {
      help(argv[1]);
    } else {
      run_cmd(argv[2]);  
    }
  } 
}
