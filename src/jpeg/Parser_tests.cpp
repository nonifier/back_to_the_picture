#include "jpeg\Parser.h"
#include "gtest\gtest.h"
#include "gmock\gmock.h"

#include <array>

using namespace jpeg;

namespace {
	const std::array<uint16_t, 4> smaller_jpeg = {
		0xFFD8, // Start of Image
		0xDEAD, // Some data ...
		0xBEEF, // Same data ...
		0xFFD9 // End Of File
	};
}

TEST(Parser, should_create_empty_parser) 
{
	Buffer buf;
	Parser parser(buf);
	EXPECT_FALSE(parser.hasNextMarker());
}

TEST(Parser, should_not_iterate_on_empty_buffer)
{
	Buffer buf;
	Parser parser(buf);

	testing::MockFunction<void(Parser::MarkerPtr)> mock;

	EXPECT_CALL(mock, Call(testing::_)).Times(0);
	parser.iterateMarkers(mock.AsStdFunction());
}

TEST(Parser, should_parse_start_of_image)
{

}