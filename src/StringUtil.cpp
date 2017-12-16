
#include "StringUtil.h"

#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <functional>

namespace utils {

std::string remove_char_and_before(const std::string& str, const char c)
{
	auto offset = str.find(c);

	if (offset == std::string::npos)
		return str;

	return str.substr(offset + 1);
}

std::string remove_char_and_after(const std::string& str, const char c)
{
	auto offset = str.find(c);

	if (offset == std::string::npos) {
		return str;
	}
	
	return str.substr(0, offset);
}

std::string extract_tag(
	const std::string& packet,
	const std::string& tagName)
{
	auto tagStartOffet = packet.find(tagName);
	if (tagStartOffet == std::string::npos) {
		std::stringstream ss("Opening tag not found: ");
		ss << tagName;
		throw std::runtime_error(ss.str());
	}

	std::string tagStart(packet.substr(tagStartOffet));
	std::string tagStr = tagStart.substr(tagName.size());

	auto tagEndOffset = tagStr.find(tagName);
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

std::size_t has_tag(
	const std::string_view str, 
	const std::string_view tag)
{
	return str.find(tag);
}

std::string convert_tm_as_string(const std::tm time)
{
	std::stringstream ss;

	ss << std::put_time(&time, "%FT%T.000");
	
	return ss.str();
}

std::ostream& get_open_tag_brackets(
	std::ostream& os,
	const tag_type tag_t,
	const std::function<std::string()> tag_name_cb)
{
	os << "<";
	os << tag_name_cb();

	switch (tag_t)
	{
	case utils::tag_type_open:
		os << ">";
		break;
	case utils::tag_type_selfclose:
		os << "/>";
		break;
	default:
		throw std::invalid_argument("Unrecognised tag type argument");
	}

	return os;
}

std::ostream& get_close_tag_bracket(
	std::ostream& os,
	const std::function<std::string()> tag_name_cb)
{
	os << "</";
	os << tag_name_cb();
	os << ">";

	return os;
}

std::string get_tag_name(
	const std::string_view _name,
	const std::string_view _namepsace)
{
	std::stringstream tag_name;

	if (_namepsace.size())
		tag_name << _namepsace << ":";

	tag_name << _name;

	return tag_name.str();
}

std::ostream& stream_open_tag(
	std::ostream& os,
	const tag_type tag_t,
	const std::string_view _name,
	const std::string_view _namespace)
{
	get_open_tag_brackets(
		os,
		tag_t,
		[&]() { return get_tag_name(_name, _namespace); }
	);

	return os;
}

std::ostream& stream_close_tag(
	std::ostream& os,
	const std::string_view _name,
	const std::string_view _namespace)
{
	get_close_tag_bracket(
		os,
		[&]() { return get_tag_name(_name, _namespace); }
	);

	return os;
}

std::string generate_tag(
	const std::string_view _name,
	const std::string_view _content,
	const std::string_view _namespace)
{
	std::stringstream ss;

	stream_open_tag(ss, tag_type_open, _name, _namespace);
	ss << _content;
	stream_close_tag(ss, _name, _namespace);

	return ss.str();
}

} // namespace utils