#pragma once

#include <assert.h>
#include <sstream>

#include <glm/glm.hpp>

#include "Debug.h"


#ifdef DEBUG
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLCheckError())
#else
#define GLCall(x) x
#endif

static unsigned int GLGetSizeOfType(unsigned int type)
{
    switch (type)
    {
    case GL_FLOAT: return sizeof(GLfloat);
    case GL_UNSIGNED_INT: return sizeof(GLuint);
    case GL_UNSIGNED_BYTE: return sizeof(GLbyte);
    }
    ASSERT(false);
    return 0;
}

static std::string mat4ToString(glm::mat4 matrix) {
    std::string result;

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::ostringstream ss;
            float myFloat = (float)matrix[i][j];
            ss << myFloat;
            std::string s(ss.str());
            result += s;
            result += " ";
        }
        result += "\n";
    }
    return result;
}