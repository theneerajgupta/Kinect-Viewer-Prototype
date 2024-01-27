#include <GL/glew.h>            // opengl extension wrangler
#include <GLFW/glfw3.h>         // opengl framework
#include <iostream>             // basic input and output fucntions
#include <string>               // for string operations
#include <fstream>              // file stream that deal with reading files
#include <sstream>              // string stream to contain long strings that hold shaders

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError() {
    // Clear all errors
    while (glGetError() != GL_NO_ERROR) {

    }
}

static bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << function << " , " << file << " , " << line << std::endl;
        return false;
    }
    return true;
}


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
    glfwSwapInterval(1);                                                                                            // VSync On

    if (glewInit() != GLEW_OK)                                                                                      // Initializing GLEW after Context Created/Set
        std::cout << "Error!" << std::endl;


    // VERTEX DATA
    float positions[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f, 
    };

    // VERTEX BUFFERS
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW);
    
    // VERTEX ATTRIBUTES
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*) 0);

    // INDEX DATA
    unsigned int indicies[] = {
        0, 1, 2,
        2, 3, 0,
    };

    // INDEX BUFFER (Keeps track of different vertexes)
    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * 3 * sizeof(unsigned int), indicies, GL_STATIC_DRAW);

    // SHADERS                                                                                                                      
    ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");                                           // Loading Shaders
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);                                 // Creating Shaders
    glUseProgram(shader);                                                                                           // Apply Shaders

    // USING UNIFORMS TO FEED INFORMATION TO SHADER
    int location = glGetUniformLocation(shader, "u_Color");
    ASSERT(location != -1);
    glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f);

    // UNIFORM VARIABLES
    float red_channel = 0.0f;
    float increment = 0.05f;

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // DRAWING A TRIANLGE
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glUniform4f(location, red_channel, 0.3f, 0.8f, 1.0f);
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        if (red_channel > 1.0f)
            increment = -0.05f;
        else if (red_channel < 0.0f)
            increment = 0.05f;

        red_channel += increment;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}
