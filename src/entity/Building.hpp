#pragma once

#include <iostream>
#include <set>

#include "Entity.hpp"

class Building : public Entity {
public:
    Building(glm::vec3 position = glm::vec3(0.0f));
    void update(float deltaTime) override;
    void remove() override;

    bool isFalling = true;

    struct BuildingComparator {
        bool operator()(const Building* a, const Building* b) const {
            if (a->getPosition().y == b->getPosition().y) {
                return a->getPosition().x < b->getPosition().x;
            }
            return a->getPosition().y < b->getPosition().y;
        }
    };

    static int countTop();

    static std::set<Building*, BuildingComparator> buildingBst;

private:
    float speed = 4.0f;
};

