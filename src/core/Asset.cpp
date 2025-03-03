#include "glad/glad.h"
#include "stb/stb_image.h"

#include <stdexcept>

#include "Asset.hpp"

std::unordered_map<Asset::Vertex, Asset::VertexData> Asset::vertexMap;
std::unordered_map<Asset::Texture, unsigned int> Asset::textureMap;

void Asset::load() {
    std::vector<float> cubeVertices = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    };
    std::vector<unsigned int> cubeIndices = {
        0, 1, 2, 2, 3, 0,  
        4, 5, 6, 6, 7, 4,  
        0, 1, 5, 5, 4, 0,  
        2, 3, 7, 7, 6, 2,  
        1, 2, 6, 6, 5, 1,  
        3, 0, 4, 4, 7, 3   
    };
    Asset::vertexMap[Vertex::CUBE] = loadVertex(cubeVertices, cubeIndices);

    Asset::textureMap[Texture::BUILDING] = loadTexture("assets/image/building.jpg");
    Asset::textureMap[Texture::GRASS] = loadTexture("assets/image/grass.jpg");
    Asset::textureMap[Texture::TUT] = loadTexture("assets/image/tut.jpg");
}

unsigned int Asset::loadTexture(std::string texturePath) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    } else {
        throw std::runtime_error("Failed to load texture at: " + texturePath);
    }

    return textureID;
}


Asset::VertexData Asset::loadVertex(std::vector<float>& vertices, std::vector<unsigned int>& indices){
    VertexData data;
    data.vao = 0;
    data.vbo = 0;
    data.ebo = 0;
    data.num = indices.size();

    glGenVertexArrays(1, &data.vao);
    glGenBuffers(1, &data.vbo);
    glGenBuffers(1, &data.ebo);

    glBindVertexArray(data.vao);

    glBindBuffer(GL_ARRAY_BUFFER, data.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    return data;
}

void Asset::cleanUp(){
    for (const auto& entry : Asset::vertexMap) {
        glDeleteVertexArrays(1, &entry.second.vao);
        glDeleteBuffers(1, &entry.second.vbo);
        glDeleteBuffers(1, &entry.second.ebo);
    }
}
