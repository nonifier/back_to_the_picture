#pragma once

#include <string>
#include <string_view>
#include <ctime>

namespace utils 
{
	std::string remove_char_and_before(
		const std::string& str, 
		const char c);
	
	std::string remove_char_and_after(
		const std::string& str, 
		const char c);
	
	std::string extract_tag(
		const std::string& packet,
		const std::string& tagName);

	std::size_t has_tag(
		std::string_view str,
		std::string_view tag);

	std::string convert_tm_as_string(
		const std::tm time);

	enum tag_type {
		tag_type_open,
		tag_type_selfclose,
	};

	std::string generate_tag(
		const std::string_view tag_name,
		const std::string_view tag_content,
		const std::string_view tag_namespace = "");
}