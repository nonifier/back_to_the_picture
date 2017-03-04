#pragma once

#include "Buffer.h"

class BufferWritter {
private:
	Buffer& buff;
	size_t writtenBytes;
public:
	BufferWritter(Buffer& buff) : buff(buff), writtenBytes(0) {}
	size_t write(const uint8_t* src_ptr, size_t write_size) {

		if (src_ptr == nullptr || write_size == 0) {
			return 0;
		}

		size_t buff_size = buff.getSize();

		if (writtenBytes >= buff_size) {
			return 0;
		}

		if (writtenBytes + write_size > buff_size) {
			return 0;
		}

		uint8_t* buff_ptr = buff.getData().get();
		uint8_t* offset_ptr = buff_ptr + writtenBytes;
		memcpy(offset_ptr, src_ptr, write_size);

		writtenBytes += write_size;

		return write_size;
	}
};

template<class T>
inline BufferWritter& operator<<(BufferWritter& buff_writter, T slice) {
	const uint8_t* slice_ptr = slice.getPtr();
	size_t slice_size = slice.getSize();
	buff_writter.write(slice_ptr, slice_size);
	return buff_writter;
}
