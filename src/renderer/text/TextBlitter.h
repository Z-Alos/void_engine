#pragma once
#include "../../../vendor/glad/include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../../include/shader.h"

#include <map>
#include <iostream>

#ifndef TEXT_BLITTER_H 
#define TEXT_BLITTER_H

typedef struct FT_LibraryRec_* FT_Library;
typedef struct FT_FaceRec_* FT_Face;


struct Character {
    unsigned int textureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    unsigned int Advance;
};

class TextBlitter {
    public:
        TextBlitter();
        ~TextBlitter();
    private:
        unsigned int VAO, VBO;
        FT_Library ft;
        FT_Face face;

        int InitializeFreeType();
        void RenderText(Shader &s, std::string text, float x, float y, float scale, glm::vec3 color);

        std::map<char, Character> Characters;
};

#endif
