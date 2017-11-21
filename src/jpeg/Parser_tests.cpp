#include "Utils.h"
#include "jpeg\Parser.h"
#include "jpeg\SOI.h"
#include "gtest\gtest.h"
#include "gmock\gmock.h"

#include <array>

using namespace jpeg;
using namespace testing;

namespace {
	typedef const std::array<uint8_t, 8> Image;
	Image smaller_jpeg = {
		0xFF, 0xD8, // Start of Image
		0xDE, 0xAD, // Some data ...
		0xBE, 0xEF, // Same data ...
		0xFF, 0xD9 // End Of File
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

	MockFunction<void(Parser::MarkerPtr)> mock;
	EXPECT_CALL(mock, Call(_)).Times(0);
	
	parser.iterateMarkers(mock.AsStdFunction());
}

TEST(Parser, should_parse_start_of_image)
{
	const auto arr_size = utils::get_array_size(smaller_jpeg);
	Buffer buff(arr_size);
	buff << smaller_jpeg;

	Parser parser(buff);
	MockFunction<void(Parser::MarkerPtr)> mock;
	
	EXPECT_CALL(mock, Call(
		Pointee(Property(&Marker::getName, StrCaseEq("SOI")))
	)).Times(1);

	parser.iterateMarkers(mock.AsStdFunction());
}

TEST(Parser, should_parse_image)
{
	const auto filename = "test_resrc/dsc05869_origin.jpg";
	Buffer file_buff = utils::read_file_to_buffer(filename);
	Parser parser(file_buff);

	MockFunction<void(Parser::MarkerPtr)> mock;
	EXPECT_CALL(mock, Call(_))
		.Times(10);

	parser.iterateMarkers(mock.AsStdFunction());
}

