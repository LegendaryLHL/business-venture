#include "glad/glad.h"
#include "glm/gtc/matrix_transform.hpp"

#include <algorithm>
#include <stdexcept>

#include "Entity.hpp"

std::vector<Entity*> Entity::entities;

Entity::Entity(glm::vec3 position, glm::vec3 scale, Asset::Texture texture, Asset::Vertex vertex) : position(position), scale(scale), texture(texture), vertex(vertex){
        entities.push_back(this);
}

void Entity::draw(Shader &shader){
    shader.use();

    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    model = glm::scale(model, scale);
    shader.setMat4("model", model);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Asset::textureMap[texture]);
    shader.setInt("texture0", 0);

    Asset::VertexData data = Asset::vertexMap[vertex];
    glBindVertexArray(data.vao);
    glDrawElements(GL_TRIANGLES, data.num, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

Entity* Entity::checkCollision() {
    // only for cube for now
    for (const auto& other : entities) {
        if (other == this){
            continue;
        }

        if (
            position.x - 0.5f * scale.x < other->position.x + 0.5f * other->scale.x &&
            position.x + 0.5f * scale.x > other->position.x - 0.5f * other->scale.x &&
            position.y - 0.5f * scale.y < other->position.y + 0.5f * other->scale.y &&
            position.y + 0.5f * scale.y > other->position.y - 0.5f * other->scale.y 
        ) {
            return other;
        }
    }
    return nullptr;
}

void Entity::update(float /*deltaTime*/){}

void Entity::remove(){
    auto it = std::find_if(entities.begin(), entities.end(),
            [this](const Entity* entity) {
            return entity == this;
            });

    if (it != entities.end()) {
        delete *it;
        entities.erase(it);
    }
    else{
        throw std::runtime_error("Tried deleting entity but does not exist.");
    }
}
