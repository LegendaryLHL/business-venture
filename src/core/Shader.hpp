#pragma once

#include <glm/glm.hpp>

#include <string>
#include <memory>

class Shader{
    public:
        Shader();
        Shader(const std::string& vertexPath, const std::string& fragmentPath);

       void use();

       void setBool(const std::string &name, bool value) const;
       void setInt(const std::string &name, int value) const;
       void setFloat(const std::string &name, float value) const;
       void setMat4(const std::string &name, const glm::mat4& value) const;

    private:
       unsigned int id; 
};

/**
 * @brief Reads a file and returns a std::unique_ptr<char[]> to the buffer.
 * 
 * Uses `std::unique_ptr<char[]>` to manage memory automatically, eliminating 
 * the need for manual deallocation.
 * 
 * @param filePath Path to the file to be read.
 * @return std::unique_ptr<char[]> A buffer containing the file contents, or throw an error if the file cannot be opened.
 */
std::unique_ptr<char[]> readFile(const std::string& filePath);

