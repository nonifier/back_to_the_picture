
#pragma once

#include "jpeg\Marker.h"

namespace jpeg {

	class Null_Marker: 
		public Marker 
	{
	public:
		Null_Marker();
		virtual ~Null_Marker();

		virtual std::string getName() const override;
		virtual std::string getInfo() const override;
		virtual uint16_t getSize() const override;
	};
}