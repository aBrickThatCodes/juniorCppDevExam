#include <iostream>
#include <filesystem>
#include "thread_pool.hpp"
#include "defs.h"

using std::filesystem::path;
using std::filesystem::exists;

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