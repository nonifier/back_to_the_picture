#pragma once

#include "Slice.h"
#include "Buffer.h"

#include <sstream>
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

		virtual ~Marker() {}
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

		std::fstream& writeTo(std::fstream & out) const {
			writeHeader(out);
			writeData(out);
			return out;
		}

		std::fstream& writeHeader(std::fstream & out) const {
			Slice_const header_slice = getHeaderSlice();
			out << header_slice;
			return out;
		}

		std::fstream& writeData(std::fstream & out) const {
			Slice_const data_slice = getDataSlice();
			out << data_slice;
			return out;
		}
	};

	class GenericMarker : public Marker
	{
	public:
		GenericMarker(const Slice s);
		virtual ~GenericMarker();
		virtual std::string getName() const override;
		virtual std::string getInfo() const override;
		virtual uint16_t getSize() const override;
		virtual Slice_const getHeaderSlice() const override;
		virtual Slice_const getDataSlice() const override;
	
		uint8_t getCode()	const;
		uint8_t	getMarker() const;
		uint32_t getIdentifier() const;

	private:
		struct Marker_header {
			uint8_t marker;
			uint8_t code;
			uint8_t size[2];
			uint8_t identifier[4];
		} header;

		Buffer data;

		uint8_t* getNextMarkerPointer();
	};
}

std::ostream& operator<<(std::ostream& o, const jpeg::Marker& marker);
std::fstream& operator<<(std::fstream& out, const jpeg::Marker& marker);