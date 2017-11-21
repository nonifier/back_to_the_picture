#pragma once

#include "slice.h"

#include <memory>
#include <fstream>

class Buffer 
{
public:
	Buffer();
	Buffer(size_t size);
	Buffer(Buffer && buf);
	virtual ~Buffer() {};

	Buffer& operator=(const Buffer & buf);
	Buffer& operator=(Buffer && buf);

	std::shared_ptr<uint8_t> getData() const;

	size_t getSize() const;

	operator Slice() const;
	operator Slice_const() const;

	errno_t write(
		const uint8_t* ptr,
		size_t size,
		size_t offset = 0U);
	errno_t write(Slice s);
	errno_t write(Slice_const s);

	Buffer& operator<<(Slice slice);
	Buffer& operator<<(Slice_const slice);

	template<class T, size_t Size>
	Buffer& operator<<(const std::array<T, Size>& arr) 
	{
		auto write_size = arr.size() * sizeof(T);
		const auto data_ptr = reinterpret_cast<const uint8_t*>(arr.data());
		write(data_ptr, write_size);
		return *this;
	}

private:
	size_t size;
	std::shared_ptr<uint8_t> data;

	void nullify();
};

std::istream& operator>>(
	std::istream& in, Buffer& buffer);
std::ostream& operator<<(
	std::ostream& in, Buffer& buffer);

class Buffer_writter : public Buffer
{
public:
	Buffer_writter(const size_t size);

	template<class T, size_t Size>
	Buffer_writter& operator<<(
		const std::array<T, Size>& arr)
	{
		auto write_size = arr.size() * sizeof(T);
		write(arr.data(), write_size, written_bytes);
		written_bytes += write_size;
		return *this;
	}

private:
	size_t written_bytes;
};