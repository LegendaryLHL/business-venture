#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../core/Shader.hpp"

#include <string>
#include <vector>

class Entity {
public:
    Entity(glm::vec3 position = glm::vec3(0.0f)) : position(position){}
    virtual ~Entity() = default;

    virtual void init() = 0;
    virtual void draw(Shader &shader) = 0;
    virtual void update(float deltaTime) = 0;

    void setPosition(glm::vec3 pos) { position = pos; }
    glm::vec3 getPosition() const { return position; }
    
protected:
    glm::vec3 position;

    static unsigned int loadTexture(const std::string& texturePath);
};

