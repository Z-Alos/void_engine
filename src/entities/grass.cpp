#include "../../include/shader.h"
#include <GL/gl.h>
#include <GL/glext.h>
#include "grass.h"

// float vertices[] = {
//     //   x      y      z     u     v
//     // Grass blade - tapered from bottom to top
//     0.0f,  0.0f, 0.0f,  0.5f, 0.0f,   // 0: bottom center
//    -0.03f, 0.0f, 0.0f,  0.0f, 0.0f,   // 1: bottom left
//     0.03f, 0.0f, 0.0f,  1.0f, 0.0f,   // 2: bottom right
//
//    -0.025f, 0.2f, 0.0f, 0.1f, 0.33f,  // 3: lower left
//     0.025f, 0.2f, 0.0f, 0.9f, 0.33f,  // 4: lower right
//
//    -0.02f,  0.4f, 0.0f, 0.2f, 0.66f,  // 5: middle left
//     0.02f,  0.4f, 0.0f, 0.8f, 0.66f,  // 6: middle right
//
//    -0.01f,  0.6f, 0.0f, 0.3f, 0.85f,  // 7: upper left
//     0.01f,  0.6f, 0.0f, 0.7f, 0.85f,  // 8: upper right
//
//     0.0f,   0.8f, 0.0f, 0.5f, 1.0f    // 9: tip (single point)
// };

// float vertices[] = {
//     //   x      y     z      nx   ny   nz     u     v
//     0.0f,  0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.5f, 0.0f,   // 0: bottom center
//    -0.03f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 1: bottom left
//     0.03f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,   // 2: bottom right
//
//    -0.025f, 0.2f, 0.0f,  0.0f, 0.0f, 1.0f,   0.1f, 0.33f,  // 3: lower left
//     0.025f, 0.2f, 0.0f,  0.0f, 0.0f, 1.0f,   0.9f, 0.33f,  // 4: lower right
//
//    -0.02f,  0.4f, 0.0f,  0.0f, 0.0f, 1.0f,   0.2f, 0.66f,  // 5: middle left
//     0.02f,  0.4f, 0.0f,  0.0f, 0.0f, 1.0f,   0.8f, 0.66f,  // 6: middle right
//
//    -0.01f,  0.6f, 0.0f,  0.0f, 0.0f, 1.0f,   0.3f, 0.85f,  // 7: upper left
//     0.01f,  0.6f, 0.0f,  0.0f, 0.0f, 1.0f,   0.7f, 0.85f,  // 8: upper right
//
//     0.0f,   0.8f, 0.0f,  0.0f, 0.0f, 1.0f,   0.5f, 1.0f    // 9: tip (single point)
// };

float vertices[] = {
    //   x       y      z     nx   ny   nz     u     v
   -0.03f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 0: bottom left
    0.03f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,   // 1: bottom right

   -0.027f, 0.13f, 0.0f, 0.0f, 0.0f, 1.0f,   0.1f, 0.16f,  // 2: seg1 left
    0.027f, 0.13f, 0.0f, 0.0f, 0.0f, 1.0f,   0.9f, 0.16f,  // 3: seg1 right

   -0.024f, 0.26f, 0.0f, 0.0f, 0.0f, 1.0f,   0.2f, 0.32f,  // 4: seg2 left
    0.024f, 0.26f, 0.0f, 0.0f, 0.0f, 1.0f,   0.8f, 0.32f,  // 5: seg2 right

   -0.021f, 0.39f, 0.0f, 0.0f, 0.0f, 1.0f,   0.3f, 0.48f,  // 6: seg3 left
    0.021f, 0.39f, 0.0f, 0.0f, 0.0f, 1.0f,   0.7f, 0.48f,  // 7: seg3 right

   -0.017f, 0.52f, 0.0f, 0.0f, 0.0f, 1.0f,   0.35f, 0.64f, // 8: seg4 left
    0.017f, 0.52f, 0.0f, 0.0f, 0.0f, 1.0f,   0.65f, 0.64f, // 9: seg4 right

   -0.013f, 0.65f, 0.0f, 0.0f, 0.0f, 1.0f,   0.4f, 0.80f,  // 10: seg5 left
    0.013f, 0.65f, 0.0f, 0.0f, 0.0f, 1.0f,   0.6f, 0.80f,  // 11: seg5 right

   -0.007f, 0.74f, 0.0f, 0.0f, 0.0f, 1.0f,   0.45f, 0.90f, // 12: seg6 left
    0.007f, 0.74f, 0.0f, 0.0f, 0.0f, 1.0f,   0.55f, 0.90f, // 13: seg6 right

    0.0f,   0.8f, 0.0f,  0.0f, 0.0f, 1.0f,   0.5f, 1.0f    // 14: tip
};

// unsigned int indices[] = {
//     // Bottom section (wider base)
//     1, 0, 3,   // left bottom triangle
//     0, 2, 4,   // right bottom triangle
//     0, 3, 4,   // center connection
//
//     // Lower middle section
//     3, 5, 4,   // left side
//     4, 5, 6,   // right side
//
//     // Upper middle section  
//     5, 7, 6,   // left side
//     6, 7, 8,   // right side
//
//     // Top section (tapering to point)
//     7, 9, 8    // tip triangle
// };

unsigned int indices[] = {
    // Bottom section
    0, 1, 2,
    1, 3, 2,

    // seg1 → seg2
    2, 3, 4,
    3, 5, 4,

    // seg2 → seg3
    4, 5, 6,
    5, 7, 6,

    // seg3 → seg4
    6, 7, 8,
    7, 9, 8,

    // seg4 → seg5
    8, 9, 10,
    9, 11, 10,

    // seg5 → seg6
    10, 11, 12,
    11, 13, 12,

    // seg6 → tip
    12, 13, 14
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
    
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);
    //
    // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    // glEnableVertexAttribArray(1);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    // Generate Grass Field
    // generateGrassField(15, 15, 0.7f);
    // generateGrassField(55, 55, 0.3f);
    generateGrassField(75, 75, 0.2f);
    // generateGrassField(400, 400, 0.2f);
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
        model = glm::scale(model, glm::vec3(2.0f * instance.scale, 2.0f * instance.scale, 1.0f));

        shader.setFloat("randomLean", instance.randomLean);
        shader.setMat4("model", model);
        // glDrawElements(GL_TRIANGLES, 27, GL_UNSIGNED_INT, 0);  
        glDrawElements(GL_TRIANGLES, 39, GL_UNSIGNED_INT, 0);  
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
            instance.scale = randomInRange(0.6f, 1.2f);  
            instance.rotation = randomInRange(0.0f, 6.28318f);  
            // instance.rotation = randomInRange(0.0f, 1.28318f);  
            instance.randomLean = randomInRange(0.0, 1.0);
            
            grassInstance.push_back(instance);
        }
    }
}

// utils
float Grass::randomInRange(float min, float max) {
    return min + (rand() / (float)RAND_MAX) * (max - min);
}
