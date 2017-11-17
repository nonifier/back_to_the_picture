#include "jpeg\Parser.h"
#include "gtest\gtest.h"
#include "gmock\gmock.h"

using namespace jpeg;

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
