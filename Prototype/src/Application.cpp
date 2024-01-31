#include <GL/glew.h>            // opengl extension wrangler
#include <GLFW/glfw3.h>         // opengl framework
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

    if (glewInit() != GLEW_OK)                                                                                      // Initializing GLEW after Context Created/Set
        std::cout << "Error!" << std::endl;

    // VERTEX DATA
    float positions[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f, 
    };

    // INDEX DATA
    unsigned int indicies[] = {
        0, 1, 2,
        2, 3, 0,
    };

    VertexArray va;
    VertexBuffer vb(positions, 4 * 2 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    // INDEX BUFFER (Keeps track of different vertexes)
    IndexBuffer ib(indicies, 2 * 3);

    // SHADERS    
    Shader shader("res/shaders/Basic.shader");
    shader.Bind();
    shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

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

    while (!glfwWindowShouldClose(window)) {

        renderer.Clear();

        // DRAWING A TRIANLGE
        
        shader.Bind();
        shader.SetUniform4f("u_Color", red_channel, 0.3f, 0.8f, 1.0f);
        
        va.Bind();
        ib.Bind();
        renderer.Draw(va, ib, shader);

        if (red_channel > 1.0f)
            increment = -0.05f;
        else if (red_channel < 0.0f)
            increment = 0.05f;

        red_channel += increment;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
