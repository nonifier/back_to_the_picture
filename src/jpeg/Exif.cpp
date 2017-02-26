#include "jpeg\Exif.h"
#include "IO.h"
#include <sstream>

using namespace jpeg;

Exif::Exif(const Slice jpeg_data): 
	Marker(jpeg_data) 
{
	header = ReinterpretSliceToMarkerHeader<Exif_header>(jpeg_data);
}

Exif::~Exif() {}

std::string Exif::getName() const {
	const char* start_name = reinterpret_cast<const char*>(header.identifier);
	return std::string(start_name);
}

std::string jpeg::Exif::getInfo() const {
	std::stringstream sstream;
	sstream << "- Exif -\n";
	sstream << "marker: " << std::hex << std::showbase 
		<< io::readSize_t<2>(header.marker) << '\n';
	sstream << "size: " << io::readSize_t<2>(header.size) << '\n';
	sstream << "id: " << reinterpret_cast<const char*>(header.identifier) << '\n';
	sstream << "alignmt: " << std::hex << std::showbase 
		<< io::readSize_t<2>(header.alignment) << '\n';
	sstream << "constructor: " << std::hex << std::showbase 
		<< io::readSize_t<2>(header.ctor_const) << '\n';
	sstream << "offset: " << std::hex << std::showbase 
		<< io::readSize_t<4>(header.offset) << '\n';

	return sstream.str();
}

uint16_t Exif::getSize() const {
	return io::readUint16<2>(header.size) + 2;
}

uint32_t jpeg::Exif::getIdentifier() const {
	return io::readSize_t<4>(header.identifier);
}
