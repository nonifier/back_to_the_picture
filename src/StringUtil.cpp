
#include "StringUtil.h"

std::string removeBeforeChar(const std::string& str, const char c)
{
	size_t offset = str.find(c);

	if (offset == std::string::npos)
		return str;

	return str.substr(offset + 1);
}

std::string removePastChar(const std::string& str, const char c)
{
	size_t offset = str.find(c);

	if (offset == std::string::npos)
		return str;

	return str.substr(0, offset);
}

