
#include "StringUtil.h"
#include "gtest\gtest.h"
#include "gmock\gmock.h"

using namespace utils;

namespace {

	const auto exif_mini_xml =
		"<?xpacket begin=\"﻿\" id=\"W5M0MpCehiHzreSzNTczkc9d\"?>"
		" <x:xmpmeta xmlns:x=\"adobe:ns:meta/\" x:xmptk=\"XMP Core 5.5.0\">"
		"  <rdf:RDF xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\">"
		"   <rdf:Description rdf:about=\"\"/>"
		"  </rdf:RDF>"
		" </x:xmpmeta> "
		"<?xpacket end=\"w\"?>";

	const auto exif_with_create_date_xml =
		"<?xpacket begin='﻿' id='W5M0MpCehiHzreSzNTczkc9d'?>"
		" <x:xmpmeta xmlns:x=\"adobe:ns:meta/\">"
		"  <rdf:RDF xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\">"
		"   <rdf:Description rdf:about=\"uuid:faf5bdd5-ba3d-11da-ad31-d33d75182f1b\" xmlns:xmp=\"http://ns.adobe.com/xap/1.0/\">"
		"    <xmp:CreateDate>2017-02-12T13:45:40.089</xmp:CreateDate>"
		"   </rdf:Description>"
		"  </rdf:RDF>"
		" </x:xmpmeta>"
		"<?xpacket end=\"w\"?>";

	const std::tm _2017_02_12T13_45_40_TM {
		40,   // seconds after the minute - [0, 60] including leap second
		45,   // minutes after the hour - [0, 59]
		13,  // hours since midnight - [0, 23]
		12,  // day of the month - [1, 31]
		1,   // months since January - [0, 11]
		117,  // years since 1900
		0,  // days since Sunday - [0, 6]
		43,  // days since January 1 - [0, 365]
		0, // daylight savings time flag
	};
} // namepsace

TEST(StringUtil, should_remove_character_and_before)
{
	EXPECT_THAT(
		remove_char_and_before("abcdefg", 'd'),
		testing::StrCaseEq("efg")
	);
}

TEST(StringUtil, should_remove_character_and_after)
{
	EXPECT_THAT(
		remove_char_and_after("abcdefg", 'd'),
		testing::StrCaseEq("abc")
	);
}

TEST(StringUtil, should_extract_tag_from_xml)
{
	EXPECT_STRCASEEQ(
		"2017-02-12T13:45:40.089", 
		extract_tag(exif_with_create_date_xml, "CreateDate").c_str()
	);
}

TEST(StringUtil, should_extract_tag_with_namespace_from_xml)
{
	EXPECT_STRCASEEQ(
		"2017-02-12T13:45:40.089",
		extract_tag(exif_with_create_date_xml, "xmp:CreateDate").c_str()
	);
}

TEST(StringUtil, should_find_tag)
{
	EXPECT_TRUE(
		has_tag(exif_with_create_date_xml, "xmp:CreateDate")
	);
}

TEST(StringUtil, should_not_find_tag)
{
	EXPECT_FALSE(
		has_tag(exif_mini_xml, "xmp:CreateDate")
	);
}

TEST(StringUtil, should_generate_date_with_correct_format)
{
	EXPECT_EQ(
		"2017-02-12T13:45:40.000",
		convert_tm_as_string(_2017_02_12T13_45_40_TM)
	);
}

TEST(StringUtil, should_generate_tag_with_name_content_and_namespace)
{
	EXPECT_EQ(
		"<xmp:CreateDate>MY_DATE</xmp:CreateDate>",
		generate_tag("CreateDate", "MY_DATE", "xmp")
	);
}

TEST(StringUtil, should_generate_tag_with_name_content_and_no_namespace)
{
	EXPECT_EQ(
		"<CreateDate>MY_DATE</CreateDate>",
		generate_tag("CreateDate", "MY_DATE")
	);
}

