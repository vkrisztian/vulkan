#include "FileReader.h"


//a glslb�l egy byte alap� spir-v file lesz compile ut�n ez�rt byteokat kell olvasni
std::vector<char> FileReader::ReadShader(const std::string & filename)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open())
	{
		throw std::runtime_error("failed to open file!");
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);
	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();

	return buffer;
}
