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
	typedef std::shared_ptr<Marker> MarkerPtr;
	typedef std::function<void(MarkerPtr)> MarkerVisitorFunc;

	Parser(const Buffer & jpeg_data);
	bool hasNextMarker() const;
	Parser& iterateMarkers(MarkerVisitorFunc func);

private:
	const Buffer& jpeg_data;
	size_t readBytes;

	void reset();

	MarkerPtr getNextMarker() const;
	void advanceJpegDaga(uint16_t parsedData);
	const Slice readNextDataSlice() const;
};

} // namepsace jpeg