#pragma once

#include "Entity.hpp"

class Building : public Entity {
public:
    Building(glm::vec3 position = glm::vec3(0.0f));
    void update(float deltaTime) override;

private:
    float speed = 4.0f;
};

