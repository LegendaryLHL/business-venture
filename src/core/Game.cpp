#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <stb/stb_image.h>

#include <math.h>
#include <iostream>
#include <stdexcept>

#include "Game.hpp"
#include "../entity/Cube.hpp"

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
    if(!glfwInit()){
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Business Venture", nullptr, nullptr);
    if(window == nullptr)
    {
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        throw std::runtime_error("Failed to initialize GLAD");
    }

    shader = Shader("../shaders/shader.vert", "../shaders/shader.frag");

    // texture
    glGenTextures(2, textures);

    glBindTexture(GL_TEXTURE_2D, textures[0]);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load("../assets/image/building.jpg", &width, &height, &nrChannels, 0);
    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        throw std::runtime_error("Failed to load texture");
    }
    stbi_image_free(data);


    shader.use();
    shader.setInt("texture1", 0);
    
    glEnable(GL_DEPTH_TEST);

    cameraPosition = glm::vec3(0.0f, 2.0f, 10.0f);
}

void Game::load(){
    view = glm::mat4(1.0f);
    view = glm::lookAt(cameraPosition, 
            glm::vec3(0.0f, -0.3f, 0.0f), 
            glm::vec3(0.0f, 1.0f, 0.0f));
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(WIDTH) / static_cast<float>(HEIGHT), 0.1f, 100.0f);
    shader.use();
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    std::vector<glm::vec3> cubePositions = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };

    for (int i = 0; i < cubePositions.size(); i++) {
        entities.push_back(std::make_unique<Cube>(cubePositions[i]));
    }
}

void Game::gameLoop(){
    load();
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);
        render();
    }
}

void Game::render(){
    glClearColor(0.52f, 0.8f, 0.92f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);

    for (auto const& entity : entities) {
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
    Cube::cleanUp();
    glfwTerminate();
}

void Game::frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Game::processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        float x = (2.0f * mouseX) / WIDTH - 1.0f;
        float y = 1.0f - (2.0f * mouseY) / HEIGHT;
        float z = 1.0f;

        glm::vec4 ray_clip = glm::vec4(x, y, -1.0f, 1.0f);

        glm::vec4 ray_eye = glm::inverse(projection) * ray_clip;
        ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 0.0f);

        glm::vec3 ray_wor = glm::vec3(glm::inverse(view) * ray_eye);
        ray_wor = glm::normalize(ray_wor);

        glm::vec3 ray_origin = cameraPosition;
        float t = -ray_origin.y / ray_wor.y;
        glm::vec3 click_world = ray_origin + t * ray_wor;

        entities.push_back(std::make_unique<Cube>(glm::vec3(click_world.x, click_world.y, -2.0f)));
    }
}
