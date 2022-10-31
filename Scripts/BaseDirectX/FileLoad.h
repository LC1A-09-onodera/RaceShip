#pragma once
#include <string>

class FileLoad
{
	enum FileType
	{
		TEXT,
		CSV,
	};

	static void LoadFile(std::string path);
};