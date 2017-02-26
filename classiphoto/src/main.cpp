
#include "jpeg\Marker.h"
#include "jpeg\Parser.h"
#include "Defer.h"
#include "Buffer.h"

#include <fstream>
#include <iostream>

size_t size(std::fstream& stream) 
{	
	stream.seekg(0, std::fstream::end);
	Defer( stream.seekg(0, std::ios_base::beg) );

	return size_t(stream.tellg());
}

int main(int argc, const char** argv)
{
	//const char* jpegFileName = "dsc05869_origin_with_date.jpg";
	std::string jpegFileName;
	std::cin >> jpegFileName;
	
	std::fstream jpegFile(jpegFileName, std::fstream::binary | std::fstream::in);

	if (jpegFile.is_open() == false) {
		std::cerr << "Cannot open file: " << jpegFileName << "\n";
		return 1;
	}

	Buffer fileBuffer( size(jpegFile) );
	jpegFile >> fileBuffer;

	jpeg::Parser parser( std::move(fileBuffer) );
	while (parser.hasNextMarker())
	{
		std::unique_ptr<jpeg::Marker> marker = parser.getNextMarker();
		std::cout << marker->getInfo() << "\n";
		parser.advanceJpegDaga(marker->getSize());
	}

	return 0;
}