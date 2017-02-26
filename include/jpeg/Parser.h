#pragma once

#include "Buffer.h"
#include "Slice.h"
#include "jpeg\Marker.h"
#include <memory>

namespace jpeg {
	class Parser
	{
	public:
		Parser(Buffer && jpeg_data);
		bool hasNextMarker() const;
		std::unique_ptr<Marker> getNextMarker() const;
		void advanceJpegDaga(uint16_t parsedData);

	private:
		Buffer jpeg_data;
		size_t readBytes;

		const Slice readNextDataSlice() const;
	};
}