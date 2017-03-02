
#include "jpeg\Parser.h"
#include "jpeg\Exif.h"
#include "jpeg\Jfif.h"
#include "jpeg\Xmp.h"
#include "jpeg\SOI.h"
#include "jpeg\Null_Marker.h"
#include "IO.h"

#include <utility>
#include <stdexcept>

using namespace jpeg;

Parser::Parser(Buffer && jpeg_data) :
	jpeg_data( std::move(jpeg_data) ),
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

Parser& Parser::iterateMarkers(std::function<void(Marker&)> func) {
	do {
		std::unique_ptr<jpeg::Marker> marker = getNextMarker();
		func(*marker);
		advanceJpegDaga(marker->getSize());
	} while (hasNextMarker());

	reset();

	return *this;
}

bool Parser::hasNextMarker() const {

	if (readBytes >= jpeg_data.getSize()) 
		return false;

	Slice nextSlice = readNextDataSlice();
	if (*nextSlice == 0xFF)
		return true;
	
	return false;
}

std::unique_ptr<Marker> Parser::getNextMarker() const 
{
	const Slice nextDataSlice = readNextDataSlice();

	uint8_t* marker_ptr = nextDataSlice.getPtr();
	uint8_t marker_id = *marker_ptr;
	if (marker_id != jpeg::Marker::MARKER) {
		throw std::runtime_error("Marker doesn't start with 0xFF");
	}

	std::unique_ptr<Marker> marker = std::make_unique<Null_Marker>();
	uint8_t marker_code = *(marker_ptr+1);
	switch (marker_code)
	{
	case jpeg::Marker::Type::SOI:
		marker = std::make_unique<SOI>(nextDataSlice);
		break;
	case jpeg::Marker::Type::APP0:
		marker = std::make_unique<Jfif>(nextDataSlice);
		break;
	case jpeg::Marker::Type::APP1: {
		std::unique_ptr<GenericMarker> generic_ptr = std::make_unique<GenericMarker>(nextDataSlice);
		uint32_t id = generic_ptr->getIdentifier();
		switch (id) 
		{
		case Exif::Id_code:
			//marker = std::make_unique<Exif>(nextDataSlice);
			break;
		case Xmp::Id_code:
			marker = std::make_unique<Xmp>(nextDataSlice);
			break;
		}

		marker = std::move(generic_ptr);
		break;
	}
	default:
		marker = std::make_unique<GenericMarker>(nextDataSlice);
		break;
	}

	return marker;
}
