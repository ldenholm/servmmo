#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

#define numVAOs 1

GLuint renderingProgram;
GLuint vao[numVAOs];

GLuint createShaderProgram()
{
    const char* vShaderSrc =
        "#version 430 \n"
        "void main(void) \n"
        "{ gl_Position = vec4(0.0, 0.0, 0.0, 1.0); }";

    const char* fShaderSrc =
        "#version 430 \n"
        "out vec4 color; \n"
        "void main(void) \n"
        "{ if (gl_FragCoord.x < 295) color = vec4(1.0, 0.0, 0.0, 1.0); else color = vec4(0.0, 0.0, 1.0, 1.0); }";

    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vShader, 1, &vShaderSrc, NULL);
    glShaderSource(fShader, 1, &fShaderSrc, NULL);
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
    glDrawArrays(GL_POINTS, 0, 1);
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