#include "../vendor/glad/include/glad/glad.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../include/shader.h"
#include "./camera/camera.h"
#include "../include/model/model.h"
#include "../include/model/animator.h"
#include "../include/model/animation.h"
#include "entities/grass.h"
// #include "renderer/text/TextBlitter.h"

#include <glm/trigonometric.hpp>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "../vendor/stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xPosIn, double yPosIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture (std::string filePath);

// GLOBAL VARIABLES 
// Screen
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT= 800;

// Time
float deltaTime = 0.0f;
float lastFrameTime = 0.0f;

// Camera
Camera camera(glm::vec3(0.0f, 3.0f, 3.0f));

// Mouse
bool firstMouse = true;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

// Light
glm::vec3 lightPos(0.0f, 20.5f, 0.0f);

// Buffer Usage:
// > Generate a buffer and store it's id
// > Bind a type to that buffer 
// > Pass in the data

int main(){
    if(!glfwInit()) return -1;

    // glfwWindowHint: Makes sure the user has needed version/functionality
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Create a window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "opengl", NULL, NULL);
    if(!window){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // capture mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // check: glad initialized...?
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    // VERTEX DATA
    // Triangle Vertices
    // float vertices[] = {
    //     -0.5f, -0.5f, 0.0f,
    //     0.5f, -0.5f, 0.0f,
    //     0.0f, 0.5f, 0.0f,
    // };
    
    // Rectangle Vertices
    // float vertices[] = {
    //     -0.5f, 0.5f, 0.0f,
    //     -0.5f, -0.5f, 0.0f,
    //     0.5f, -0.5f, 0.0f,
    //     0.5f, 0.5f, 0.0f
    // };
    
    // float vertices[] = {
    //     // vertex coords        // texture coords
    //     -0.5f, 0.5f, 0.0f,      0.0f, 1.0f,
    //     -0.5f, -0.5f, 0.0f,     0.0f, 0.0f,
    //     0.5f, -0.5f, 0.0f,      1.0f, 0.0f,
    //     0.5f, 0.5f, 0.0f,       1.0f, 1.0f,
    // };

    // unsigned int indices[] = {
    //     0, 1, 2,
    //     0, 2, 3
    // };

    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    float coloredCubeVertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    glm::vec3 cubePositions[] = {
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

    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    float quadVertices[] = { 
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 1.0f
    };
    // VERTICES
    // ---------------Textured Cube-----------------
    // ---------------------------------------------
    unsigned int VBO, VAO, EBO; 
    
    // generates the buffer and stores the id into VBO 
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);     
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // vertex attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    // ---------------Colored Cube------------------
    // ---------------------------------------------
    unsigned int CubeVBO, CubeVAO; 

    glGenVertexArrays(1, &CubeVAO);
    glGenBuffers(1, &CubeVBO);

    glBindVertexArray(CubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, CubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(coloredCubeVertices), coloredCubeVertices, GL_STATIC_DRAW);

    // vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // ---------------Light Cube--------------------
    // ---------------------------------------------
    unsigned int LightCubeVBO, LightCubeVAO; 

    glGenVertexArrays(1, &LightCubeVAO);
    glGenBuffers(1, &LightCubeVBO);

    glBindVertexArray(LightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, LightCubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(coloredCubeVertices), coloredCubeVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // ---------------Quad--------------------
    // ---------------------------------------------
    unsigned int quadVBO, quadVAO; 

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);

    glBindVertexArray(quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    Shader ourShader("../src/shaders/shader.vert", "../src/shaders/shader.frag");
    Shader lightingShader("../src/shaders/color.vert", "../src/shaders/color.frag");
    Shader lightCubeShader("../src/shaders/light.vert", "../src/shaders/light.frag");
    Shader modelShader("../src/shaders/model.vert", "../src/shaders/model.frag");
    Shader animShader("../src/shaders/model/animation.vert", "../src/shaders/model/animation.frag");

    Shader depthShader("../src/shaders/depth_testing_shader/depth.vert", "../src/shaders/depth_testing_shader/depth.frag");
    Shader outlineShader("../src/shaders/outline_shader/outline.vert", "../src/shaders/outline_shader/outline.frag");
    Shader alphaShader("../src/shaders/transparent/trasparent.vert", "../src/shaders/transparent/transparent.frag");

    // Grass Shader
    Shader grassShader("../src/shaders/grass/grass.vert", "../src/shaders/grass/grass.frag");

    Shader transformShader("../src/shaders/transform/transform.vert", "../src/shaders/transform/transform.frag");

    // TextBlitter
    // TextBlitter textBlitter;

    // Model
    // Model backpack("../res/models/backpack/backpack.obj"); 
    // Model backpack("../res/models/sponza/scene.gltf"); 
    Model pistol("../res/models/pistol/scene.gltf"); 
    Animation pistolAnimation("../res/models/pistol/scene.gltf", &pistol); 
    Animator animator(&pistolAnimation);

    Grass grass;

    // Textures
    unsigned int diffuseMap = loadTexture("../res/textures/container2.png");
    unsigned int specularMap = loadTexture("../res/textures/container2_specular.png");

    unsigned int grassTexture = loadTexture("../res/textures/grass.png");

    ourShader.use();
    ourShader.setInt("material.diffuse", 0);
    ourShader.setInt("material.specular", 1);
    ourShader.setInt("material.emission", 2);

    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    // glEnable(GL_STENCIL_TEST);
    // glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);  

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // RENDER LOOP
    while(!glfwWindowShouldClose(window)){
        // Delta Time
        float currentFrameTime = (float)glfwGetTime();
        deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        // render glyphs 
        // std::cout << "FPS: " << 1 / deltaTime << std::endl;
        // textBlitter.RenderText("This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
        // textBlitter.RenderText("(C) LearnOpenGL.com", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));


        // process input
        process_input(window);
        // animator.UpdateAnimation(deltaTime);

        // render
        glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // view 
        glm::mat4 view = glm::mat4(1.0);
        view = camera.GetViewMatrix();

        // projection matrix
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f);

        glm::mat4 model = glm::mat4(1.0);

        // ------------------MODEL-----------------
        // TEXTURED CUBE MODEL
        ourShader.use();

        // bind diffuseMap
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        // for(unsigned int i=0; i<10; ++i){
        //     model = glm::mat4(1.0);
        //     model = glm::translate(model, cubePositions[i]);
        //     float angle = 25.0f*i;
        //     model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        //     if(i%3 == 0) {
        //         angle = glfwGetTime() * 25.0f;
        //         model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        //     }
        //
        //     ourShader.setVec3("viewPos", camera.Position);
        //     ourShader.setMat4("view", view);
        //     ourShader.setMat4("projection", projection);
        //     ourShader.setMat4("model", model);
        //
        //     glStencilFunc(GL_ALWAYS, 1, 0xFF);
        //     glStencilMask(0xFF); 
        //
        //     glBindVertexArray(VAO);
        //     glDrawArrays(GL_TRIANGLES, 0, 36);
        //     glBindVertexArray(0);
        //
        //     // outline 
        //     // glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        //     // glStencilMask(0x00); 
        //     // glDisable(GL_DEPTH_TEST);
        //     //
        //     // outlineShader.use();
        //     // outlineShader.setMat4("view", view);
        //     // outlineShader.setMat4("projection", projection);
        //     // model = glm::scale(model, glm::vec3(1.06));
        //     // outlineShader.setMat4("model", model);
        //     // glBindVertexArray(VAO);
        //     // glDrawArrays(GL_TRIANGLES, 0, 36);
        //     // glBindVertexArray(0);
        //     //
        //     // glStencilMask(0xFF); 
        //     // glEnable(GL_DEPTH_TEST);
        // }

        // LIGHT CUBE
        // lightCubeShader.use();
        // lightCubeShader.setMat4("view", view);
        // lightCubeShader.setMat4("projection", projection);
        //  glBindVertexArray(LightCubeVAO);
        //  for (unsigned int i = 0; i < 4; i++)
        //  {
        //      model = glm::mat4(1.0f);
        //      model = glm::translate(model, pointLightPositions[i]);
        //      model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
        //      lightCubeShader.setMat4("model", model);
        //
        //      glBindVertexArray(LightCubeVAO);
        //      glDrawArrays(GL_TRIANGLES, 0, 36);
        //  }
        
        // PLANE 
        lightingShader.use();

        lightingShader.setVec3("material.ambient", glm::vec3(0.7f, 0.7f, 0.7f)); 
        lightingShader.setVec3("material.diffuse", glm::vec3(0.7f, 0.7f, 0.7f)); 
        lightingShader.setVec3("material.specular", glm::vec3(0.2f)); 
        lightingShader.setFloat("material.shininess", 32.0f);

        lightingShader.setVec3("light.position", lightPos); 
        lightingShader.setVec3("light.ambient", glm::vec3(0.4f)); 
        lightingShader.setVec3("light.diffuse", glm::vec3(1.0f)); 
        lightingShader.setVec3("light.specular", glm::vec3(1.0f)); 

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(35.0f, 0.01f, 35.0f));

        lightingShader.setMat4("view", view);
        lightingShader.setMat4("projection", projection);
        lightingShader.setVec3("viewPos", camera.Position); 
        lightingShader.setMat4("model", model);

        glBindVertexArray(CubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Render Grass
        grassShader.use();
        grassShader.setMat4("view", view);
        grassShader.setMat4("projection", projection);
        grassShader.setFloat("time", glfwGetTime());
        grassShader.setFloat("windStrength", 1.5f);
        grassShader.setVec2("windDirection", glm::vec2(1.0f, 0.3f));
        grassShader.setVec3("lightPos", lightPos);
        // grassShader.setVec3("lightPos", camera.Position);
        grass.Draw(grassShader);

        // // Render Model
        // modelShader.use();
        // modelShader.setMat4("view", view);
        // modelShader.setMat4("projection", projection);
        //
        // model = glm::mat4(1.0f);
        // model = glm::scale(model, glm::vec3(0.01));
        // modelShader.setMat4("model", model);
        //
        // model = glm::mat4(1.0f);
        // modelShader.setMat4("model", model);
        //
        // animShader.use();
        //
        // // view/projection transformations
        // projection = glm::perspective(glm::radians(camera.Zoom), 
        //     (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        // view = camera.GetViewMatrix();
        // animShader.setMat4("projection", projection);
        // animShader.setMat4("view", view);
        //
        // auto transform = animator.GetFinalBoneMatrices();
        // for (int i = 0; i < transform.size(); ++i)
        //     animShader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transform[i]);
        //
        // // render the loaded model
        // model = glm::mat4(1.0f);
        // // model = glm::translate(model, glm::vec3(0.0f, 0.1f, 0.0f)); 
        // model = glm::translate(model, glm::vec3(camera.Position.x, camera.Position.y, camera.Position.z - 0.5)); 
        // model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));	
        // model = glm::scale(model, glm::vec3(.01f, .01f, .01f));	
        // animShader.setMat4("model", model);
        // pistol.Draw(animShader);
        //
        // // backpack.Draw(modelShader);
        // pistol.Draw(modelShader);

        //---------------------
        // Transform Controls
        //---------------------
        // X
        // glm::vec3 xAxis = glm::vec3(1.0f, 0.1f, 0.1f);
        // glm::vec3 yAxis = glm::vec3(0.1f, 1.0f, 0.1f);
        // glm::vec3 zAxis = glm::vec3(0.1f, 0.1f, 1.0f);
        //
        // transformShader.use();
        // transformShader.setVec3("viewPos", camera.Position);
        // transformShader.setMat4("view", view);
        // transformShader.setMat4("projection", projection);
        // model = glm::mat4(1.0);
        // model = glm::translate(model, glm::vec3(0.0, 3.0, 0.0)); 
        // model = glm::scale(model, glm::vec3(0.3f, 0.01f, 0.01f));
        // transformShader.setMat4("model", model);
        //
        // glBindVertexArray(CubeVAO);
        // glDrawArrays(GL_TRIANGLES, 0, 36);

        //-------------------------------------------------------------------
        
        // swap front and back color buffers 
        // [back buffer: new data, front buffer: currently displayed]
        glfwSwapBuffers(window);
        // Poll & Process events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window){
    float cameraSpeed = 3.5f * deltaTime;

    // Closing Window 
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS ||
       glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    // MOVEMENTS 
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        camera.ProcessKeyboard(FORWARD, deltaTime); 
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        camera.ProcessKeyboard(BACKWARD, deltaTime); 
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        camera.ProcessKeyboard(LEFT, deltaTime); 
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        camera.ProcessKeyboard(RIGHT, deltaTime); 
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    camera.ProcessMouseScroll((float)yoffset);
}

void mouse_callback(GLFWwindow* window, double xPosIn, double yPosIn){
    float xpos = (float)xPosIn;
    float ypos = (float)yPosIn;

    if(firstMouse){
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);

    lastX = xpos;
    lastY = ypos;
}

unsigned int loadTexture (std::string filePath){
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *texture_data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

    unsigned int textureID;
    glGenTextures(1, &textureID);

    if(texture_data){
        GLenum format;
        if(nrChannels == 1) format = GL_RED;
        else if(nrChannels == 3) format = GL_RGB;
        else if(nrChannels == 4) format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, texture_data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // set texture wraping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else {
        std::cout << "Failed to load Base texture" << std::endl;
    }
    stbi_image_free(texture_data);

    return textureID;
}
