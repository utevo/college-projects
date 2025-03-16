#include "IndexBuffer.h"

#include "Utilities.h"


IndexBuffer::IndexBuffer() {
    GLCall(glGenBuffers(1, &_id));
}

IndexBuffer::IndexBuffer(const unsigned int* indices, unsigned int count)
    : _count(count) {
    GLCall(glGenBuffers(1, &_id));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer() {
    GLCall(glDeleteBuffers(1, &_id));
}

void IndexBuffer::bind() const {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id));
}

void IndexBuffer::unbind() const {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}