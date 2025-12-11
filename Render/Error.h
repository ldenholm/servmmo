#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <string>
#include <iostream>

namespace smmo
{
	namespace error
	{
		void printShaderLog(GLuint shader)
		{
			int len = 0;
			int chWritten = 0;

			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
			if (len > 0)
			{
				std::unique_ptr<char[]> log(new char[len]);
				glGetShaderInfoLog(shader, len, &chWritten, log.get());
				std::cout << "Shader Info Log: " << log.get() << std::endl;

			}
		}
		void printProgramLog(int prog)
		{
			int len = 0;
			int chWritten = 0;
			glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
			if (len > 0)
			{
				std::unique_ptr<char[]> log(new char[len]);
				glGetProgramInfoLog(prog, len, &chWritten, log.get());
				std::cout << "Program Info Log: " << log.get() << std::endl;
			}
		}

		bool checkOpenGLError()
		{
			bool foundError = false;
			int glErr = glGetError();
			while (glErr != GL_NO_ERROR)
			{
				std::cout << "glError: " << glErr << std::endl;
				foundError = true;
				glErr = glGetError();
			}
			return foundError;
		}
	}
}