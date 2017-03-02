#pragma once

#include "jpeg\Marker.h"

namespace jpeg {

	class SOI : public Marker
	{
	public:
		SOI(Slice s);
		virtual ~SOI();
		virtual std::string getName() const override;
		virtual std::string getInfo() const override;
		virtual uint16_t getSize() const override;
		virtual Slice_const getHeaderSlice() const override;
		virtual Slice_const getDataSlice() const override;

	private:
		struct SOI_Header {
			uint8_t code[2];
		} header;

		uint16_t getCode() const;
	};

}