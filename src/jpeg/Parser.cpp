
#include "jpeg\Parser.h"
#include "jpeg\Exif.h"
#include "jpeg\Jfif.h"
#include "jpeg\Xmp.h"
#include "IO.h"

#include <utility>

using namespace jpeg;

Parser::Parser(Buffer && jpeg_data) :
	jpeg_data( std::move(jpeg_data) ),
	readBytes(0)
{}

const Slice Parser::readNextDataSlice() const {
	return Slice(
		jpeg_data.getSize() - readBytes, 
		jpeg_data.getData().get() + readBytes );
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

	const Marker marker(readNextDataSlice());
	if (marker.getMarker() == 0xFF)
		return true;
	
	return false;
}

std::unique_ptr<Marker> Parser::getNextMarker() const 
{
	const Slice nextDataSlice = readNextDataSlice();
	std::unique_ptr<Marker> marker = std::make_unique<Marker>(nextDataSlice);

	uint8_t marker_code = marker->getCode();
	switch (marker_code)
	{
	case jpeg::Marker::Type::SOI:
		marker = std::make_unique<Jfif>(nextDataSlice);
		break;
	case jpeg::Marker::Type::APP1:
	{
		uint32_t id = marker->getIdentifier();
		switch (id) 
		{
		case Exif::Id_code:
			marker = std::make_unique<Exif>(nextDataSlice);
			break;
		case Xmp::Id_code:
			marker = std::make_unique<Xmp>(nextDataSlice);
			break;
		}

		break;
	}
	case jpeg::Marker::Type::APP0:
	default:
		break;
	}

	return marker;
}
