#include "glad/glad.h"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

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

#define BUILDING_PLACEMENT 4.0f
#define BUILDING_DEPTH -5.0f

bool Game::isLost = false;
bool Game::isWon = false;

std::vector<std::string> Game::orderInfoTexts = {
    "\"Going straight up is too predictable. Investors demand a creative expansion strategy.\"",
    "\"Our vertical strategy is exceeding projections-pause all upward expansion immediately!\"",
    "\"Shareholders demand a more diversified growth pattern. Expand sideways before proceeding.\"",
    "\"We must future-proof our architecture! No more stacking until we establish a 'scalable' base.\"",
    "\"Our structural vision must align with our corporate synergy. Horizontal expansion is now top priority.\"",
    "\"The board has identified a risk in excessive upward growth. Redirect resources to lateral development.\"",
    "\"Due to market volatility, we must stabilize before reaching new heights. Expand outward first.\"",
    "\"A recent consultant report suggests we should 'widen our horizons'-literally. No more stacking sideway!\"",
    "\"Upward growth? That's so last quarter. We're pivoting to horizontal expansion for now.\"",
    "\"We need a strong foundation before we reach the clouds. Build sideways or risk total collapse!\"",
    "\"A bold new vision: Before we build higher, we must build wider! No exceptions.\"",
    "\"We don't just grow-we innovate our growth patterns. That means lateral movement first.\"",
    "\"Studies show customers prefer buildings with 'visual harmony.' Widen the base before stacking!\"",
    "\"The data is clear: Buildings that only go up are statistically riskier. Expand sideways immediately.\"",
    "\"If we only go up, where’s the depth? Let’s build outward for a more '3D business model.'\"",
    "\"Success isn't just about height-it's about stability. And stability comes from a wide foundation.\"",
    "\"Every great empire starts with a strong base! No more stacking until we broaden our foundation.\"",
    "\"We must embrace structural synergy! Lateral growth is the key to long-term success.\"",
};

std::vector<std::string> Game::infoTexts = {
    "\"The higher we go, the richer we become. Flawless business model. No risks whatsoever.\"",
    "\"Profit scales infinitely with height. We've cracked capitalism.\"",
    "\"Remember: A taller building means a more successful company. That's just basic economics.\"",
    "\"Our business strategy? Go up. Get money. No further questions.\"",
    "\"Financial experts confirm: More floors = More profit. Science is amazing.\"",
    "\"This isn't a reckless growth bubble-it's vertical innovation.\"",
    "\"Why diversify revenue when we can just build taller? Genius.\"",
    "\"Some say this is an unsustainable financial model. Those people were fired.\"",
    "\"We have no backup plan. We only have 'up'.\"",
    "\"Every great empire was built on a strong foundation. Ours is built on maximizing floor count.\"",
	"\"No worries! If this collapses, we’ll just call it a 'strategic reset.'\"",
	"\"Experts warn we may be 'over-leveraging' our height. Sounds like fake news.\"",
	"\"Reminder: The last company that tried this is now a cautionary tale. But we're built different.\"",
	"\"Risk management? Never heard of it.\"",
	"\"They say 'what goes up must come down.' We say 'not if we keep building.'\"",
	"\"We're totally in control of this situation. Probably.\"",
	"\"If you think about it, a collapse is just another opportunity for growth.\"",
	"\"We might be in trouble. But as long as we keep stacking, everything will be fine.\"",
};

std::vector<std::string> Game::topInfoTexts = {
    "\"Uh… has anyone checked if the top actually exists?\"",
	"\"This strategy is flawless. Unless there's some kind of height limit no one told us about…\"",
	"\"Keep stacking. We'll figure out what happens when we reach the top when we get there.\"",
	"\"We're so close to victory. Now’s definitely not the time to question anything.\"",
	"\"The board of directors is nervous. They didn't think we’d actually make it this far.\"",
	"\"If we stop now, all this was for nothing. If we keep going… well, let’s just hope for the best.\"",
	"\"We've ignored physics, financial logic, and structural integrity this whole time. Why stop now?\"",
	"\"Once we reach the top, we’ll be too big to fail. That's how it works, right?\"",
	"\"Our 'build until we win' strategy has a 100% success rate. Because no one has ever gotten this far before.\"",
	"\"Reaching the top will prove all our doubters wrong. Or it'll prove them very right.\"",
};

