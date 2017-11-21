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
	auto arr_size = smaller_jpeg.size() * sizeof(Image::value_type);
	Buffer buff(arr_size);
	buff << smaller_jpeg;

	Parser parser(buff);
	MockFunction<void(Parser::MarkerPtr)> mock;
	
	EXPECT_CALL(mock,
		 Call(
			 Pointee(
				 Property(&Marker::getName, StrCaseEq("SOI"))
			 )
		 )
	).Times(1);

	parser.iterateMarkers(mock.AsStdFunction());
}