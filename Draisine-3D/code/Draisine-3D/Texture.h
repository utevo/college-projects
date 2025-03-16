#pragma once

#include "Utilities.h"


class Texture {
public:
    Texture();
    Texture(const std::string& filePath, const bool toRepeat = 0);
    ~Texture();

    void bind(unsigned int slot = 0) const;
    void unbind() const;

    inline int getWidth() const { return _width; }
    inline int getHeight() const { return _height; }

private:
    unsigned int _id;
    unsigned char* _buffer;
    int _width;
    int _height;
    int _channels;
};