void Game::run(){
    try{
        init();
        gameInit();
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


    // audio
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::string error = std::string("Failed to initialize SDL: ") + SDL_GetError();
        throw std::runtime_error(error.c_str());
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::string error = std::string("Failed to initialize SDL_mixer: ") + Mix_GetError();
        throw std::runtime_error(error.c_str());
    }

    bgMusic = Mix_LoadMUS("../assets/sound/BusinessVenture.wav");
    if (!bgMusic) {
        std::string error = std::string("Failed to load background music: ") + Mix_GetError();
        throw std::runtime_error(error.c_str());
    }

    soundEffect = Mix_LoadWAV("../assets/sound/SoundEffect1.wav");
    if (!soundEffect) {
        std::string error = std::string("Failed to load sound effect: ") + Mix_GetError();
        throw std::runtime_error(error.c_str());
    }

    soundEffect2 = Mix_LoadWAV("../assets/sound/SoundEffect2.wav");
    if (!soundEffect2) {
        std::string error = std::string("Failed to load sound effect: ") + Mix_GetError();
        throw std::runtime_error(error.c_str());
    }

    // Background Music
    if (Mix_PlayingMusic() == 0) {
        Mix_PlayMusic(bgMusic, -1);
    }
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

float Game::executeDecision(bool isAccepted) {
    std::vector<EffectType> effects = isAccepted ? decision.acceptEffect : decision.declineEffect;
    
    for (EffectType effect : effects) {
        switch (effect) {
            case EffectType::GAIN_MONEY:
                money += 30 * difficulty;
                break;

            case EffectType::LOSE_MONEY:
                money -= 20 * difficulty;
                break;

            case EffectType::INCREASE_MONEY_RATE:
                moneyMultiplier += 0.1f * difficulty;
                break;

            case EffectType::REDUCE_MONEY_RATE:
                moneyMultiplier -= 0.05f * difficulty;
                if (moneyMultiplier < 0.5f) moneyMultiplier = 0.5f;
                break;

            case EffectType::REMOVE_BUILDING:
                for (int i = 0; i < difficulty; i++) {
                    Building* building = *Building::buildingBst.rbegin();
                    building->remove();
                }
                break;

            case EffectType::INFLATION:
                buildingCost += 5 * difficulty;
                break;

            case EffectType::DEFLATION:
                buildingCost -= std::max(5 * (int)difficulty, 5);
                break;

            case EffectType::BAD_LUCK:
                rareEventRate -= 0.01f * difficulty;
                break;

            case EffectType::GOOD_LUCK:
                rareEventRate += 0.01f * difficulty;
                break;

            case EffectType::NO_EFFECT:
            default:
                break;
        }
    }

    return moneyMultiplier; // Returns updated money rate modifier
}


void Game::update(float deltaTime){
    keyPressCooldown -= deltaTime;
    if(isPaused){
        return;
    }
    if(decisionRunning){
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

        // If last entity already left then pause
        if(Entity::entities[Entity::entities.size() - 1]->getPosition().y < 2.0f){
            isPaused = true;
        }
        return;
    }

    if(isWon){
        isPaused = true;
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


    if(moneyCooldown <= 0.0f){
        moneyCooldown = 3.0f;
        float actual = cameraPosition.y - 4.0f;
        if(actual != 0){
            money += std::ceil(std::pow(actual, 0.7));
        }
        std::cout << cameraPosition.y / 300.0f << std::endl;
    }

    difficulty = std::ceil(static_cast<float>(money) / 100.0f);

    if(!orderRunning && eventCooldown <= 0.0f){
        if(rand() % 2 == 0){
            // Corporate decision
            int randomIndex = floor(randomValue(1, Decision::decisionMap.size() - 1));
            decision = std::next(Decision::decisionMap.begin(), randomIndex)->second;
            decisionRunning = true;
            if(cameraPosition.y / 300.0f > 0.40f){
                infoText = topInfoTexts[round(randomValue(0, topInfoTexts.size() - 1))];
            }
            else{
                infoText = infoTexts[round(randomValue(0, infoTexts.size() - 1))];
            }

            eventCooldown = randomValue(8.0f, 15.0f);
        }
        else{
            // Corporate order
            topRequired = std::max(static_cast<int>((randomValue(difficulty, difficulty * 1.5f + 2))), 2);
            orderCooldown = randomValue(topRequired * 6.0f, topRequired * 8.0f);
            orderRunning = true;
            infoText = orderInfoTexts[round(randomValue(0, orderInfoTexts.size() - 1))];

            if(firstOrder){
                decision = Decision::decisionMap[DecisionType::FIRST_ORDER];
                decisionRunning = true;
                firstOrder = false;
            }
        }
        Mix_PlayChannel(-1, soundEffect2, 0);
    }


    if(orderRunning && orderCooldown <= 0.0f){
        if(Building::countTop() == topRequired){
            if(cameraPosition.y / 300.0f > 0.40f){
                infoText = topInfoTexts[round(randomValue(0, topInfoTexts.size() - 1))];
            }
            else{
                infoText = infoTexts[round(randomValue(0, infoTexts.size() - 1))];
            }
            eventCooldown = randomValue(12.0f, 20.0f);
            orderRunning = false;
        }
        else{
            Game::isLost = true;
        }
    }

    if(cameraPosition.y / 300.0f > 0.52f){
        Game::isWon = true;
        bestTime = std::min(gameTime, bestTime);
    }
}

bool Game::maybeHappen(float probability) {
    if (probability <= 0.0f) {
        return false;
    }
    if (probability >= 1.0f) {
        return true;
    }
    
    return (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) < probability;
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

        ImGui::Begin("Pause Menu", nullptr, flags | ImGuiWindowFlags_NoInputs);
        ImGui::End();
        ImGui::PopStyleColor();

        int padding = 10;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(padding, padding));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.12f, 0.12f, 0.12f, 0.9f)); 
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));


        if(isTrophie){
            float boxWidth = 280;
            float boxHeight = ImGui::GetFontSize() + 2 * padding;
            float centerX = (windowWidth - boxWidth) * 0.5f;
            float centerY = (windowHeight - boxHeight) * 0.5f - 60;

            ImGui::SetNextWindowPos(ImVec2(centerX, centerY));
            ImGui::SetNextWindowSize(ImVec2(boxWidth, boxHeight));

            ImGui::Begin("Result", nullptr, flags);
            ImGui::Text("Best Time: %.2f sec", bestTime);
            ImGui::End();
            centerY += 120;
            boxHeight = ImGui::GetFontSize() + 4 * padding;
            ImGui::SetNextWindowPos(ImVec2(centerX, centerY));
            ImGui::SetNextWindowSize(ImVec2(boxWidth, boxHeight));
            ImGui::Begin("BackBox", nullptr, flags);
            if(ImGui::Button("Back", ImVec2(boxWidth - 2 * padding, boxHeight - 2 * padding))){
                isTrophie = false;
            }
            ImGui::End();
        }
        else{
            float boxWidth = 280;
            float boxHeight = ImGui::GetFontSize() + 2 * padding;
            float centerX = (windowWidth - boxWidth) * 0.5f;
            float centerY = (windowHeight - boxHeight) * 0.5f - 60;

            if(Game::isLost || Game::isWon){
                ImGui::SetNextWindowPos(ImVec2(centerX, centerY));
                ImGui::SetNextWindowSize(ImVec2(boxWidth, boxHeight));
                ImGui::Begin("Result", nullptr, flags);
                if (Game::isLost) {
                    ImGui::Text("You Lost! Try Again.");
                } 
                else if (Game::isWon) {
                    ImGui::Text("You Won! Time: %.2f sec", gameTime);
                }
                ImGui::End();
                centerY += 120;
            }

            boxHeight = ImGui::GetFontSize() + 4 * padding;
            ImGui::SetNextWindowPos(ImVec2(centerX, centerY));
            ImGui::SetNextWindowSize(ImVec2(boxWidth, boxHeight));
            ImGui::Begin("StartBox", nullptr, flags);

            if(Game::isLost || Game::isWon){
                if(ImGui::Button("Restart", ImVec2(boxWidth - 2 * padding, boxHeight - 2 * padding))){
                    gameInit(false);
                    isPaused = false;
                    newGame = true;
                }
            }
            else{
                if(ImGui::Button(newGame ? "Start Game" : "Unpause", ImVec2(boxWidth - 2 * padding, boxHeight - 2 * padding))) {
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

            if (ImGui::Button("Trophies", ImVec2(boxWidth - 2 * padding, boxHeight - 2 * padding))) {
                isTrophie = true;
            }
            ImGui::End();
        }

        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(4);
    }
    else{
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
        int padding = static_cast<int>(static_cast<float>(windowWidth) / 60.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(padding, padding));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.1f, 0.1f, 0.75f));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));

        if(decisionRunning){
            int boxHeight = static_cast<int>(windowHeight / 1.5f);
            int boxMargin = static_cast<int>(windowHeight / 20.0f);
            ImGui::SetNextWindowPos(ImVec2(boxMargin, boxMargin));
            ImGui::SetNextWindowSize(ImVec2(windowWidth - boxMargin * 2, boxHeight));

            ImGui::Begin("Decision Window", nullptr, flags);

            ImGui::TextWrapped("%s", decision.title.c_str());
            ImGui::Separator();
            ImGui::TextWrapped("%s", decision.description.c_str());

            ImVec2 availableSpace = ImGui::GetContentRegionAvail();
            ImGui::Dummy(ImVec2(availableSpace.x, availableSpace.y - padding - ImGui::GetFont()->FontSize - 20));

            ImGui::Spacing();
            ImGui::Separator();

            ImVec2 buttonSize = ImVec2(availableSpace.x * 0.45f, 40);
            ImGui::SetCursorPosX((availableSpace.x - buttonSize.x * 2) / 2);

            if (ImGui::Button("Accept", buttonSize)) {
                executeDecision(true);
                decisionRunning = false;
            }
            ImGui::SameLine(false);
            if (ImGui::Button("Decline", buttonSize)) {
                decisionRunning = false;
            }

            ImGui::End();
        }

        // Info box
        int boxHeight = static_cast<int>(static_cast<float>(windowHeight) / 5.0f);
        int margin = static_cast<int>(static_cast<float>(windowHeight) / 20.0f);

        ImGui::SetNextWindowPos(ImVec2(margin, windowHeight - boxHeight - margin));
        ImGui::SetNextWindowSize(ImVec2(windowWidth - margin * 2, boxHeight));
        ImGui::Begin("Info Box", nullptr, flags);

        std::string fullText;
        if(orderRunning){
            fullText = infoText + " - CEO";
        }
        else{
            fullText = infoText;
        }
        ImGui::TextWrapped("%s", fullText.c_str());

        ImVec2 availableSpace = ImGui::GetContentRegionAvail();
        ImGui::Dummy(ImVec2(availableSpace.x, availableSpace.y - padding - ImGui::GetFont()->FontSize));

        if(orderRunning){
            ImGui::TextWrapped("Money: %dM  Order deadline: %.1f    Need: %d    Now: %d", money, orderCooldown < 0 ? 0 : orderCooldown, topRequired, Building::countTop());
        }
        else{
            ImGui::TextWrapped("Money: %dM", money);
        }

        ImGui::End();
        ImGui::PopStyleVar(3);
        ImGui::PopStyleColor(2);
    }

    ImGui::PopFont();
    ImGui::PopStyleColor();
}

