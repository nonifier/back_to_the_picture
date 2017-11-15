
#include "Utils.h"
#include "jpeg\Marker.h"
#include "jpeg\Parser.h"
#include "Defer.h"
#include "Buffer.h"
#include "BufferWritter.h"

#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <ctime>
#include <iomanip>

#include <array>

namespace utils {

size_t size(std::istream& stream) 
{	
	stream.seekg(0, std::ios_base::end);
	Defer( stream.seekg(0, std::ios_base::beg) );

	return size_t(stream.tellg());
}

std::ostream& Help::echo(std::ostream& o) const {
	o << "BackToThePicture.exe	\"fileName\" \"creationDate\" \n";
	o << " - filename: path to Jpeg file\n";
	o << " - creationDate: creation date of the JPEG file (YYYY-MM-DD)\n";
	return o;
}

std::ostream& operator<<(std::ostream& o, const Help& h) {
	return h.echo(o);
}

std::fstream open_file(const std::string& filename) {
	auto file_opt = std::fstream::binary | std::fstream::in;
	std::fstream file(filename, file_opt);

	if (file.is_open()) {
		return file;
	}
	
	std::stringstream ss;
	ss << "Cannot open file: " << filename << "\n";
	throw std::invalid_argument(ss.str());
}

Buffer readFileToBuffer(const std::string& jpegFileName) 
{
	auto jpeg_file = open_file(jpegFileName);
	Buffer fileBuffer(size(jpeg_file));
	jpeg_file >> fileBuffer;

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

std::fstream getOutputStreamFile() {
	std::string outputFileName = constructOutputFileName();
	std::fstream outFile(outputFileName, std::fstream::binary | std::fstream::out);

	if (!outFile.is_open()) {
		std::stringstream ss;
		ss << "ERROR - cannot open output file: " << outputFileName;

		throw std::runtime_error(ss.str());
	}

	return outFile;
}

} // namespace utils



using namespace utils;

int __main__(int argc, const char** argv)
{
	try {
		std::string jpegFileName = readJpegFileNameFromArg(argc, argv);
		Buffer fileBuffer = readFileToBuffer(jpegFileName);
		jpeg::Parser parser(fileBuffer);

		std::vector<jpeg::Parser::MarkerPtr> markers;
		parser.iterateMarkers([&](jpeg::Parser::MarkerPtr marker) {
			std::cout << *marker;
			markers.push_back(std::move(marker));

			std::string marker_name = marker->getName();
			// make marker name to lower case
		});

		std::fstream outFile = getOutputStreamFile();
		std::for_each(markers.cbegin(), markers.cend(), [&](const jpeg::Parser::MarkerPtr& marker) {
			outFile << *marker;
		});

	}
	catch (std::exception& e) {
		std::cerr << "\nERROR - " << e.what();
		return 0;
	}

	return 1;
}

