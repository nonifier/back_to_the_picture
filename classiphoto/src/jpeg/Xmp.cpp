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
	
	static const std::string createDateLabel = "CreateDate";
	
	size_t creationDateStartOffet = packet.find(createDateLabel);
	if (creationDateStartOffet != std::string::npos) {
		std::string creationDateStart(packet.substr(creationDateStartOffet));
		std::string creationDateStr = creationDateStart.substr(createDateLabel.size());

		size_t creationDateEndOffset = creationDateStr.find(createDateLabel);
		creationDateStr = creationDateStr.substr(0, creationDateEndOffset);
		creationDateStr = removeBeforeChar(creationDateStr, '>');
		creationDateStr = removePastChar(creationDateStr, '<');

		std::stringstream ss(creationDateStr);
		ss >> std::get_time(&createDate, "%Y-%m-%d");
	}
}

std::string Xmp::getName() const { return "Xmp"; }
std::string Xmp::getInfo() const { 
	std::stringstream ss;

	ss << "- Xmp -\n";
	ss << "Create Date: " << std::put_time(&createDate, "%F\n");

	return ss.str(); 
}
uint16_t Xmp::getSize() const { return io::readUint16<2>(header.size); }
