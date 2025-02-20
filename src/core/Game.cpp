#include "glad/glad.h"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <memory>
#include <math.h>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <ctime>

#include "Game.hpp"
#include "Asset.hpp"
#include "../entity/Building.hpp"
#include "../entity/Ground.hpp"
#include "../entity/Entity.hpp"
#include "../debug/Debug.hpp"

bool Game::isLost = false;
bool Game::isWon = false;

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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsLight();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("../assets/fonts/PixelOperator.ttf", 24.0f);
    io.Fonts->AddFontFromFileTTF("../assets/fonts/PixelOperator.ttf", 36.0f);
    ImGui_ImplOpenGL3_CreateFontsTexture();
}

void Game::load(){
    Asset::load();

    new Ground();
}

void Game::gameLoop(){
    load();

    float lastFrameTime = glfwGetTime();
    while(!glfwWindowShouldClose(window))
    {
        float currentFrameTime = glfwGetTime();
        float deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        processInput(window);
        update(deltaTime);
        render();
        ui();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }
}

void Game::panCamera(float height, float deltaTime){
    float dy = std::fabs(cameraSpeed * deltaTime); 
    if(dy > std::fabs(cameraPosition.y - height)){
        cameraPosition.y = height;
    }
    else{
        if (cameraPosition.y - height > 0.5f){
            cameraPosition.y = cameraPosition.y - cameraSpeed * deltaTime; 
        }
        else if (cameraPosition.y - height < -0.5f){
            cameraPosition.y = cameraPosition.y + cameraSpeed * deltaTime; 
        }
    }
}

void Game::update(float deltaTime){
    keyPressCooldown -= deltaTime;
    if(isPaused){
        return;
    }
    cooldown -= deltaTime;
    eventCooldown -= deltaTime;
    moneyCooldown -= deltaTime;
    orderCooldown -= deltaTime;
    gameTime += deltaTime;


    // From left because update could delete itselve
    for (int i = Entity::entities.size() - 1; i >= 0; i--) {
        Entity::entities[i]->update(deltaTime);
    }

    if(isLost){
        cameraSpeed = 50.0f;
        float neededHeight = 4.0f;
        panCamera(neededHeight, deltaTime);
        return;
    }

    if(isWon){
        // temp
        gameInit();
        return;
    }

    auto buildingBstIt = Building::buildingBst.rbegin(); 
    while (buildingBstIt != Building::buildingBst.rend()){
        Building* building = *buildingBstIt;
        if(!building->isFalling){
            float neededHeight = building->getPosition().y + 5.0f;
            panCamera(neededHeight, deltaTime);
            break;
        }
        buildingBstIt++;
    }


    if(moneyCooldown < 0.0f){
        moneyCooldown = 3.0f;
        float actual = cameraPosition.y - 4.0f;
        if(actual != 0){
            money += std::ceil(std::pow(actual, 0.7));
        }

        // temp to play
        if(orderRunning){
            std::cout << "order " << orderCooldown << " need: " << topRequired << " money: " << money << "M" << " now: " << Building::countTop() << " difficulty " << difficulty << std::endl;
        }
    }

    difficulty = std::ceil(static_cast<float>(money) / 300.0f);

    if(!orderRunning && eventCooldown < 0.0f){
        if(rand() % 2 == 0){
            // decision
            eventCooldown = randomValue(8.0f, 15.0f);
        }
        else{
            topRequired = std::round(randomValue(difficulty, difficulty * 1.5f + 2));
            orderCooldown = randomValue(topRequired * 6.0f, topRequired * 8.0f);
            orderRunning = true;
            std::cout << "order" << std::endl;
        }
        std::cout << "event" << std::endl;
    }


    if(orderRunning && orderCooldown < 0.0f){
        if(Building::countTop() == topRequired){
            eventCooldown = randomValue(8.0f, 15.0f);
            orderRunning = false;
        }
        else{
            std::cout << "lost " << orderCooldown << " need: " << topRequired << " money: " << money << "M" << " now: " << Building::countTop() << " order running: "<< orderRunning<< std::endl;
            Game::isLost = true;
        }
    }

    if(money > 2000){
        Game::isWon = true;
        std::cout << "won, game time: " << gameTime << std::endl;
    }
}

