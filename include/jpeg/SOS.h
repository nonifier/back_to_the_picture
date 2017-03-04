#pragma once

#include "Buffer.h"
#include "jpeg\Marker.h"

namespace jpeg {
	class SOS : public Marker 
	{
	public:
		SOS(Slice data);
		virtual ~SOS() override;
		virtual std::string getName() const override;
		virtual std::string getInfo() const override;
		virtual uint16_t getSize() const override;
		virtual Slice_const getHeaderSlice() const override;
		virtual Slice_const getDataSlice() const override;

	private:
		struct SOS_Header {
			uint8_t code;
			uint8_t id;
			uint8_t size[2];
		} header;

		Buffer dataBuffer;
	};

}