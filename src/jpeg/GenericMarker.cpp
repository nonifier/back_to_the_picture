
#include "jpeg\GenericMarker.h"
#include "io\IO.h"
#include "Buffer.h"

using namespace jpeg;

GenericMarker::GenericMarker(Slice s) {
	header = ReinterpretSliceToMarkerHeader<Marker_header>(s);
	size_t marker_size = getSize();

	const uint8_t* data_ptr = s.getPtr() + sizeof(header);
	size_t data_size = marker_size - sizeof(header);

	data = std::move(Buffer(data_size));
	data.write(data_ptr, data_size);
}

GenericMarker::~GenericMarker() {}

std::string GenericMarker::getName() const {
	return std::string("Marker");
}

std::string GenericMarker::getInfo() const {
	std::stringstream strm;
	strm << std::hex << std::showbase;
	strm << "marker: " << int(getMarker()) << "\n";
	strm << "code: " << int(getCode()) << "\n";
	strm << "size: " << getSize() << "\n";
	return strm.str();
}

uint16_t GenericMarker::getSize() const {
	return io::readUint16<2>(header.size) + 2;
}

Slice_const GenericMarker::getHeaderSlice() const {
	return getSliceFromHeader(header);
}

Slice_const GenericMarker::getDataSlice() const {
	return Slice_const(data);
}

uint8_t GenericMarker::getCode() const {
	return header.code;
}

uint8_t GenericMarker::getMarker() const {
	return header.marker;
}

uint32_t GenericMarker::getIdentifier() const {
	return io::readSize_t<4>(header.identifier);
}

uint8_t* GenericMarker::getNextMarkerPointer()
{
	size_t offset = getSize();
	uint8_t* start_marker_ptr = header.size;
	return start_marker_ptr + offset;
}
