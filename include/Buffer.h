#pragma once

#include "slice.h"

#include <memory>
#include <fstream>

class Buffer {
public:
	Buffer();
	Buffer(size_t size);
	Buffer(Buffer && buf);

	Buffer& operator=(const Buffer & buf);
	Buffer& operator=(Buffer && buf);

	std::shared_ptr<uint8_t> getData() const;

	size_t getSize() const;

	operator Slice() const;
	operator Slice_const() const;

	errno_t write(const uint8_t* ptr, size_t size);
	errno_t write(Slice s);
	errno_t write(Slice_const s);

private:
	size_t size;
	std::shared_ptr<uint8_t> data;
};

std::fstream& operator >> (std::fstream& in, Buffer& buffer);