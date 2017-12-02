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
		const void* ptr,
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

	Buffer_writter& write(
		const void* data_ptr,
		size_t write_size) 
	{
		auto remain = getSize() - written_bytes;
		if (write_size > remain)
			throw std::overflow_error("Buffer overflow");

		Buffer::write(data_ptr, write_size, written_bytes);
		written_bytes += write_size;
		return *this;
	}

	template<class T, size_t Size>
	Buffer_writter& operator<<(
		const std::array<T, Size>& arr)
	{
		auto write_size = arr.size() * sizeof(T);
		return write(arr.data(), write_size);
	}

	template<class T>
	Buffer_writter& operator<<(
		const std::basic_string<T>& str) 
	{
		auto val_size = sizeof(std::string::value_type);
		auto write_size = val_size * str.size();
		return write(str.c_str(), write_size);
	}
	
	Buffer_writter& operator<<(uint8_t i) {
		return write(&i, sizeof(i));
	}

	Buffer_writter& operator<<(uint16_t i) {
		uint8_t lower_bits = uint8_t(i & 0x00FF);
		uint8_t highter_bits = uint8_t(i >> 8);
		write(&highter_bits, sizeof(highter_bits));
		return write(&lower_bits, sizeof(lower_bits));
	}
private:
	size_t written_bytes;
};