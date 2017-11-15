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

	std::string readJpegFileNameFromArg(
		int argc, const char** argv);

	std::string getLocalTime();
	std::string constructOutputFileName();
	std::fstream getOutputStreamFile();

} // namespace utils