#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

#include <fstream>
#include <sstream>
#include <cstring>

#include "Shader.hpp"

Shader::Shader(){}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath){
    auto vertexShaderSource = readFile("../shaders/shader.vert");
    auto fragmentShaderSource = readFile("../shaders/shader.frag");
    const char* vertexShaderSrc = vertexShaderSource.get();
    const char* fragmentShaderSrc = fragmentShaderSource.get();

    // compile vertex
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);


    glShaderSource(vertexShader, 1, &vertexShaderSrc, nullptr);
    glCompileShader(vertexShader);

    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);


    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        throw std::runtime_error("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" + std::string(infoLog));
    }

    // compile fragment
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        throw std::runtime_error("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" + std::string(infoLog));
    }

    // shader program
    id = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glLinkProgram(id);

    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        throw std::runtime_error("ERROR::SHADER::PROGRAM::LINK_FAILED\n" + std::string(infoLog));
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
}

void Shader::use(){
    glUseProgram(id);
}

void Shader::setBool(const std::string& name, bool value) const{
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value); 
}
void Shader::setInt(const std::string& name, int value) const{
    glUniform1i(glGetUniformLocation(id, name.c_str()), value); 
}
void Shader::setFloat(const std::string& name, float value) const{
    glUniform1f(glGetUniformLocation(id, name.c_str()), value); 
}
void Shader::setMat4(const std::string& name, const glm::mat4& value) const{
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

std::unique_ptr<char[]> readFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filePath);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    std::unique_ptr<char[]> result(new char[content.size() + 1]);
    std::strcpy(result.get(), content.c_str());

    return result;
}
