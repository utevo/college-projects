#include "TexturesMapper.h"

#include <SOIL.h>
#include <GLFW/glfw3.h>

#include "Utilities.h"


TexturesMapper::TexturesMapper(std::vector <std::pair<std::string, std::shared_ptr<Texture> > > mapping, Shader& shader)
: _shader(shader), _mapping(mapping) {
};

TexturesMapper::~TexturesMapper() {
}

void TexturesMapper::bind() {
	unsigned int slot = 0;
	_mapping;
	for (std::pair<std::string, std::shared_ptr<Texture> > pair : _mapping) {
		Texture& texture = *(pair.second);
		std::string name = pair.first;
		texture.bind(slot);
		_shader.setUniformInt(name, slot);
		++slot;
	}
}

void TexturesMapper::unbind() const {
	for (std::pair<std::string, std::shared_ptr<Texture> > pair : _mapping) {
		Texture& texture = *(pair.second);
		std::string name = pair.first;
		texture.unbind();
	}
}