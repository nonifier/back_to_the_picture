
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

class Help 
{
public:
	std::ostream& operator<<(std::ostream& o) const {
		o << "BackToThePicture.exe	fileName creationDate \n";
		o << "\t-filename: path to Jpeg file\n";
		o << "\t-creationDate: creation date of the JPEG file (YYYY-MM-DD)\n";
		return o;
	}
};

std::ostream& operator<<(std::ostream& o, const Help& h) {
	o << h;
	return o;
}

int main(int argc, const char** argv)
{
	if (argc < 2) {
		Help help;
		std::cerr << "Unsifiscient number of parameter \n\n";
		std::cout << help;
		return 0;
	}

	std::string jpegFileName(argv[1]);
	std::string jpegFileDqte(argv[2]);

	std::fstream jpegFile(jpegFileName, std::fstream::binary | std::fstream::in);

	if (jpegFile.is_open() == false) {
		std::cerr << "Cannot open file: " << jpegFileName << "\n";
		return 0;
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

	return 1;
}