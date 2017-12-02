#pragma once

#include "jpeg\Marker.h"
#include "Slice.h"
#include <memory>

namespace jpeg {

	class Exif : public Marker
	{
	public:
		const static uint32_t Id_code = 0x45786966;

		Exif(const Slice jpeg_data);
		virtual ~Exif();
		
		// public Marker
		virtual std::string getName() const override;
		virtual std::string getInfo() const override;
		virtual uint16_t getSize() const override;
		virtual Slice_const getHeaderSlice() const override;
		virtual Slice_const getDataSlice() const override;
		uint32_t getIdentifier() const;

	private:
		struct Exif_header {
			uint8_t	marker[2];	//	0xffe1
			uint8_t	size[2];	// 0x09a0
			uint8_t	identifier[6]; // "Exif00"
			uint8_t	alignment[2];	// 0x4949="II" - Intel 
									// 0x4d4d="MM" - Motorola
			uint8_t	ctor_const[2];	// 0x2a00 - if Intel
									// 0x002a - if Motorola
			uint8_t offset[8];		// 0x00000008
		} header;
	};

	/*
	IFD : Image file directory

	uint16_t directory_entry_number - 0x2
	uint16_t tag_number; // shows a kind of data (IFD0 = 0x8769)
	uint16_t data_format;
	uint32_t component_number;
	uint32_t value;
	uint32_t offset_to_next_IFD; // 0x00000000 if no other IFD

	Data format
	1 unsigned byte		1B
	2 ascii strings 	1B
	3 unsigned short 	2B
	4 unsigned long 	4B
	5 unsigned rational 8B
	6 signed byte		1B
	7 undefined 		1B
	8 signed short 		2B
	9 signed long 		4B
	10 signed rational 	8B
	11 single float 	4B
	12 double float		8B

	*/


	/*
	uint16_t		marker	- ffe1
	uint16_t		size	- 09a0
	UTF8-Data			- http://ns.adobe.com/xap/1.0/
	\0
	UTF8-Data			-
	<?xpacket begin='﻿' id='W5M0MpCehiHzreSzNTczkc9d'?>
	<x:xmpmeta xmlns : x = "adobe:ns:meta/">
	<rdf:RDF xmlns : rdf = "http://www.w3.org/1999/02/22-rdf-syntax-ns#">
	<rdf:Description rdf : about = "uuid:faf5bdd5-ba3d-11da-ad31-d33d75182f1b" xmlns : xmp = "http://ns.adobe.com/xap/1.0/">
	<xmp:CreateDate>2017 - 02 - 12T13:45 : 40.089< / xmp : CreateDate>
	</ rdf : Description>
	< / rdf : RDF>
	< / x : xmpmeta>
	*/
}
