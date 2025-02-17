#include "Building.hpp"

Building::Building(glm::vec3 position) : Entity(position, glm::vec3(1.0f), Asset::Texture::BUILDING, Asset::Vertex::CUBE){}

void Building::update(float deltaTime){
    position += glm::vec3(0.0f, -speed, 0.0f) * deltaTime;
}
