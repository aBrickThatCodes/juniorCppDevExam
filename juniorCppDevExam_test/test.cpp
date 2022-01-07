#include "pch.h"
#include "../junioCppDevExam/main.cpp"
#include <filesystem>
#include <fstream>

using std::filesystem::current_path;
using std::filesystem::temp_directory_path;
using std::filesystem::create_directories;
using std::filesystem::remove_all;

class DirectoryLineCountingTest :public::testing::Test {
protected:
	void SetUp() override {
		current_path(temp_directory_path());

		auto threeLines = std::ofstream("threeLines.txt");
		threeLines << "1\n2\n3" << std::endl;

		std::ofstream("empty.txt");

		create_directories("singleDir");
		std::ofstream("singleDir/2in1.txt") << "1\n2" << std::endl;
		std::ofstream("singleDir/1in1.txt") << "1" << std::endl;

		create_directories("doubleDir/inner");
		std::ofstream("doubleDir/4in2.txt") << "4\n4\n4\n4" << std::endl;
		std::ofstream("doubleDir/inner/1in2.txt") << "1" << std::endl;
	}
	void TearDown() override {
		remove_all("threeLines.txt");
		remove_all("empty.txt");
		remove_all("singleDir");
		remove_all("doubleDir");
	}
};

TEST_F(DirectoryLineCountingTest, CountsLines) {
	size_t size = 0;
	count_lines("threeLines.txt", size);
	EXPECT_EQ(size, 3);
}

TEST_F(DirectoryLineCountingTest, HandlesEmptyFile) {
	size_t size = 1;
	count_lines("empty.txt", size);
	EXPECT_EQ(size, 0);
}

TEST_F(DirectoryLineCountingTest, HandlesOnlyDirPaths) {
	auto p = path("empty.txt");
	ASSERT_EXIT(count_lines_in_dir(p),testing::ExitedWithCode(1), p.string()+" is not a directory");
}

TEST_F(DirectoryLineCountingTest, DirWithTwoFiles) {
	ASSERT_EQ(count_lines_in_dir("singleDir"), 3);
}

TEST_F(DirectoryLineCountingTest, RecursiveDir) {
	EXPECT_EQ(count_lines_in_dir("doubleDir"), 5);
}