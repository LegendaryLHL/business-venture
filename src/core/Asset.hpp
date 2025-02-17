#pragma once
#include <string>
#include <vector>
#include <unordered_map>

class Asset{
    public: 
        enum class Texture {
            BUILDING,
            GRASS,
        };

        enum class Vertex {
            CUBE,
        };

        struct VertexData {
            unsigned int vao;
            unsigned int vbo;
            unsigned int ebo;
            unsigned int num;
        };

        static std::unordered_map<Vertex, VertexData> vertexMap;
        static std::unordered_map<Texture, unsigned int> textureMap;

        static void load();
        static void cleanUp();

    private:
        static unsigned int loadTexture(std::string texturePath);
        static VertexData loadVertex(std::vector<float>& vertices, std::vector<unsigned int>& indices);
};
