#pragma once

#include <vector>

#include "Utilities.h"


struct VertexBufferElement {
    unsigned int type;
    unsigned int count;
    unsigned char normalized;
};

class VertexBufferLayout
{
public:
    VertexBufferLayout() :
        _size(0) { }

    void addFloat(unsigned int count) { _push(GL_FLOAT, count, GL_FALSE); }
    void addUnsignedInt(unsigned int count) { _push(GL_UNSIGNED_INT, count, GL_FALSE); }
    void addUnsignedByte(unsigned int count) { _push(GL_UNSIGNED_BYTE, count, GL_TRUE); }

    const std::vector<VertexBufferElement> getElements() const { return _elements; };
    unsigned int getSize() const { return _size; };

private:
    unsigned int _size;
    std::vector<VertexBufferElement> _elements;

    void _push(unsigned int type, unsigned int count, unsigned char normalized) {
        _elements.push_back({ type, count, normalized });
        _size += count * GLGetSizeOfType(type);
    };


};