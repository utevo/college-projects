#include "Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Utilities.h"

Shader::Shader() {
    _id = glCreateProgram();
}

Shader& Shader::operator=(const Shader& shader) {
    _id = shader._id;
    _source = shader._source;
    _uniformLocationCache = shader._uniformLocationCache;

    return *this;
}

Shader::Shader(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath) {
    _source = _parseShaderSource(vertexShaderFilePath, fragmentShaderFilePath);
    _id = _createShader(_source.vertexShaderSource, _source.fragmentShaderSource);
    GLCall(glUseProgram(_id));
}

std::string Shader::_readFile(const std::string& filePath) const {
    std::ifstream file(filePath);
    std::stringstream stringStream;
    stringStream << file.rdbuf();
    std::string result = stringStream.str();
    return result;
}

ShaderSource Shader::_parseShaderSource(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath) const {
    std::string vertexShaderSource = _readFile(vertexShaderFilePath);
    std::string fragmentShaderSource = _readFile(fragmentShaderFilePath);

    ShaderSource shaderSource = { vertexShaderSource, fragmentShaderSource };
    return shaderSource;
}

unsigned int Shader::_createShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) {
    GLCall(unsigned int program = glCreateProgram());
    unsigned int vertexShader = _compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fragmentShader = _compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    GLCall(glAttachShader(program, vertexShader));
    GLCall(glAttachShader(program, fragmentShader));
    GLCall(glLinkProgram(program));

    GLint programLinked;
    GLCall(glGetProgramiv(program, GL_LINK_STATUS, &programLinked));
    if (programLinked != GL_TRUE) {
        GLsizei messageLength = 0;
        GLchar message[2000];
        GLCall(glGetProgramInfoLog(program, 2000, &messageLength, message));
        std::cout << "Failed to link program" << std::endl;
        std::cout << message << std::endl;
    }

    GLCall(glValidateProgram(program));
    GLCall(glDeleteShader(vertexShader));
    GLCall(glDeleteShader(fragmentShader));
    return program;
}

unsigned int Shader::_compileShader(unsigned int type, const std::string& shaderSource) {
    GLCall(unsigned int shader = glCreateShader(type));
    const GLchar* shaderSourceAsGLcharPoiter = shaderSource.c_str();
    GLCall(glShaderSource(shader, 1, &shaderSourceAsGLcharPoiter, nullptr));
    GLCall(glCompileShader(shader));

    int shaderCompiled;
    GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderCompiled));
    if (shaderCompiled == GL_FALSE) {
        GLsizei messageLength = 0;
        GLchar message[2000];
        GLCall(glGetShaderInfoLog(shader, 2000, &messageLength, message));
        std::cout << "Failed to compile shader" << std::endl;
        std::cout << message << std::endl;
        GLCall(glDeleteShader(shader));
        return 0;
    }

    return shader;
}

Shader::~Shader() {
    GLCall(glDeleteProgram(_id));
}

int Shader::getUniformLocation(const std::string& name) {
    if (_uniformLocationCache.find(name) != _uniformLocationCache.end())
        return _uniformLocationCache[name];

    GLCall(int location = glGetUniformLocation(_id, name.c_str()));
    if (location == -1)
        std::cout << "No uniform with name " << name << std::endl;

    return _uniformLocationCache[name] = location;
}

void Shader::setUniformVec4(const std::string& name, glm::vec4 vec4) {
    GLCall(glUniform4f(getUniformLocation(name), vec4[0], vec4[1], vec4[2], vec4[3]));
}

void Shader::setUniformVec3(const std::string& name, glm::vec3 vec3) {
    GLCall(glUniform3f(getUniformLocation(name), vec3[0], vec3[1], vec3[2]));
}

void Shader::setUniformFloat(const std::string& name, float value) {
    GLCall(glUniform1f(getUniformLocation(name), value));
}

void Shader::setUniformInt(const std::string& name, int value) {
    GLCall(glUniform1i(getUniformLocation(name), value));
}

void Shader::setUniformMat4(const std::string& name, const glm::mat4& mat4) {
    GLCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat4)));
}

void Shader::bind() const {
    GLCall(glUseProgram(_id));
}

void Shader::unbind() const {
    GLCall(glUseProgram(0));
}