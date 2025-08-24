#include "../../include/shader.h"
#include <GL/gl.h>
#include <GL/glext.h>
#include "grass.h"

float vertices[] = {
    //   x      y      z     u     v
    // Grass blade - tapered from bottom to top
    0.0f,  0.0f, 0.0f,  0.5f, 0.0f,   // 0: bottom center
   -0.03f, 0.0f, 0.0f,  0.0f, 0.0f,   // 1: bottom left
    0.03f, 0.0f, 0.0f,  1.0f, 0.0f,   // 2: bottom right
    
   -0.025f, 0.2f, 0.0f, 0.1f, 0.33f,  // 3: lower left
    0.025f, 0.2f, 0.0f, 0.9f, 0.33f,  // 4: lower right
    
   -0.02f,  0.4f, 0.0f, 0.2f, 0.66f,  // 5: middle left
    0.02f,  0.4f, 0.0f, 0.8f, 0.66f,  // 6: middle right
    
   -0.01f,  0.6f, 0.0f, 0.3f, 0.85f,  // 7: upper left
    0.01f,  0.6f, 0.0f, 0.7f, 0.85f,  // 8: upper right
    
    0.0f,   0.8f, 0.0f, 0.5f, 1.0f    // 9: tip (single point)
};

unsigned int indices[] = {
    // Bottom section (wider base)
    1, 0, 3,   // left bottom triangle
    0, 2, 4,   // right bottom triangle
    0, 3, 4,   // center connection
    
    // Lower middle section
    3, 5, 4,   // left side
    4, 5, 6,   // right side
    
    // Upper middle section  
    5, 7, 6,   // left side
    6, 7, 8,   // right side
    
    // Top section (tapering to point)
    7, 9, 8    // tip triangle
};

Grass::Grass() {
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

    // Generate Grass Field
    generateGrassField(15, 15, 0.7f);
}

Grass::~Grass() {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void Grass::Draw(Shader& shader) {
    shader.use();
    glBindVertexArray(VAO);

    glm::mat4 model = glm::mat4(1.0f);

    for(const auto& instance: grassInstance) {
        model = glm::mat4(1.0f);
        model = glm::translate(model, instance.position);
        model = glm::rotate(model, instance.rotation, glm::vec3(0.0, 1.0, 0.0));
        model = glm::scale(model, glm::vec3(3.5f * instance.scale, 2.0f * instance.scale, 1.0f));

        shader.setFloat("randomLean", instance.randomLean);
        shader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, 27, GL_UNSIGNED_INT, 0);  
    }

    glBindVertexArray(0);
}

void Grass::generateGrassField(int gridWidth, int gridHeight, float spacing) {
    grassInstance.clear();
    
    for(unsigned int i = 0; i < gridWidth; i++) {
        for(unsigned int j = 0; j < gridHeight; j++) {
            GrassInstance instance;
            
            float baseX = i * spacing - (gridWidth * spacing) * 0.5f;
            float baseZ = j * spacing - (gridHeight * spacing) * 0.5f;
            
            float offsetX = randomInRange(-0.5f, 0.5f);
            float offsetZ = randomInRange(-0.5f, 0.5f);  
            
            instance.position = glm::vec3(baseX + offsetX, 0.0f, baseZ + offsetZ);
            instance.scale = randomInRange(0.8f, 1.2f);  
            instance.rotation = randomInRange(0.0f, 6.28318f);  
            instance.randomLean = randomInRange(-1.0, 1.0);
            
            grassInstance.push_back(instance);
        }
    }
}

// utils
float Grass::randomInRange(float min, float max) {
    return min + (rand() / (float)RAND_MAX) * (max - min);
}
