#include "jpeg\Xmp.h"
#include "io\IO.h"
#include "StringUtil.h"

#include <algorithm>
#include <sstream>
#include <iomanip>

using namespace jpeg;

Xmp::Xmp(const Slice jpeg_data) :
	header(ReinterpretSliceToMarkerHeader<Xmp_header>(jpeg_data)),
	packet(
		extract_packet(
			jpeg_data.sub_slice(sizeof(header))))
{}

std::string Xmp::extract_packet(const Slice data)
{
	auto packet_ptr = reinterpret_cast<char*>(data.getPtr());
	auto packet_ptr_end = std::next(packet_ptr);
	auto data_size = data.getSize();

	while (*packet_ptr_end != Marker::MARKER 
		&& data_size) 
	{
		packet_ptr_end++;
		data_size--;
	}

	auto packet_size = std::distance(packet_ptr, packet_ptr_end);

	return std::string(packet_ptr, packet_size);
}

std::string Xmp::getName() const 
{ 
	return "Xmp"; 
}

std::string Xmp::getInfo() const 
{ 
	std::stringstream ss;
	ss << "size: " << std::hex << std::showbase << getSize() << "\n";
	ss << packet << "\n";

	try {
		std::tm creationDate = getCreateDate();
		ss << "Create Date: " << std::put_time(&creationDate, "%F\n");
	}catch(...){}

	return ss.str(); 
}

uint16_t Xmp::getSize() const {
	return io::readUint16<2>(header.size) + 2;
}

std::tm Xmp::getCreateDate() const {
	try {
		std::string creationDateStr = extractTag("CreationDate");
		std::stringstream ss(creationDateStr);
		std::tm creationDate;
		ss >> std::get_time(&creationDate, "%Y-%m-%d");

		return creationDate;
	}
	catch (const std::exception& e) {
		throw e;
	}
}

Slice_const Xmp::getHeaderSlice() const {
	return getSliceFromHeader(header);
}

Slice_const Xmp::getDataSlice() const {
	const uint8_t* packet_ptr = reinterpret_cast<const uint8_t*>(packet.c_str());
	size_t packet_size = packet.size();

	return Slice_const(packet_ptr, packet_size);
}

std::string Xmp::extractTag(const std::string& tagName) const {
	size_t tagStartOffet = packet.find(tagName);

	if (tagStartOffet == std::string::npos) {
		std::stringstream ss("Opening tag not found: ");
		ss << tagName;
		throw std::runtime_error(ss.str());
	}

	std::string tagStart(packet.substr(tagStartOffet));
	std::string tagStr = tagStart.substr(tagName.size());
	size_t tagEndOffset = tagStr.find(tagName);

	if (tagEndOffset == std::string::npos) {
		std::stringstream ss("Closing tag not found: ");
		ss << tagName;
		throw std::runtime_error(ss.str());
	}

	tagStr = tagStr.substr(0, tagEndOffset);
	tagStr = removeBeforeChar(tagStr, '>');
	tagStr = removePastChar(tagStr, '<');

	return tagStr;
}
