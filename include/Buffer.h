#pragma once

#include "slice.h"

#include <memory>
#include <fstream>

class Buffer {
public:
	Buffer();
	Buffer(size_t size);
	Buffer(const Buffer && buf);

	std::shared_ptr<uint8_t> getData() const;
	size_t getSize() const;

	operator Slice() const;

private:
	size_t size;
	std::shared_ptr<uint8_t> data;
};

std::fstream& operator >> (std::fstream& in, Buffer& buffer);