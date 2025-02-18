#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <memory>

#include "../core/Shader.hpp"
#include "../core/Asset.hpp"

class Entity {
public:
    Entity(glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f), Asset::Texture texture = Asset::Texture::BUILDING, Asset::Vertex = Asset::Vertex::CUBE);
    virtual ~Entity() = default;

    static std::vector<std::unique_ptr<Entity>> entities;

    virtual void draw(Shader &shader);
    virtual void update(float deltaTime) = 0;

    const glm::vec3& getPosition() const { return position; }

    bool checkCollision();
    bool onGround();

protected:
    glm::vec3 position;
    glm::vec3 scale;

    Asset::Texture texture;
    Asset::Vertex vertex;
};
