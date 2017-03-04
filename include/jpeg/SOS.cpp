#include "SOS.h"

using namespace jpeg;


SOS::SOS(Slice data) {
	header = Marker::ReinterpretSliceToMarkerHeader<decltype(header)>(data);

	size_t data_size = data.getSize() - sizeof(header);
	uint8_t* data_ptr = data.getPtr() + sizeof(header);

	Slice_const dataSlice(data_ptr, data_size);

	dataBuffer = std::move(Buffer(data_size));
	dataBuffer << dataSlice;
}

SOS::~SOS() {}

std::string SOS::getName() const {
	return "SOS";
}

std::string SOS::getInfo() const {
	std::stringstream ss;
	
	ss << std::hex << std::showbase;
	ss << "code: " << header.code << "\n";
	ss << "id: " << header.id << "\n";
	ss.flags(0);
	ss << "data size: " << dataBuffer.getSize() << "\n";

	return ss.str();
}

uint16_t SOS::getSize() const {
	size_t header_size = sizeof(header);
	size_t data_size = dataBuffer.getSize();
	return uint16_t( header_size + data_size );
}

Slice_const SOS::getHeaderSlice() const {
	return Marker::getSliceFromHeader(header);
}

Slice_const SOS::getDataSlice() const {
	return dataBuffer;
}
