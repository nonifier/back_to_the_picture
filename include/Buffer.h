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

Buffer& operator<<(Buffer& buffer, Slice slice);
Buffer& operator<<(Buffer& buffer, Slice_const slice);

class BufferWritter {
private:
	Buffer& buff;
	size_t writtenBytes;
public:
	BufferWritter(Buffer& buff): buff(buff), writtenBytes(0) {}
	size_t write(const uint8_t* src_ptr, size_t write_size) {
		
		size_t buff_size = buff.getSize();
		
		if (writtenBytes >= buff_size) {
			return 0;
		}

		if (writtenBytes + write_size >= buff_size) {
			return 0;
		}

		uint8_t* buff_ptr = buff.getData().get();
		uint8_t* offset_ptr = buff_ptr + writtenBytes;
		memcpy(offset_ptr, src_ptr, write_size);
		
		writtenBytes += write_size;

		return write_size;
	}
};

inline BufferWritter& operator<<(BufferWritter& buff_writter, Slice slice) {
	buff_writter.write(slice.getPtr(), slice.getSize());
	return buff_writter;
}

inline BufferWritter& operator<<(BufferWritter& buff_writter, Slice_const slice) {
	buff_writter.write(slice.getPtr(), slice.getSize());
	return buff_writter;
}