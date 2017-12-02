#include "Utils.h"
#include "jpeg\Parser.h"
#include "jpeg\SOI.h"
#include "jpeg\Xmp.h"
#include "gtest\gtest.h"
#include "gmock\gmock.h"

#include <array>

using namespace jpeg;
using namespace testing;

namespace {
	typedef const std::array<uint8_t, 8> Image;

	const Image smaller_jpeg = {
		0xFF, 0xD8, // Start of Image
		0xDE, 0xAD, // Some data ...
		0xBE, 0xEF, // Same data ...
		0xFF, 0xD9 // End Of File
	};

	const auto filename = "test_resrc/dsc05869_correcte_date.jpg";
	const auto filename_w_date = "test_resrc/dsc05869_origin_with_date.jpg";
	const auto filename_full_data = "test_resrc/1_px_full_data.jpeg";
	const auto xmp_str = std::string(
		"http://ns.adobe.com/xap/1.0/."
		"<?xpacket begin=\"﻿\" id=\"W5M0MpCehiHzreSzNTczkc9d\"?>"
		"<x:xmpmeta xmlns:x=\"adobe:ns:meta/\" x:xmptk=\"XMP Core 5.5.0\">"
		"<rdf:RDF xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\">"
		"<rdf:Description rdf:about=\"\"/>"
		"</rdf:RDF>"
		"</x:xmpmeta>"
		"<?xpacket end=\"w\"?>"
	);

	Buffer build_APP1_buffer_from_string(const std::string& str)
	{
		const auto tag_size = uint16_t(str.size() + 2);
		Buffer_writter xmp_buff(tag_size + 2);

		xmp_buff
			<< Marker::MARKER
			<< Marker::APP1
			<< tag_size
			<< xmp_str;

		return std::move(xmp_buff);
	}
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
	auto file_buff = utils::read_file_to_buffer(filename);
	Parser parser(file_buff);

	MockFunction<void(Parser::MarkerPtr)> mock;
	EXPECT_CALL(mock, Call(_))
		.Times(AtLeast(10));

	parser.iterateMarkers(mock.AsStdFunction());
}

TEST(Parser, should_parse_xmp_tag)
{
	const auto buff = build_APP1_buffer_from_string(xmp_str);

	MockFunction<void(Parser::MarkerPtr)> mock;
	EXPECT_CALL(mock, Call(
		AllOf(
			Pointee(Property(&Marker::getName, StrCaseEq("Xmp"))),
			Pointee(Property(&Marker::getSize, Eq(buff.getSize())))
	))).Times(AtLeast(1));

	Parser parser(buff);
	const auto& mock_call = mock.AsStdFunction();
	parser.iterateMarkers(mock_call);
}