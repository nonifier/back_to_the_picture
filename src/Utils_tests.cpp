
#include "Utils.h"
#include "gtest\gtest.h"

using namespace utils;

TEST(Utils, should_open_file_if_it_exist) {
	std::string filename("Hello.jpeg");
	EXPECT_NO_THROW(open_file(filename));
}
