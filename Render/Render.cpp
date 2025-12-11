#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"

using namespace std;

#define numVAOs 1

GLuint renderingProgram;
GLuint vao[numVAOs];

GLuint createShaderProgram()
{
    std::string vShdrStr = smmo::shader::io::readShaderSrc("vertexShader.glsl");
    std::string fShdrStr = smmo::shader::io::readShaderSrc("fragShader.glsl");

    const char* vShdrSrc = vShdrStr.c_str();
    const char* fShdrSrc = fShdrStr.c_str();

    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vShader, 1, &vShdrSrc, NULL);
    glShaderSource(fShader, 1, &fShdrSrc, NULL);
    glCompileShader(vShader);
    glCompileShader(fShader);

    GLuint vfProgram = glCreateProgram();
    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);
    // Link program actually creates executables for vertex and fragment
    // shaders that will be executed on the GPU.
    glLinkProgram(vfProgram);

    return vfProgram;

}


void init(GLFWwindow* window)
{
   renderingProgram = createShaderProgram();
   glGenVertexArrays(numVAOs, vao);
   glBindVertexArray(vao[0]);
}

void display(GLFWwindow* window, double currentTime)
{
    // Install the shader executables on the GPU.
    glUseProgram(renderingProgram);
    glPointSize(30.0f);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    //glClearColor(1.0, 0, 0, 1.0);
    //glClear(GL_COLOR_BUFFER_BIT);

}

int main()
{
    if (!glfwInit()) { exit(EXIT_FAILURE); }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow* window = glfwCreateWindow(600, 600, "c00l gfx d00d", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
    glfwSwapInterval(1);
    
    init(window);

    while (!glfwWindowShouldClose(window))
    {
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}