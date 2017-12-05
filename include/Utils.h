#pragma once 

#include "Buffer.h"
#include <fstream>

namespace utils {

	class Help 
	{
	public:
		std::ostream& echo(std::ostream& o) const;
	};

	size_t size(std::istream& stream);

	std::ostream& operator<<(
		std::ostream& o, const Help& h);

	std::fstream open_file(
		const std::string& filename);

	Buffer read_stream_to_buffer( 
		std::istream& streeam);

	Buffer read_file_to_buffer(
		const std::string& filename);

	std::string readJpegFileNameFromArg(
		int argc, const char** argv);

	template<class T, size_t S>
	size_t get_array_size(std::array<T, S> arr) {
		return arr.size() * sizeof(T);
	}

	std::string getLocalTime();
	std::string constructOutputFileName();
	std::fstream getOutputStreamFile();

} // namespace utils