void Game::ui() {
    ImGuiIO& io = ImGui::GetIO();
    float windowWidth = io.DisplaySize.x;
    float windowHeight = io.DisplaySize.y;

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                             ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | 
                             ImGuiWindowFlags_NoScrollbar;


    if(windowWidth < 1600){
        ImGui::PushFont(io.Fonts->Fonts[0]);
    }
    else{
        ImGui::PushFont(io.Fonts->Fonts[1]);
    }

    if(isPaused){
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0.5f));

        ImGui::Begin("Pause Menu", nullptr, flags);
        ImGui::End();
        ImGui::PopStyleColor();

        // First box
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20, 20));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.15f, 0.15f, 0.15f, 0.9f));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.6f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.7f, 0.3f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.7f, 0.5f, 0.2f, 1.0f));

        float boxWidth = 300;
        float boxHeight = 100;
        float centerX = (windowWidth - boxWidth) * 0.5f;
        float centerY = (windowHeight - boxHeight) * 0.5f - 60;

        ImGui::SetNextWindowPos(ImVec2(centerX, centerY));
        ImGui::SetNextWindowSize(ImVec2(boxWidth, boxHeight));

        ImGui::Begin("StartBox", nullptr, flags);
        ImGui::SetCursorPosY(30);

        if(Game::isLost || Game::isWon){
            if(ImGui::Button("Restart", ImVec2(260, 40))){
                isPaused = false;
                newGame = true;
            }
        }
        else{
            if(ImGui::Button(newGame ? "Start Game" : "Unpause", ImVec2(260, 40))) {
                isPaused = false;
                newGame = false;
            }
        }

        ImGui::End();

        // Second button
        centerY += 120;

        ImGui::SetNextWindowPos(ImVec2(centerX, centerY));
        ImGui::SetNextWindowSize(ImVec2(boxWidth, boxHeight));

        ImGui::Begin("TrophyBox", nullptr, flags);
        ImGui::SetCursorPosY(30);


        if (ImGui::Button("Trophies", ImVec2(260, 40))) {
            // showTrophies = true;
        }

        ImGui::End();

        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(4);
    }
    else{
        // Info box
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.1f, 0.1f, 0.75f));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
        int boxHeight = static_cast<int>(static_cast<float>(windowHeight) / 5.0f);
        int margin = static_cast<int>(static_cast<float>(windowHeight) / 20.0f);

        ImGui::SetNextWindowPos(ImVec2(margin, windowHeight - boxHeight - margin));
        ImGui::SetNextWindowSize(ImVec2(windowWidth - margin * 2, boxHeight));
        ImGui::Begin("Info Box", nullptr, flags);

        if(orderRunning){
            ImGui::Text("Order Cooldown: %.1f", orderCooldown);
            ImGui::Text("Need: %d", topRequired);
        }
        ImGui::Text("Money: %dM", money);
        ImGui::Text("Now: %d | Difficulty: %d", Building::countTop(), difficulty);

        ImGui::End();
        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(2);
    }

    ImGui::PopFont();
    ImGui::PopStyleColor();
}

float Game::randomValue(float start, float end){
    return start + static_cast<float>(rand()) / (static_cast<float>((float)RAND_MAX / (end - start)));
}

void Game::gameInit(){
    cooldown = 0.3f;
    cameraSpeed = 5.0f;
    moneyCooldown = 1.0f;
    eventCooldown = 10.0f;
    orderCooldown = 0.0f;
    gameTime = 0.0f;
    orderRunning = false;
    isPaused = false;
    moneyMultiplier = 1.0f;
    firstOrder = true;
    money = 100; 

    cameraPosition.y = 4.0f;

    for (int i = Entity::entities.size() - 1; i >= 0; i--) {
        if(Entity::entities[i]->isBuilding){
            Entity::entities[i]->remove();
        }
    }

    Game::isWon = false;
    Game::isLost = false;
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

    for (Entity* const& entity : Entity::entities) {
        entity->draw(shader);
    }
}

void Game::cleanUp(){
    for (int i = Entity::entities.size() - 1; i >= 0; i--) {
        Entity::entities[i]->remove();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

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
        if(keyPressCooldown < 0 && !newGame){
            keyPressCooldown = 0.3f;
            isPaused = !isPaused;
        }
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        if(isPaused){
            return;
        }

        glm::vec3 ray_wor = mousePick(window);

        glm::vec3 ray_origin = cameraPosition;

        float fixedDepth = -5.0f;

        float t = (fixedDepth - ray_origin.z) / ray_wor.z;
        glm::vec3 click_world = ray_origin + t * ray_wor;

        if(cooldown < 0 && money > buildingCost){
            cooldown = 0.3f;
            new Building(glm::vec3(click_world.x, cameraPosition.y + 4.0f, fixedDepth));
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
