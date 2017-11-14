#include "io/IO.h"

std::string io::to_upper(std::string & s) {
	std::string upper(s);
	std::transform(std::cbegin(s), std::cend(s),
		std::begin(upper), [](char c)
	{
		return std::toupper(c);
	});
	return upper;
}
