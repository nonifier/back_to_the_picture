
#include "Buffer.h"

Buffer::Buffer() :
	size(0),
	data(nullptr)
{}

Buffer::Buffer(size_t size) :
	size(size),
	data(new uint8_t[size])
{}

Buffer::Buffer(Buffer && buf) :
	size(std::move(buf.size)),
	data(std::move(buf.data))
{}

Buffer& Buffer::operator=(const Buffer & buf) {
	size = buf.size;
	data = buf.data;
	return *this;
}

Buffer& Buffer::operator=(Buffer && buf) {
	size = std::move(buf.size);
	data = std::move(buf.data);
	return *this;
}

std::shared_ptr<uint8_t> Buffer::getData() const {
	return data;
}

size_t Buffer::getSize() const {
	return size;
}

Buffer::operator Slice() const {
	return Slice(data.get(), size);
}

Buffer::operator Slice_const() const {
	return Slice_const(data.get(), size);
}

errno_t Buffer::write(const uint8_t* src, size_t size) {
	errno_t res = 0;

	if (size) {
		void * dest_ptr = data.get();
		res = memcpy_s(dest_ptr, size, src, size);
	}

	return res;
}

errno_t Buffer::write(Slice s) {
	return write(s.getPtr(), s.getSize());
}

errno_t Buffer::write(Slice_const s) {
	return write(s.getPtr(), s.getSize());
}

std::fstream& operator>> (std::fstream& in, Buffer& buffer) {
	char* ptr = reinterpret_cast<char*>(buffer.getData().get());
	in.read(ptr, buffer.getSize());
	return in;
}

std::fstream& operator << (std::fstream& in, Buffer& buffer) {
	const char* ptr = reinterpret_cast<const char*>(buffer.getData().get());
	in.write(ptr, buffer.getSize());
	return in;
}
Buffer& operator<<(Buffer& buffer, Slice slice) {
	buffer.write(slice);
	return buffer;
}

Buffer& operator<<(Buffer& buffer, Slice_const slice) {
	buffer.write(slice);
	return buffer;
}