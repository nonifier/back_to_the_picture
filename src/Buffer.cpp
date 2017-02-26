
#include "Buffer.h"

Buffer::Buffer(size_t size) :
	size(size),
	data(new uint8_t[size])
{}

Buffer::Buffer(const Buffer && buf) :
	size(std::move(buf.size)),
	data(std::move(buf.data))
{}

std::shared_ptr<uint8_t> Buffer::getData() const { 
	return data; 
}

size_t Buffer::getSize() const { 
	return size; 
}

Buffer::operator Slice() const {
	return Slice(size, data.get());
}

std::fstream& operator >> (std::fstream& in, Buffer& buffer) {
	char* ptr = reinterpret_cast<char*>(buffer.getData().get());
	in.read(ptr, buffer.getSize());
	return in;
}