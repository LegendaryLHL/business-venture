#include "Building.hpp"

std::set<Building*, Building::BuildingComparator> Building::buildingBst;

Building::Building(glm::vec3 position) : Entity(position, glm::vec3(1.0f), Asset::Texture::BUILDING, Asset::Vertex::CUBE){}

void Building::update(float deltaTime){
    if(checkCollision()){
        if (isFalling) {
            auto it = buildingBst.find(this);
            if (it != buildingBst.end()) {
                buildingBst.erase(it);
            }
            buildingBst.insert(this);
            isFalling = false;
        }
    }
    else{
        position += glm::vec3(0.0f, -speed, 0.0f) * deltaTime;
        isFalling = true;
    }
}
