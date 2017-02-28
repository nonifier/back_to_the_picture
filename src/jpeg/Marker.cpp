
#include "jpeg\Marker.h"
#include "jpeg\Exif.h"
#include "jpeg\Jfif.h"
#include "IO.h"
#include <sstream>

using namespace jpeg;

Marker::Marker() {}

Marker::Marker(const Slice s) {
	header = ReinterpretSliceToMarkerHeader<Marker_header>(s);
}

Marker::~Marker() {}

std::string Marker::getName() const { 
	return std::string("Marker"); 
}

std::string Marker::getInfo() const { 
	std::stringstream strm;
	strm << std::hex << std::showbase;
	strm << "marker: " << int(getMarker()) << "\n";
	strm << "code: " << int(getCode()) << "\n";
	strm << "size: " << getSize() << "\n";
	return strm.str();
}

uint16_t Marker::getSize() const { 
	return io::readUint16<2>(header.size) + 2; 
}

uint8_t Marker::getCode() const {
	return header.code; 
}

uint8_t Marker::getMarker() const { 
	return header.marker; 
}

uint32_t Marker::getIdentifier() const {
	return io::readSize_t<4>(header.identifier);
}

uint8_t* Marker::getNextMarkerPointer()
{
	size_t offset = getSize();
	uint8_t* start_marker_ptr = header.size;
	return start_marker_ptr + offset;
}

std::ostream& operator<<(std::ostream& o, const Marker& marker) {
	o << "# " << marker.getName() << "\n";
	o << marker.getInfo() << "\n";
	return o;
}