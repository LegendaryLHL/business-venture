#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../core/Shader.hpp"

class Entity {
public:
    Entity(glm::vec3 position = glm::vec3(0.0f)) : position_(position) {}
    virtual ~Entity() = default;

    virtual void init() = 0;
    virtual void draw(Shader &shader) = 0;

    void setPosition(glm::vec3 pos) { position_ = pos; }
    glm::vec3 getPosition() const { return position_; }
    
protected:
    glm::vec3 position_;
};

