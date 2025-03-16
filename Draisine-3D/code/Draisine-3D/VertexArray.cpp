#include "VertexArray.h"
#include "Utilities.h"

VertexArray::VertexArray() {
    GLCall(glGenVertexArrays(1, &_id));
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &_id));
}

void VertexArray::link(const VertexBuffer& vertexBuffer, const VertexBufferLayout& vertexBufferLayout)
{
    bind();
    vertexBuffer.bind();
    const std::vector<VertexBufferElement> elements = vertexBufferLayout.getElements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const VertexBufferElement element = elements[i];
        GLCall(glEnableVertexAttribArray(i));
        GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized,
            vertexBufferLayout.getSize(), (void*)(uintptr_t)(offset)));
        offset += element.count * GLGetSizeOfType(element.type);
    }
}

void VertexArray::bind() const {
    GLCall(glBindVertexArray(_id));
}

void VertexArray::unbind() const {
    GLCall(glBindVertexArray(0));
};