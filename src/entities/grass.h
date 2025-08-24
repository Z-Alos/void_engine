#pragma once
#ifndef GRASS_H
#define GRASS_H

#include "../../vendor/glad/include/glad/glad.h"
#include "glm/glm.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../../include/shader.h"

#include <vector>

struct GrassInstance {
    glm::vec3 position;
    float rotation;
    float scale;
};

class Grass {
    public:
        Grass ();
        ~Grass ();

        void Draw(Shader& shader);
        void generateGrassField (int gridWidth, int gridHeight, float spacing = 2.0f);
    private:
        float randomInRange(float min, float max);
        unsigned int VAO, VBO, EBO;
        std::vector<GrassInstance> grassInstance;
};

#endif 

