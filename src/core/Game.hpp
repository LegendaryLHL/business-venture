#pragma once

#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <memory>

#include "../entity/Entity.hpp"
#include "Shader.hpp"

const unsigned int HEIGHT = 600;
const unsigned int WIDTH = 800;

class Game{
    public: 
        void run();

    private: 
        std::vector<std::unique_ptr<Entity>> entities;

        GLFWwindow* window = nullptr;
        unsigned int textures[2];
        Shader shader;

        void init();
        void load();
        void gameLoop();
        void update();
        void render();
        void processInput(GLFWwindow *window);
        void cleanUp();

        static void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
};

