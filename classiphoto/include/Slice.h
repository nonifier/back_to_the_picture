#pragma once

#include <memory>

class Slice
{
public:
	Slice() : size(0), ptr(nullptr) {}
	Slice(size_t size, uint8_t* ptr) :size(size), ptr(ptr) {}
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

private:
	size_t size;
	uint8_t* ptr;

	void nullify() {
		size = 0;
		ptr = nullptr;
	}
};