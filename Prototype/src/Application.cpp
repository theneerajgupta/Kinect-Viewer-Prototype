#include <GL/glew.h>            // opengl extension wrangler
#include <GLFW/glfw3.h>         // opengl framework
#include <iostream>             // basic input and output fucntions
#include <string>               // for string operations
#include <fstream>              // file stream that deal with reading files
#include <sstream>              // string stream to contain long strings that hold shaders

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath) {
    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos) // Fixed the typo here
                type = ShaderType::FRAGMENT;
        }
        else {
            ss[static_cast<int>(type)] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = new char[length]; // Allocate memory dynamically
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile: " << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << " SHADER" << std::endl;
        std::cout << message << std::endl;
        delete[] message; // Deallocate memory
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void) {

    GLFWwindow* window;                                                                                             // Create OpenGL Window

    if (!glfwInit())                                                                                                // Initialize GLFW
        return -1;

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);                                                 // Setting Window Params
    if (!window) {
        glfwTerminate();                                                                            
        return -1;
    }

    glfwMakeContextCurrent(window);                                                                                 // Setting Window as Current Context

    if (glewInit() != GLEW_OK)                                                                                      // Initializing GLEW after Context Created/Set
        std::cout << "Error!" << std::endl;


    // VERTEX DATA
    float positions[6] = {
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f,
    };


    // VERTEX BUFFERS
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);
    
    // VERTEX ATTRIBUTES
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*) 0);

    // SHADERS                                                                                                                      
    ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");                                           // Loading Shaders
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);                                 // Creating Shaders
    glUseProgram(shader);                                                                                           // Apply Shaders

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // DRAWING A TRIANLGE
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}
