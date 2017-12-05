
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

	const auto exif_create_date_xml =
		"<?xpacket begin='﻿' id='W5M0MpCehiHzreSzNTczkc9d'?>"
		" <x:xmpmeta xmlns:x=\"adobe:ns:meta/\">"
		"  <rdf:RDF xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\">"
		"   <rdf:Description rdf:about=\"uuid:faf5bdd5-ba3d-11da-ad31-d33d75182f1b\" xmlns:xmp=\"http://ns.adobe.com/xap/1.0/\">"
		"    <xmp:CreateDate>2017-02-12T13:45:40.089</xmp:CreateDate>"
		"   </rdf:Description>"
		"  </rdf:RDF>"
		" </x:xmpmeta>"
		"<?xpacket end=\"w\"?>";

}

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
		extract_tag(exif_create_date_xml, "CreateDate").c_str()
	);
}

TEST(StringUtil, should_extract_tag_with_namespace_from_xml)
{
	EXPECT_STRCASEEQ(
		"2017-02-12T13:45:40.089",
		extract_tag(exif_create_date_xml, "xmp:CreateDate").c_str()
	);
}