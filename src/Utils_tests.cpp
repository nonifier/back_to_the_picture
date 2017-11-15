
#include "Utils.h"
#include "gtest\gtest.h"

using namespace utils;
using namespace std;

namespace {
	const string empty_filename = "test_resrc/empty.txt";
	const string ten_bytes_filename = "test_resrc/10bytes.txt";
	const string stupid_filename = "stupid_file.txt";
}

TEST(Utils, should_open_file_if_it_exist) 
{
	auto file = open_file(empty_filename);
	EXPECT_TRUE(file.is_open());
}

TEST(Utils, should_throw_if_file_doesnt_exist) 
{
	std::string filename(stupid_filename);
	EXPECT_THROW(open_file(filename), 
		std::invalid_argument);
}

TEST(Utils, empty_file_should_have_0_size)
{
	auto file = open_file(empty_filename);
	EXPECT_EQ(0u, size(file));
}

TEST(Utils, file_should_have_size_above_0)
{
	auto file = open_file(ten_bytes_filename);
	EXPECT_EQ(10u, size(file));
}
