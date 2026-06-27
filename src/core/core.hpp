#pragma once
#include <iostream>
#include <string>
#include <vector>

void help();
void help(std::string cmd);

void run_cmd(std::string cmd);
void http_cmd(std::string url);
void compare_cmd(std::vector<std::string> programs);
void file_cmd(std::string path);
void db_cmd(std::string target);
void load_cmd(std::string target);