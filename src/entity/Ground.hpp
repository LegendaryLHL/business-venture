#pragma once

#include <glm/glm.hpp>

#include <iostream>

#include "Entity.hpp"
#include "../core/Asset.hpp"

class Ground : public Entity {
public:
    Ground() : Entity(glm::vec3(0.0f), glm::vec3(100.0f, 1.0f, 100.0f), Asset::Texture::GRASS, Asset::Vertex::CUBE){
        isGround = true;
        entities.push_back(std::make_unique<Ground>(*this));
    }
};
