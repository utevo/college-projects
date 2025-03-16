#pragma once

class IndexBuffer
{
public:
    IndexBuffer();
    IndexBuffer(const unsigned int* indices, unsigned int count);
    ~IndexBuffer();

    void bind() const;
    void unbind() const;

    unsigned int getCount() const { return _count; }

private:
    unsigned int _id;
    unsigned int _count;
};