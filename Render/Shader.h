#pragma once

#include <string>
#include <iostream>
#include <fstream>

namespace smmo
{
	namespace shader
	{
		namespace io
		{
			std::string readShaderSrc(const char* path)
			{
				std::string content;
				std::ifstream fs(path);
				std::string line = "";

				while (!fs.eof())
				{
					std::getline(fs, line);
					content.append(line + "\n");
				}
				fs.close();
				return content;
			}
		}
	}
}