#include "jpeg\Xmp.h"
#include "IO.h"
#include "StringUtil.h"

#include <algorithm>
#include <sstream>
#include <iomanip>

using namespace jpeg;

Xmp::Xmp(const Slice jpeg_data) :
	Marker(jpeg_data)
{
	header = ReinterpretSliceToMarkerHeader<Xmp_header>(jpeg_data);

	const char * packet_ptr = reinterpret_cast<char*>(jpeg_data.getPtr());
	packet_ptr += sizeof(Xmp_header);
	packet = std::move(std::string(packet_ptr));	
}

std::string Xmp::getName() const { 
	return "Xmp"; 
}

std::string Xmp::getInfo() const { 
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
