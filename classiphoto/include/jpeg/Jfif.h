#pragma once
#pragma once
#include "jpeg\Marker.h"
#include "Slice.h"
#include <memory>

namespace jpeg {
	
	class Jfif : public Marker
	{
	public:
		Jfif(const Slice jpeg_data);
		virtual ~Jfif();
		virtual std::string getName() const override;
		virtual std::string getInfo() const override;
		virtual uint16_t getSize() const override;

	private:
		struct Jfif_header
		{
			uint8_t start_of_image[2];	/* 00h  Start of Image Marker     */
			uint8_t application_use_marker[2];	/* 02h  Application Use Marker    */
			uint8_t length[2];       /* 04h  Length of APP0 Field      */
			uint8_t identifier[5];   /* 06h  "JFIF" (zero terminated) Id String */
			uint8_t version[2];      /* 07h  JFIF Format Revision      */
			uint8_t units;           /* 09h  Units used for Resolution */
			uint8_t x_density[2];     /* 0Ah  Horizontal Resolution     */
			uint8_t y_density[2];     /* 0Ch  Vertical Resolution       */
			uint8_t x_thumbnail;      /* 0Eh  Horizontal Pixel Count    */
			uint8_t y_thumbnail;      /* 0Fh  Vertical Pixel Count      */
		} header;
	};

}