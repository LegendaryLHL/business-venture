#include "Building.hpp"

std::set<Building*, Building::BuildingComparator> Building::buildingBst;

Building::Building(glm::vec3 position) : Entity(position, glm::vec3(1.0f), Asset::Texture::BUILDING, Asset::Vertex::CUBE){
    entities.push_back(std::make_unique<Building>(*this));
}

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

int Building::countTop(){
    float height;
    int count = 0;

    auto buildingBstIt = Building::buildingBst.rbegin(); 

    if(buildingBstIt == Building::buildingBst.rend()){
        return 0;
    }
    else{
        Building* firstBuilding = *buildingBstIt;
        height = firstBuilding->position.y;
        count++;
        buildingBstIt++;
    }

    while (buildingBstIt != Building::buildingBst.rend()){
        Building* building = *buildingBstIt;
        if(!building->isFalling){
            if(fabs(building->position.y - height) < 0.1f){
                count++;
            }
            else{
                break;
            }
        }
        buildingBstIt++;
    }

    return count;

}
