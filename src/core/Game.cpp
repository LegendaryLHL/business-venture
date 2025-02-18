#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

#include <math.h>
#include <iostream>
#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include "Game.hpp"
#include "Asset.hpp"
#include "../entity/Building.hpp"
#include "../entity/Ground.hpp"
#include "../entity/Entity.hpp"
#include "../debug/Debug.hpp"

bool Game::isLost = false;

void Game::run(){
    try{
        init();
        gameLoop();
        cleanUp();
    }
    catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
        cleanUp();
    }
}

void Game::init(){
    srand(time(0));

    if(!glfwInit()){
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(screenWidth, screenHeight, "Business Venture", nullptr, nullptr);
    if(window == nullptr)
    {
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwSwapInterval(1);

    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        throw std::runtime_error("Failed to initialize GLAD");
    }

    shader = Shader("../shaders/shader.vert", "../shaders/shader.frag");
    
    glEnable(GL_DEPTH_TEST);

    cameraPosition = glm::vec3(0.0f, 4.0f, 10.0f);
    projection = glm::perspective(glm::radians(fov), static_cast<float>(screenWidth) / static_cast<float>(screenHeight), nearPlane, farPlane);
    shader.use();
    shader.setMat4("projection", projection);
}

void Game::load(){
    Asset::load();

    Ground ground;
}

void Game::gameLoop(){
    load();

    float lastFrameTime = glfwGetTime();
    while(!glfwWindowShouldClose(window))
    {
        float currentFrameTime = glfwGetTime();
        float deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        processInput(window);
        update(deltaTime);
        render();
    }
}

void Game::update(float deltaTime){
    cooldown -= deltaTime;
    eventCooldown -= deltaTime;
    moneyCooldown -= deltaTime;
    for (int i = 0; i < Entity::entities.size(); i++) {
        Entity::entities[i]->update(deltaTime);
    }

    auto buildingBstIt = Building::buildingBst.rbegin(); 
    while (buildingBstIt != Building::buildingBst.rend()){
        Building* building = *buildingBstIt;
        if(!building->isFalling){
            float neededHeight = building->getPosition().y + 5.0f;
            if (cameraPosition.y - neededHeight > 0.5f){
                cameraPosition.y = cameraPosition.y - cameraSpeed * deltaTime; 
            }
            else if (cameraPosition.y - neededHeight < -0.5f){
                cameraPosition.y = cameraPosition.y + cameraSpeed * deltaTime; 
            }
            break;
        }
        buildingBstIt++;
    }

    if(moneyCooldown < 0.0f){
        moneyCooldown = 3.0f;
        money += static_cast<int>(cameraPosition.y) - 4;
    }

    std::cout << eventCooldown << std::endl;

    if(eventCooldown < 0.0f){
        eventCooldown = 8.0f + static_cast<float>(rand()) / (static_cast<float>((float)RAND_MAX / (15.0f - 8.0f)));

        if(rand() % 2 == 0){
            // decision
        }
        else{
            // corperate order
        }
    }
}


void Game::render(){
    view = glm::lookAt(cameraPosition, 
            glm::vec3(0.0f, cameraPosition.y - 1.0f, 0.0f), 
            glm::vec3(0.0f, 1.0f, 0.0f));
    shader.use();
    shader.setMat4("view", view);

    float colorDiming = cameraPosition.y / 300.0f;

    glClearColor(0.52f - colorDiming, 0.8f - colorDiming, 0.92f - colorDiming, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto const& entity : Entity::entities) {
        entity->draw(shader);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();    
}

void Game::cleanUp(){
    if(window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }
    Asset::cleanUp();
    glfwTerminate();
}

void Game::frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));

    glViewport(0, 0, width, height);

    game->screenWidth = width;
    game->screenHeight = height;

    game->projection = glm::perspective(glm::radians(game->fov), static_cast<float>(width) / static_cast<float>(height), game->nearPlane, game->farPlane);
    game->shader.use();
    game->shader.setMat4("projection", game->projection);
}

void Game::processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        glm::vec3 ray_wor = mousePick(window);

        glm::vec3 ray_origin = cameraPosition;

        float fixedDepth = -5.0f;

        float t = (fixedDepth - ray_origin.z) / ray_wor.z;
        glm::vec3 click_world = ray_origin + t * ray_wor;

        if(cooldown < 0 && money > buildingCost){
            cooldown = 0.3f;
            Building building(glm::vec3(click_world.x, cameraPosition.y + 4.0f, fixedDepth));
            money -= buildingCost;
        }
    }
}

glm::vec3 Game::mousePick(GLFWwindow *window){
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    float x = (2.0f * mouseX) / screenWidth - 1.0f;
    float y = 1.0f - (2.0f * mouseY) / screenHeight;
    float z = -1.0f;

    glm::vec4 ray_clip = glm::vec4(x, y, z, 1.0f);

    glm::vec4 ray_eye = glm::inverse(projection) * ray_clip;
    ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 0.0f);

    glm::vec3 ray_wor = glm::vec3(glm::inverse(view) * ray_eye);
    return glm::normalize(ray_wor);
}
