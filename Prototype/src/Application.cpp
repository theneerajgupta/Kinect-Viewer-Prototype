#include <GL/glew.h>            // opengl extension wrangler
#include <GLFW/glfw3.h>         // opengl framework
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>             // basic input and output fucntions
#include <string>               // for string operations
#include <fstream>              // file stream that deal with reading files
#include <sstream>              // string stream to contain long strings that hold shaders

#include "Renderer.h"           // holds renderer + GLCall Macro
#include "VertexBuffer.h"       // Vertex Buffer Code
#include "IndexBuffer.h"        // Index Buffer Code
#include "VertexBufferLayout.h" // Vertex Attrib Layout Code
#include "VertexArray.h"        // Vertex + Attrib Layout Code
#include "Shader.h"             // Loads + Compiles Shaders


// control variables
float rotationSpeed =  0.06f;
float rotationAngleX = 0.0f;
float rotationAngleY = 0.0f;
float range = 30.0f;
float snapRange = 0.1f;
float maxRotationAnglePos = glm::radians(range);
float maxRotationAngleNeg = glm::radians(-1.0f * range);
bool rightArrowKeyPressed = false;
bool leftArrowKeyPressed = false;
bool upArrowKeyPressed = false;
bool downArrowKeyPressed = false;

void key_rollback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) rightArrowKeyPressed = true;
    else if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE) rightArrowKeyPressed = false;
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) leftArrowKeyPressed = true;
    else if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE) leftArrowKeyPressed = false;
    if (key == GLFW_KEY_UP && action == GLFW_PRESS) upArrowKeyPressed = true;
    else if (key == GLFW_KEY_UP && action == GLFW_RELEASE) upArrowKeyPressed = false;
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) downArrowKeyPressed = true;
    else if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE) downArrowKeyPressed = false;
}


int main(void) {

    GLFWwindow* window;                                                                                             // Create OpenGL Window

    if (!glfwInit())                                                                                                // Initialize GLFW
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);                                                 // Setting Window Params
    if (!window) {
        glfwTerminate();                                                                            
        return -1;
    }

    glfwMakeContextCurrent(window);                                                                                 // Setting Window as Current Context
    glfwSwapInterval(1);                                                                                            // Set VSync On
    glfwSetKeyCallback(window, key_rollback);


    if (glewInit() != GLEW_OK)                                                                                      // Initializing GLEW after Context Created/Set
        std::cout << "Error!" << std::endl;

    // Define the vertices and colors for the cube
    float positions[] = {
        // top vertexes
        -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,     // front left    // red      // 0
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,     // front right   // green    // 1
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,     // back left     // blue     // 2
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,     // back right    // yellow   // 3
        
        // bottom vertexes
        -0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f,     // front left    // magenta  // 4
         0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f,     // front right   // cyan     // 5
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,     // back left     // orange   // 6
         0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,     // back right    // black    // 7
    };

    // INDEX DATA
    unsigned int indicies[] = {
        2, 6, 3, 3, 6, 7,   // back
        0, 2, 1, 1, 2, 3,   // top
        4, 0, 5, 5, 0, 1,   // front
        6, 4, 7, 7, 4, 5,   // bottom
        3, 7, 1, 1, 7, 5,   // right
        0, 4, 2, 2, 4, 6,   // left
    };

    VertexArray va;
    VertexBuffer vb(positions, 8 * 6 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(3);
    va.AddBuffer(vb, layout);

    // INDEX BUFFER (Keeps track of different vertexes)
    IndexBuffer ib(indicies, 12 * 3);

    // SHADERS    
    Shader shader("res/shaders/Basic.shader");
    shader.Bind();
    //shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

    // Set up projection matrix
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    // Set up view matrix
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));

    // Set up model matrix and rotate cube
    glm::mat4 model = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 0.0f, 0.0f));

    // UNBIND EVERYTHING
    va.Unbind();
    shader.Unbind();
    vb.Unbind();
    ib.Unbind();

    // RENDERER
    Renderer renderer;

    // UNIFORM VARIABLES
    float red_channel = 0.0f;
    float increment = 0.05f;

    GLCall(glEnable(GL_DEPTH_TEST));


    while (!glfwWindowShouldClose(window)) {

        renderer.Clear();

        // DRAWING A TRIANLGE
        va.Bind();
        ib.Bind();
        shader.Bind();

        // Set up view matrix
        // glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));

        // Set up model matrix and rotate cube
        // glm::mat4 model = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(1.0f, 0.5f, 0.0f));

        if (!rightArrowKeyPressed && leftArrowKeyPressed) {
            rotationAngleX += rotationSpeed;
            if (rotationAngleX > maxRotationAnglePos) rotationAngleX = maxRotationAnglePos;
        }
        else if (rightArrowKeyPressed && !leftArrowKeyPressed)  {
            rotationAngleX -= rotationSpeed;
            if (rotationAngleX < maxRotationAngleNeg) rotationAngleX = maxRotationAngleNeg;
        }
        else if (!rightArrowKeyPressed && !leftArrowKeyPressed) {
            if ((rotationAngleX < snapRange) && (rotationAngleX > -1 * snapRange)) rotationAngleX = 0.0f;
            else if (rotationAngleX > 0) rotationAngleX -= rotationSpeed;
            else if (rotationAngleX < 0) rotationAngleX += rotationSpeed;
        }

        if (upArrowKeyPressed && !downArrowKeyPressed) {
            rotationAngleY += rotationSpeed;
            if (rotationAngleY > maxRotationAnglePos) rotationAngleY = maxRotationAnglePos;
        }
        else if (!upArrowKeyPressed && downArrowKeyPressed) {
            rotationAngleY -= rotationSpeed;
            if (rotationAngleY < maxRotationAngleNeg) rotationAngleY = maxRotationAngleNeg;
        }
        else if (!upArrowKeyPressed && !downArrowKeyPressed) {
            if ((rotationAngleY < snapRange) && (rotationAngleY > -1 * snapRange)) rotationAngleY = 0.0f;
            else if (rotationAngleY > 0) rotationAngleY -= rotationSpeed;
            else if (rotationAngleY < 0) rotationAngleY += rotationSpeed;
        }


        // Set up model matrix and rotate cube
        glm::mat4 model = glm::rotate(glm::rotate(glm::mat4(1.0f), rotationAngleX, glm::vec3(0.0f, 1.0f, 0.0f)), rotationAngleY, glm::vec3(1.0f, 0.0f, 0.0f));



        shader.SetUniformMVP(model, view, projection);
        renderer.Draw(va, ib, shader);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
