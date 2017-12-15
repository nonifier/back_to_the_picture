#pragma once

#include <string>
#include <string_view>

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

	bool has_tag(
		std::string_view str,
		std::string_view tag);

	std::string convert_tm_as_string(
		const tm time);
}