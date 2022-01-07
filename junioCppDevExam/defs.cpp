#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include "thread_pool.hpp"
#include "defs.h"

using std::filesystem::recursive_directory_iterator;
using std::filesystem::path;
using std::filesystem::is_directory;

size_t count_lines_in_dir(path path) {
	if (!is_directory(path)) {
		std::cerr << path.string() << " is not a directory" << std::endl;
		exit(1);
	}

	size_t lines = 0;
	thread_pool pool;

	auto iter = recursive_directory_iterator(path);

	for (auto& entry : iter) {
		if (entry.is_regular_file()) {
			pool.push_task([entry, &lines] { count_lines(entry.path(), lines); });
		}
	}

	pool.wait_for_tasks();

	return lines;
}

void count_lines(path path, size_t& counter) {
	std::ifstream in(path);
	std::string sink;

	if (in.peek() == std::ifstream::traits_type::eof()) {
		return;
	}

	while (std::getline(in, sink)) {
		counter++;
	}
}