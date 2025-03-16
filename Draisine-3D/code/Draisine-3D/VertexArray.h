#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray {
public:
    VertexArray();
    ~VertexArray();

    void link(const VertexBuffer& vertexBuffer, const VertexBufferLayout& vertexBufferLayout);
    void bind() const;
    void unbind() const;

private:
    unsigned int _id;
};
