#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>
#include <map>

#include "../core/Shader.hpp"
#include "../core/Asset.hpp"

class Entity {
public:
    Entity(glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f), Asset::Texture texture = Asset::Texture::BUILDING, Asset::Vertex = Asset::Vertex::CUBE);
    virtual ~Entity() = default;

    virtual void draw(Shader &shader);
    virtual void update(float deltaTime) = 0;

protected:
    glm::vec3 position;
    glm::vec3 scale;

    Asset::Texture texture;
    Asset::Vertex vertex;
};
