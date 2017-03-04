
#include "jpeg\Marker.h"

using namespace jpeg;

Marker::~Marker() {}

std::ostream& operator<<(std::ostream& o, const Marker& marker) {
	o << "# " << marker.getName() << "\n";
	o << marker.getInfo() << "\n";
	return o;
}
