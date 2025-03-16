#pragma once

#include <string>
#include <unordered_map>

#include <glm/vec4.hpp>
#include <glm/glm.hpp>


struct ShaderSource {
    std::string vertexShaderSource;
    std::string fragmentShaderSource;
};


class Shader {
public:
    Shader();
    Shader(const std::string& vertexShaderFilepath, const std::string& fragmentShaderFilepath);
    ~Shader();

    Shader& operator=(const Shader& shader);

    void bind() const;
    void unbind() const;

    void setUniformVec4(const std::string& name, glm::vec4 vec4);
    void setUniformVec3(const std::string& name, glm::vec3 vec3);
    void setUniformFloat(const std::string& name, float value);
    void setUniformInt(const std::string& name, int value);
    void setUniformMat4(const std::string& name, const glm::mat4& mat4);

    int getUniformLocation(const std::string& name);

private:
    unsigned int _id;
    ShaderSource _source;
    std::unordered_map<std::string, int> _uniformLocationCache;

    ShaderSource _parseShaderSource(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath) const;
    unsigned int _createShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
    static unsigned int _compileShader(unsigned int type, const std::string& source);

    std::string _readFile(const std::string& filePath) const;
};