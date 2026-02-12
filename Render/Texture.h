#include <GL/glew.h>
#include <SOIL2/SOIL2.h>
#include <iostream>

namespace smmo {
	namespace texture {
		GLuint loadTexture(const char* texImgPth)
		{
			GLuint textureID;
			textureID = SOIL_load_OGL_texture(texImgPth,
				SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
			if (textureID == 0) { std::cout << "Failed to load texture: " << texImgPth << std::endl; }
			return textureID;
		}
	}
}