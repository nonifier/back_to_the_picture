
#include "jpeg\Marker.h"
#include "jpeg\Parser.h"
#include "Defer.h"
#include "Buffer.h"
#include "BufferWritter.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <ctime>
#include <iomanip>

size_t size(std::fstream& stream) 
{	
	stream.seekg(0, std::fstream::end);
	Defer( stream.seekg(0, std::ios_base::beg) );

	return size_t(stream.tellg());
}

class Help 
{
public:
	std::ostream& echo(std::ostream& o) const {
		o << "BackToThePicture.exe	\"fileName\" \"creationDate\" \n";
		o << " - filename: path to Jpeg file\n";
		o << " - creationDate: creation date of the JPEG file (YYYY-MM-DD)\n";
		return o;
	}
};

std::ostream& operator<<(std::ostream& o, const Help& h) {
	return h.echo(o);
}

Buffer readFileToBuffer(const std::string& jpegFileName) {

	std::fstream jpegFile(jpegFileName, std::fstream::binary | std::fstream::in);

	if (jpegFile.is_open() == false) {
		std::stringstream ss;
		ss << "Cannot open file: " << jpegFileName << "\n";
		throw std::invalid_argument(ss.str());
	}

	Buffer fileBuffer(size(jpegFile));
	jpegFile >> fileBuffer;

	return fileBuffer;
}

std::string readJpegFileNameFromArg(int argc, const char** argv) {
	if (argc < 2) {
		Help help;
		std::stringstream ss;
		ss << "Unsifiscient number of parameter \n\n" << help;
		throw std::invalid_argument(ss.str());
	}

	std::string fileName(argv[1]);
	return fileName;
}

std::string getLocalTime() {
	std::tm sysTime;
	std::time_t timeT = std::time(nullptr);

	localtime_s(&sysTime, &timeT);

	static std::tm prevSysTime = { 0 };
	static uint32_t counter = 0;

	std::stringstream ss;
	ss << std::put_time(&sysTime, "%Y-%m-%d_%H.%M.%S") << "_" << counter;
	return ss.str();
}

std::string constructOutputFileName() {
	std::string outputFileName("output_");
	outputFileName += getLocalTime();
	outputFileName += ".jpeg";

	return outputFileName;
}

int main(int argc, const char** argv)
{
	try {
		std::string jpegFileName = readJpegFileNameFromArg(argc, argv);
		Buffer fileBuffer = readFileToBuffer(jpegFileName);
		jpeg::Parser parser(fileBuffer);

		std::string outputFileName = constructOutputFileName();
		std::fstream outFile(outputFileName, std::fstream::binary | std::fstream::out);

		if (!outFile.is_open()) {
			std::cerr << "ERROR - cannot open output file: " << outputFileName << "\n";
			return 0;
		}

		parser.iterateMarkers([&](jpeg::Parser::MarkerPtr marker) {
			std::cout << *marker;
			outFile << *marker;
		});
	}
	catch (std::exception& e) {
		std::cerr << "\nERROR - " << e.what();
		return 0;
	}
	
	return 1;
}