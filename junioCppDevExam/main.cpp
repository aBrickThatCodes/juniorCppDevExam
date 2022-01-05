#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include "thread_pool.hpp"

using std::filesystem::recursive_directory_iterator;
using std::filesystem::current_path;
using std::filesystem::path;

size_t directory_counter(path path);
void line_count(path path, size_t& counter);

int main(int argc, char** argv) {
	auto lines = directory_counter(current_path());

	std::cout << "Total number of lines in " << current_path() << "is " << lines << std::endl;

	return 0;
}

size_t directory_counter(path path) {
	size_t lines = 0;
	thread_pool pool;

	auto iter = recursive_directory_iterator(path);

	for (auto& entry : iter) {
		if (entry.is_regular_file()) {
			pool.push_task([entry, &lines] { line_count(entry.path(), lines); });
		}
	}

	pool.wait_for_tasks();

	return lines;
}

void line_count(path path, size_t& counter) {
	std::ifstream in(path);
	std::string sink;

	while(std::getline(in, sink)) {
		counter++;
	}
}