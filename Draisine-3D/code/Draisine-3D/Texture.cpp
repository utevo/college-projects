#include "Texture.h"

#include <SOIL.h>
#include <GLFW/glfw3.h>

#include "Utilities.h"


Texture::Texture() {
    GLCall(glGenTextures(1, &_id));
};

Texture::Texture(const std::string& filePath, const bool toRepeat) {
    _buffer = SOIL_load_image(filePath.c_str(), &_width, &_height, &_channels, 4);
    ASSERT(_buffer);

    GLCall(glGenTextures(1, &_id));
    GLCall(glBindTexture(GL_TEXTURE_2D, _id)); // Bind without slot selection

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	if (toRepeat)
	{
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	}
	else
	{
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	}

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _buffer));
    unbind();

    if (_buffer)
        SOIL_free_image_data(_buffer);
};

Texture::~Texture() {
    GLCall(glDeleteTextures(1, &_id));
}

void Texture::bind(unsigned int slot) const {
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, _id));
}

void Texture::unbind() const {
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}