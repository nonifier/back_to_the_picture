#pragma once

#include "Buffer.h"
#include "Slice.h"
#include "jpeg\Marker.h"

#include <functional>
#include <memory>

namespace jpeg {
	class Parser
	{
	public:
		Parser(Buffer && jpeg_data);
		bool hasNextMarker() const;
		std::unique_ptr<Marker> getNextMarker() const;
		Parser& iterateMarkers(std::function<void(Marker&)> func);

	private:
		Buffer jpeg_data;
		size_t readBytes;

		void reset();
		void advanceJpegDaga(uint16_t parsedData);
		const Slice readNextDataSlice() const;
	};
}