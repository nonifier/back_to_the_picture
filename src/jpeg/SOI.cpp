
#include "io\IO.h"
#include "jpeg\SOI.h"

#include <sstream>

using namespace jpeg;

SOI::SOI(Slice s) {
	header = ReinterpretSliceToMarkerHeader<SOI_Header>(s);
}

SOI::~SOI() {}

std::string SOI::getName() const { 
	return "SOI"; 
}

std::string SOI::getInfo() const {
	std::stringstream ss;

	ss << std::hex << std::showbase;
	ss << "code: " << getCode() << "\n";

	return ss.str();
}

uint16_t SOI::getSize() const {
	return sizeof(header);
}

Slice_const jpeg::SOI::getHeaderSlice() const {
	const uint8_t* header_ptr = reinterpret_cast<const uint8_t*>(&header);
	size_t header_size = sizeof(header);

	return Slice_const(header_ptr, header_size);
}

uint16_t SOI::getCode() const {
	return io::readUint16<2>(header.code);
}

Slice_const SOI::getDataSlice() const
{
	return Slice_const();
}
