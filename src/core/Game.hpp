#pragma once

#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include <SDL2/SDL_mixer.h>

#include <string>
#include <vector>
#include <memory>
#include <set>

#include "../entity/Entity.hpp"
#include "../entity/Building.hpp"
#include "../event/Decision.hpp"
#include "Shader.hpp"

class Game{
    public: 
        void run();

        static bool isLost;
        static bool isWon;

    private: 
        unsigned int screenWidth = 800;
        unsigned int screenHeight = 600;
        float nearPlane = 0.1f;
        float farPlane = 100.0f;
        float fov = 45.0f;

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
        void ui();
        glm::vec3 mousePick(GLFWwindow *window);
        void panCamera(float height, float deltaTime);

        static void frameBufferSizeCallback(GLFWwindow* window, int width, int height);

        float cooldown = 0.3f;
        float cameraSpeed = 5.0f;
        float moneyCooldown = 1.0f;
        float eventCooldown = 10.0f;
        float orderCooldown = 0.0f;
        float gameTime = 0.0f;
        float keyPressCooldown = 0.3f;
        float moneyMultiplier = 1.0f;
        float bestTime = 0.0f;
        float effectTimer = 0.0f;
        bool newGame = true;
        bool firstOrder = true;
        bool orderRunning = false;
        bool decisionRunning = false;
        bool firstOrderRunning = false;
        bool isPaused = true;
        bool isTrophie = false;
        std::vector<EffectType>* recentEffect;
        Decision decision = Decision::decisionMap[DecisionType::FIRST_ORDER];
        int money = 100; 
        unsigned int difficulty = 1;
        float rareEventRate = 0.005f;
        unsigned int topRequired = 1;
        unsigned int buildingCost = 10;
        std::string infoText = "";

        Mix_Music* bgMusic = nullptr;
        Mix_Chunk* soundEffect = nullptr;
        Mix_Chunk* soundEffect2 = nullptr;

        float randomValue(float start, float end);
        bool maybeHappen(float probability);
        void gameInit(bool paused = true);
        void executeDecision(bool isAcepted);

        static std::vector<std::string> infoTexts;
        static std::vector<std::string> orderInfoTexts;
        static std::vector<std::string> topInfoTexts;
};

