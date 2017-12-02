
#include "jpeg\Parser.h"
#include "jpeg\Exif.h"
#include "jpeg\Jfif.h"
#include "jpeg\Xmp.h"
#include "jpeg\SOI.h"
#include "jpeg\Null_Marker.h"
#include "jpeg\GenericMarker.h"
#include "jpeg\SOS.h"
#include "IO.h"

#include <utility>
#include <stdexcept>

using namespace jpeg;

Parser::Parser(const Buffer & jpeg_data) :
	jpeg_data(jpeg_data),
	readBytes(0)
{}

const Slice Parser::readNextDataSlice() const {
	return Slice(
		jpeg_data.getData().get() + readBytes,
		jpeg_data.getSize() - readBytes );
}

void Parser::reset() { 
	readBytes = 0; 
}

void Parser::advanceJpegDaga(uint16_t parsedByte) {
	readBytes += parsedByte;
}

Parser& Parser::iterateMarkers(MarkerVisitorFunc func) 
{
	while (hasNextMarker())
	{
		MarkerPtr marker = getNextMarker();
		advanceJpegDaga(marker->getSize()); 
		func(marker);
	}

	reset();

	return *this;
}

bool Parser::hasNextMarker() const {

	if (readBytes >= jpeg_data.getSize()) 
		return false;

	Slice nextSlice = readNextDataSlice();
	if (*nextSlice == Marker::MARKER)
		return true;
	
	return false;
}

Parser::MarkerPtr Parser::getNextMarker() const
{
	const Slice nextDataSlice = readNextDataSlice();

	const uint8_t* marker_ptr = nextDataSlice.getPtr();
	const uint8_t marker_id = *marker_ptr;
	if (marker_id != Marker::MARKER) {
		throw std::runtime_error("Marker doesn't start with 0xFF");
	}

	MarkerPtr marker = nullptr;
	uint8_t marker_code = *(marker_ptr+1);
	switch (marker_code)
	{
	case Marker::SOI:
		return std::make_shared<SOI>(nextDataSlice);

	case Marker::APP0:
	{
		GenericMarkerPtr generic_marker = 
			std::make_shared<GenericMarker>(nextDataSlice);
		uint32_t id = generic_marker->getIdentifier();

		switch (id) 
		{
		case Jfif::Id_code:
			return std::make_shared<Jfif>(nextDataSlice);
		default:
			return generic_marker;
		}
	}
	case jpeg::Marker::SOS:
		return std::make_shared<SOS>(nextDataSlice);

	case jpeg::Marker::APP1: {
		GenericMarkerPtr generic_ptr = std::make_shared<GenericMarker>(nextDataSlice);
		uint32_t id = generic_ptr->getIdentifier();
		switch (id) 
		{
		//case Exif::Id_code:
			//marker = std::make_unique<Exif>(nextDataSlice);
			//break;
		case Xmp::Id_code:
			return std::make_shared<Xmp>(nextDataSlice);
		default:
			return generic_ptr;
		}
	}
	default:
		return std::make_shared<GenericMarker>(nextDataSlice);
	}
}
