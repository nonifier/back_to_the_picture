#pragma once

#include <memory>
#include <fstream>

class Slice
{
public:
	Slice() : size(0), ptr(nullptr) {}
	Slice(uint8_t* ptr, size_t size) :size(size), ptr(ptr) {}
	Slice(const Slice & s) :size(s.size), ptr(s.ptr) {}
	Slice(Slice && s) :
		size(s.size), 
		ptr(s.ptr) { s.nullify(); }

	Slice& operator=(const Slice&& s) {
		size = s.size;
		ptr = s.ptr;
		return *this;
	}

	size_t getSize() const { return size; }
	uint8_t* getPtr() const { return ptr; }
	uint8_t* operator->() const { return ptr; }
	uint8_t* operator&() const { return ptr; }
	uint8_t operator*() const { return *ptr; }

	Slice sub_slice(size_t ptr_offset) const {
		return Slice(ptr + ptr_offset, size-ptr_offset); 
	}

private:
	size_t size;
	uint8_t* ptr;

	void nullify() {
		size = 0;
		ptr = nullptr;
	}
};

class Slice_const
{
public:
	Slice_const() : size(0), ptr(nullptr) {}
	Slice_const(const uint8_t* ptr, size_t size) :size(size), ptr(ptr) {}
	Slice_const(const Slice_const & s) :size(s.size), ptr(s.ptr) {}
	Slice_const(Slice_const&& s) :
		size(s.size),
		ptr(s.ptr) {
		s.nullify();
	}

	Slice_const& operator=(const Slice_const&& s) {
		size = s.size;
		ptr = s.ptr;
		return *this;
	}

	size_t getSize() const { return size; }
	const uint8_t* getPtr() const { return ptr; }
	const uint8_t* operator->() const { return ptr; }
	const uint8_t* operator&() const { return ptr; }
	const uint8_t operator*() const { return *ptr; }

private:
	size_t size;
	const uint8_t* ptr;

	void nullify() {
		size = 0;
		ptr = nullptr;
	}
};

inline std::fstream & operator<<(std::fstream& o, Slice slice) {
	char * slice_ptr = reinterpret_cast<char*>(slice.getPtr());
	size_t slice_size = slice.getSize();

	o.write(slice_ptr, slice_size);
	return o;
}

inline std::fstream & operator<<(std::fstream& o, Slice_const slice) {
	const char * slice_ptr = reinterpret_cast<const char*>(slice.getPtr());
	size_t slice_size = slice.getSize();

	o.write(slice_ptr, slice_size);
	return o;
}