float Game::randomValue(float start, float end){
    return start + static_cast<float>(rand()) / (static_cast<float>((float)RAND_MAX / (end - start)));
}

void Game::gameInit(bool paused){
    cooldown = 0.3f;
    cameraSpeed = 5.0f;
    moneyCooldown = 1.0f;
    eventCooldown = 10.0f;
    orderCooldown = 0.0f;
    gameTime = 0.0f;
    orderRunning = false;
    isPaused = paused;
    moneyMultiplier = 1.0f;
    firstOrder = true;
    money = 100; 
    difficulty = 1;
    buildingCost = 10;
    infoText = infoTexts[round(randomValue(0, infoTexts.size() - 1))];

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

    Mix_FreeMusic(bgMusic);
    Mix_FreeChunk(soundEffect);
    Mix_CloseAudio();
    SDL_Quit();

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

        float fixedDepth = BUILDING_DEPTH;

        float t = (fixedDepth - ray_origin.z) / ray_wor.z;
        glm::vec3 click_world = ray_origin + t * ray_wor;

        if(cooldown <= 0 && !decisionRunning){
            if(money < static_cast<int>(buildingCost)){
                Mix_PlayChannel(-1, soundEffect, 0);
                cooldown = 0.3f;
                return;
            }
            cooldown = 0.3f;
            new Building(glm::vec3(click_world.x, cameraPosition.y + BUILDING_PLACEMENT, fixedDepth));
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
