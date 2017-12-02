#pragma once

#include "Slice.h"

#include <sstream>
#include <string>

namespace jpeg {

	class Marker
	{
	public:
		
		const static uint8_t MARKER = 0xFF;
		const static uint8_t SOI = 0xD8; // Start Of Image
		const static uint8_t EOI = 0xD9; // End Of Image
		const static uint8_t APP0 = 0xE0; // APP 0
		const static uint8_t APP1 = 0xE1; // APP 1
		const static uint8_t SOS = 0xDA; // Start Of Scan

		virtual ~Marker();
		virtual std::string getName() const = 0;
		virtual std::string getInfo() const = 0;
		virtual uint16_t getSize() const = 0;
		virtual Slice_const getHeaderSlice() const = 0;
		virtual Slice_const getDataSlice() const = 0;

		template<class T>
		static T ReinterpretSliceToMarkerHeader(const Slice slice) {
			T* header_ptr = reinterpret_cast<T*>(&slice);
			return *header_ptr;
		}

		template<class T>
		static Slice_const getSliceFromHeader(T& header) { 
			const uint8_t* header_ptr = reinterpret_cast<const uint8_t*>(&header);
			size_t header_size = sizeof(header);

			return Slice_const(header_ptr, header_size);
		}

		template<class T>
		T& writeHeader(T& out) const {
			Slice_const header_slice = getHeaderSlice();
			out << header_slice;
			return out;
		}

		template<class T>
		T& writeData(T& out) const {
			Slice_const data_slice = getDataSlice();
			out << data_slice;
			return out;
		}
	};
}

std::ostream& operator<<(std::ostream& o, const jpeg::Marker& marker);

template<class T>
T& operator<<(T& out, const jpeg::Marker& marker) {
	marker.writeHeader(out);
	marker.writeData(out);
	return out;
}