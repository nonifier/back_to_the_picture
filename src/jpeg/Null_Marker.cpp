#include "jpeg\Null_Marker.h"

using namespace jpeg;

Null_Marker::Null_Marker() {}
Null_Marker::~Null_Marker() {}

std::string Null_Marker::getName() const {
	return std::string("Null Marker");
}

std::string Null_Marker::getInfo() const {
	return std::string("");
}

uint16_t Null_Marker::getSize() const {
	return uint16_t(0);
}

Slice_const Null_Marker::getHeaderSlice() const {
	return Slice_const();
}

Slice_const Null_Marker::getDataSlice() const {
	return Slice_const();
}