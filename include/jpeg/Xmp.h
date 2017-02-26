#pragma once

#include "jpeg\Marker.h"

namespace jpeg {

	/*
		Specs: http://www.adobe.com/devnet/xmp.html

		Part 1, Data model, Serialization, and Core Properties
		http://wwwimages.adobe.com/content/dam/Adobe/en/devnet/xmp/pdfs/XMP%20SDK%20Release%20cc-2016-08/XMPSpecificationPart1.pdf
	
		Part 3, Storage in Files 
		http://wwwimages.adobe.com/content/dam/Adobe/en/devnet/xmp/pdfs/XMP%20SDK%20Release%20cc-2016-08/XMPSpecificationPart3.pdf
	*/
	class Xmp : public Marker {
	public:
		const static uint32_t Id_code = 0x68747470;

		Xmp(const Slice jpeg_data);

		virtual std::string getName() const override;
		virtual std::string getInfo() const override;
		virtual uint16_t getSize() const override;

	private:
		struct Xmp_header {
			uint8_t marker;
			uint8_t code;
			uint8_t size[2];
			uint8_t namespaceURI[29];
		} header;

		std::string packet;

		std::string extractTag(const std::string& tagName) const;
		std::tm getCreateDate() const;
	};

}