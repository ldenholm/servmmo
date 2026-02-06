/*
    This program explores model/view/perspective matrices in greater detail than my earlier work.
    A quick sensical (to me) summary of these 3 transformations and what they really mean.
    Model: we take our object (model) and place it in our world.
    View: we place our camera in our world and apply this transformation so we view
          our object according to our cameras position.
    Perspective: we interpret our object through our camera lens further by applying human eye-like perspective.
         Essentially applying depth perception and vanishing points so lines converge. This gives the 3d effect
         to our 2d render.

*/


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Shader.h"
#include "Error.h"
#include <stack>


using namespace std;

// Globals
// =======================================================

#define numVAOs 1
#define numVBOs 2

float cameraX, cameraY, cameraZ;
float cubePosX, cubePosY, cubePosZ;
float pyrPosX, pyrPosY, pyrPosZ;
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

GLuint mvLoc, pLoc, rotLoc;
int g_width, g_height;
float g_aspect, g_timeFactor;
glm::mat4 g_pMat, g_vMat, g_mdlMat, g_mvMat, g_tMat, g_rMat;
// =======================================================

void setupVertices()
{
    // 36 vertices creates 12 triangles to build a cube at origin.
    float cubePositions[108] = {
        -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f
    };

    float pyramidPositions[54] =
    { -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // front face
    1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // right face
    1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // back face
    -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // left face
    -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, // base – left front
    1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f // base – right back
    };

    
    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);
    glGenBuffers(numVBOs, vbo);
    // cube
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubePositions), cubePositions, GL_STATIC_DRAW);
    // pyramid
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidPositions), pyramidPositions, GL_STATIC_DRAW);
}

void init(GLFWwindow* window)
{
   renderingProgram = smmo::shader::createShaderProgram("vertexShader.glsl", "fragShader.glsl");
   cameraX = cameraY = 0.0f; cameraZ = 14.0f;
   cubePosX = 0.0f; cubePosY = -2.0f; cubePosZ = 0.0f; // translate down Y to show perspective.
   pyrPosX = 1.0f; pyrPosY = 2.0f; pyrPosZ = 1.0f;
   setupVertices();

   // Construct any static matrices here.
   
   // Build perspective matrix.
   glfwGetFramebufferSize(window, &g_width, &g_height);
   g_aspect = (float)g_width / (float)g_height;
   // Note the last 2 params for perspective are distance from viewer
   // to near clipping plane (hence small value), and distance to far clipping plane.
   g_pMat = glm::perspective(1.0472f, g_aspect, 0.1f, 1000.0f); // 1.0472 radians = 60 degs fov.
}

stack<glm::mat4> mvStack;

void display(GLFWwindow* window, double currentTime)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Install the shader executables on the GPU.
    glUseProgram(renderingProgram);

    // Get unfirom vars location in shader prog.
    mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
    pLoc = glGetUniformLocation(renderingProgram, "p_matrix");
    

    // Construct any dynamic (frame-dependent) matrices here.

    // Build Model, View, View-Model matrices.
    // View matrix transforms to the inverse camera position.
    g_vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
    mvStack.push(g_vMat);
    
    // Drawing the sun
    // ======================================================================================
    mvStack.push(mvStack.top()); // push a duplicate matrix.
    mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); // position of our sun.
    mvStack.push(mvStack.top()); // duplicate mv matrix of sun, we're gonna use it to build the x-rotation matrix.
    mvStack.top() *= glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(1.0f, 0.0f, 0.0f));

    glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(g_pMat));
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, 18);
    // ======================================================================================
    mvStack.pop(); // remove the sun x axis rotation

    // ======================================================================================
    // drawing the cube:
    mvStack.push(mvStack.top());
    // cube (planet) orbiting the sun (pyramid):
    mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(sin((float)currentTime) * 4.0, 0.0f, cos((float)currentTime) * 4.0));
    mvStack.push(mvStack.top());
    // cube (planet) rotating the sun (pyramid)
    mvStack.top() *= glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 1.0, 0.0));
    
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    mvStack.pop();
    // ======================================================================================
    // drawing the moon (smol cube):
    // ======================================================================================
    // Reconstruct model matrix using desired pyrmid loc.
    mvStack.push(mvStack.top());
    mvStack.top() *= glm::translate(glm::mat4(1.0f), 
        glm::vec3(0.0f, sin((float)currentTime) * 2.0, cos((float)currentTime) * 2.0)); // moon pos
    mvStack.top() *= glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 0.0, 1.0)); // moon rotation
    mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.25f, 0.25f, 0.25f)); // shrink the moon cube
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // rm moon scale/rot/pos, planet pos, sun pos, view matrices from stack
    mvStack.pop(); mvStack.pop(); mvStack.pop(); mvStack.pop();

    // ======================================================================================
      
}

void registerEventHandlers(GLFWwindow* window)
{
    glfwSetScrollCallback(window, 
        [](GLFWwindow* window, double xoffset, double yoffset)
        {
            if (yoffset > 0) { cameraZ += 1.0f; } else if (yoffset < 0) { cameraZ -= 1.0f;}
        });

    glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
        {
            glViewport(0, 0, width, height);
            g_aspect = (float)width / (float)height;
            g_pMat = glm::perspective(1.0472f, g_aspect, 0.1f, 1000.0f);
        });
}

int main()
{
    if (!glfwInit()) { exit(EXIT_FAILURE); }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow* window = glfwCreateWindow(600, 600, "cutecube", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
    glfwSwapInterval(1);
    registerEventHandlers(window);
    
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