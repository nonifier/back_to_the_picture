#pragma once

#include "Slice.h"
#include <string>

namespace jpeg {

	class Marker
	{
	public:
		enum Type {
			MARKER = 0xFF,
			SOI = 0xD8, // Start Of Image
			EOI = 0xD9, // End Of Image
			APP0 = 0xE0, // APP 0
			APP1 = 0xE1, // APP 1
		};

		Marker() {}
		Marker(const Slice s);
		virtual ~Marker();
		virtual std::string getName() const;
		virtual std::string getInfo() const;
		virtual uint16_t getSize() const;

		uint8_t getCode()	const;
		uint8_t	getMarker() const;
		uint32_t getIdentifier() const;

		template<class T>
		static T ReinterpretSliceToMarkerHeader(const Slice slice) {
			T* header_ptr = reinterpret_cast<T*>(&slice);
			return *header_ptr;
		}

	private:
		struct Marker_header {
			uint8_t marker;
			uint8_t code;
			uint8_t size[2];
			uint8_t identifier[4];
		} header;

		uint8_t* getNextMarkerPointer();
	};
}