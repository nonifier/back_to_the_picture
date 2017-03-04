#pragma once

#include "jpeg\Marker.h"

namespace jpeg {
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
