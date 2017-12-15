
#include "StringUtil.h"

#include <sstream>
#include <iomanip>

namespace utils {

std::string remove_char_and_before(const std::string& str, const char c)
{
	size_t offset = str.find(c);

	if (offset == std::string::npos)
		return str;

	return str.substr(offset + 1);
}

std::string remove_char_and_after(const std::string& str, const char c)
{
	size_t offset = str.find(c);

	if (offset == std::string::npos)
		return str;

	return str.substr(0, offset);
}

std::string extract_tag(
	const std::string& packet,
	const std::string& tagName)
{
	size_t tagStartOffet = packet.find(tagName);
	if (tagStartOffet == std::string::npos) {
		std::stringstream ss("Opening tag not found: ");
		ss << tagName;
		throw std::runtime_error(ss.str());
	}

	std::string tagStart(packet.substr(tagStartOffet));
	std::string tagStr = tagStart.substr(tagName.size());

	size_t tagEndOffset = tagStr.find(tagName);
	if (tagEndOffset == std::string::npos) {
		std::stringstream ss("Closing tag not found: ");
		ss << tagName;
		throw std::runtime_error(ss.str());
	}

	tagStr = tagStr.substr(0, tagEndOffset);
	tagStr = remove_char_and_before(tagStr, '>');
	tagStr = remove_char_and_after(tagStr, '<');

	return tagStr;
}	

bool has_tag(std::string_view str, std::string_view tag)
{
	return str.find(tag) != std::string::npos;
}

std::string convert_tm_as_string(const tm time)
{
	std::stringstream ss;

	ss << std::put_time(&time, "%FT%T.000");
	
	return ss.str();
}

} // namespace utils