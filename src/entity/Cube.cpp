#include "Cube.hpp"
#include <iostream>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>


unsigned int Cube::VAO = 0;
unsigned int Cube::VBO = 0;
unsigned int Cube::EBO = 0;
unsigned int Cube::texture = 0;
bool Cube::initialized = false;

static float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
};

static unsigned int indices[] = {
    0, 1, 2, 2, 3, 0,  
    4, 5, 6, 6, 7, 4,  
    0, 1, 5, 5, 4, 0,  
    2, 3, 7, 7, 6, 2,  
    1, 2, 6, 6, 5, 1,  
    3, 0, 4, 4, 7, 3   
};

Cube::Cube(glm::vec3 position) : Entity(position){
    init();
}

void Cube::init() {
    if (initialized) return;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    texture = loadTexture("../assets/image/building.jpg");

    initialized = true;
}


void Cube::update(float deltaTime){
    position += glm::vec3(0.0f, -speed, 0.0f) * deltaTime;
}

void Cube::draw(Shader& shader) {
    shader.use();

    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    shader.setMat4("model", model);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    shader.setInt("texture0", 0);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Cube::cleanUp() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
