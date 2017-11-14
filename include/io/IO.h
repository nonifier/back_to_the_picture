#pragma once

#include <cstdint>
#include <string>
#include <algorithm>
#include <cctype>

namespace io {

	template<uint8_t size>
	uint16_t readUint16(const uint8_t array[size]) {
		uint16_t x = array[0] << 8 | array[1];
		return x;
	}

	template<uint8_t Size>
	size_t readSize_t(const uint8_t array[Size]) {
		size_t size(Size);
		size_t x = 0;
		for (size_t i = 0; i < size; i++) {
			size_t tmp = array[size - 1 - i];
			tmp = tmp << (8 * i);
			x |= tmp ;
		}
		return x;
	}

	std::string to_upper(std::string& s);
}