#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include "thread_pool.hpp"

using std::filesystem::recursive_directory_iterator;
using std::filesystem::path;
using std::filesystem::is_directory;
using std::filesystem::exists;


size_t count_lines_in_dir(path path);
void count_lines(path path, size_t& counter);

int main(int argc, char** argv) {
	if (argc < 2) {
		std::cerr << "missing argument" << std::endl;
		exit(1);
	}

	auto target = argv[1];

	if (!exists(target)) {
		std::cerr << "directory "<< argv[1] <<"does not exist" << std::endl;
		exit(1);
	}

	

	auto lines = count_lines_in_dir(target);

	std::cout << "Total number of lines in " << target << "is " << lines << std::endl;

	return 0;
}

size_t count_lines_in_dir(path path) {
	if (!is_directory(path)) {
		std::cerr << path.string() << "is not a directory" << std::endl;
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

	while(std::getline(in, sink)) {
		counter++;
	}
}