#include "../vendor/glad/include/glad/glad.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

// GLOBAL VARIABLES 
const unsigned int SCR_WIDTH = 1080;
const unsigned int SCR_HEIGHT= 720;

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

    // RENDER LOOP
    while(!glfwWindowShouldClose(window)){
        // process input
        process_input(window);

        // update
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // render

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
