#pragma once
#include <filesystem>

using std::filesystem::path;

size_t count_lines_in_dir(path path);
void count_lines(path path, size_t& counter);