#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <string_view>

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

        GLuint createShaderProgram(std::string_view vrtxShdrPth, std::string_view fragShdrPth)
        {
            
            const char* vShdrSrc = vrtxShdrPth.c_str();
            const char* fShdrSrc = fragShdrPth.c_str();

            GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
            GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

            glShaderSource(vShader, 1, &vShdrSrc, NULL);
            glShaderSource(fShader, 1, &fShdrSrc, NULL);
            glCompileShader(vShader);
            glCompileShader(fShader);
            // Check errors
            smmo::error::printShaderLog(vShader);
            smmo::error::printShaderLog(fShader);

            GLuint vfProgram = glCreateProgram();
            glAttachShader(vfProgram, vShader);
            glAttachShader(vfProgram, fShader);
            // Link program actually creates executables for vertex and fragment
            // shaders that will be executed on the GPU.
            glLinkProgram(vfProgram);

            // Check errors
            smmo::error::printProgramLog(vfProgram);

            return vfProgram;

        }
	}
}