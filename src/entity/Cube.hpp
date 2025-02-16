#pragma once

#include <glad/glad.h>
#include <vector>

#include "Entity.hpp"
#include "../core/Shader.hpp"

class Cube : public Entity {
public:
    Cube(glm::vec3 position = glm::vec3(0.0f));
    void init() override;  // Initializes cube buffers
    void draw(Shader& shader) override;  // Renders the cube
    static void cleanUp(); // Deletes VAO/VBO

private:
    static unsigned int VAO, VBO, EBO;  // Shared among all cubes
    static bool initialized;  // Flag to track if `init()` was called
};

