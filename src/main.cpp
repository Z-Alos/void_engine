#include "../vendor/glad/include/glad/glad.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include "../include/shader.h"

#include <cmath>
#include <iostream>
#include <iterator>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

// GLOBAL VARIABLES 
const unsigned int SCR_WIDTH = 1080;
const unsigned int SCR_HEIGHT= 720;

// Buffer Usage:
// > Generate a buffer and store it's id
// > Bind a type to that buffer 
// > Pass in the data

int main(){
    if(!glfwInit()) return -1;

    // glfwWindowHint: Makes sure the user has needed version/ functionality
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "opengl", NULL, NULL);
    if(!window){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // check: glad initialized...?
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader ourShader("../src/shaders/shader.vert", "../src/shaders/shader.frag");

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
    
    float vertices[] = {
        // vertex coords        // colors
        -0.5f, 0.5f, 0.0f,      1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.0f,      0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.0f,       0.2f, 0.4f, 0.7f,
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    // stores the id
    unsigned int VBO, VAO, EBO; 
    
    // generates the buffer and stores the id into VBO 
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);     
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // vertex attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // RENDER LOOP
    while(!glfwWindowShouldClose(window)){
        // process input
        process_input(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw 
        ourShader.use();

        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
    // Closing Window 
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS ||
       glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

}
