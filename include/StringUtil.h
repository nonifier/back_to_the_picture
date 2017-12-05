#pragma once

#include <string>

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
}