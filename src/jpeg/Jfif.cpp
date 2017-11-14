
#include "jpeg\Jfif.h"
#include "io\IO.h"
#include <sstream>

using namespace jpeg;

Jfif::Jfif(const Slice jpeg_data) {
	header = ReinterpretSliceToMarkerHeader<Jfif_header>(jpeg_data);
}

Jfif::~Jfif() {}

std::string Jfif::getName() const {
	const char* start_name = reinterpret_cast<const char*>(header.identifier);
	return std::string(start_name);
}

std::string Jfif::getInfo() const {
	std::stringstream sstrm;
	sstrm << "size: " << io::readSize_t<2>(header.length) << " \n";
	sstrm << "id: " << reinterpret_cast<const char*>(header.identifier) << " \n";
	sstrm << "vers: v." << int(header.version[1]) << "." 
		<< int(header.version[0]) << "\n";
	sstrm << "units: " << int(header.units) << "\n";
	sstrm << "density: {" 
		<< io::readSize_t<2>(header.x_density) << ", "
		<< io::readSize_t<2>(header.y_density) << "}\n";
	sstrm << "thumbnail: {"
		<< int(header.x_thumbnail) << ", "
		<< int(header.y_thumbnail) << "}\n";

	return sstrm.str();
}

uint16_t Jfif::getSize() const {
	return io::readUint16<2>(header.length) + 2;
}

Slice_const Jfif::getHeaderSlice() const {
	return getSliceFromHeader(header);
}

Slice_const Jfif::getDataSlice() const {
	return Slice_const();
}
