#pragma once

#include <glad/glad.h>

#include <string>
#include <vector>

#include "Entity.hpp"
#include "../core/Shader.hpp"

class Cube : public Entity {
public:
    Cube(glm::vec3 position = glm::vec3(0.0f));
    void init() override;
    void draw(Shader& shader) override;
    void update(float deltaTime) override;
    static void cleanUp();

private:
    static unsigned int VAO, VBO, EBO, texture;
    static bool initialized;

    float speed = 4.0f;
};

