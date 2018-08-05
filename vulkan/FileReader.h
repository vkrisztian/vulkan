#pragma once
#include <fstream>
#include <vector>
static class FileReader
{
public:
	static std::vector<char> ReadShader(const std::string & filename);
};