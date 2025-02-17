#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <memory>

#include "../entity/Entity.hpp"
#include "Shader.hpp"

class Game{
    public: 
        void run();

    private: 
        unsigned int screenWidth = 800;
        unsigned int screenHeight = 600;
        float nearPlane = 0.1f;
        float farPlane = 100.0f;
        float fov = 45.0f;

        std::vector<std::unique_ptr<Entity>> entities;

        GLFWwindow* window = nullptr;
        unsigned int textures[2];
        Shader shader;

        glm::mat4 view;
        glm::mat4 projection;
        glm::vec3 cameraPosition;

        void init();
        void load();
        void gameLoop();
        void update(float deltaTime);
        void render();
        void processInput(GLFWwindow *window);
        void cleanUp();
        glm::vec3 mousePick(GLFWwindow *window);

        static void frameBufferSizeCallback(GLFWwindow* window, int width, int height);

        float cooldown = 0.5f;
};

