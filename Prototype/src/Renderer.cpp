﻿#include "Renderer.h"

#include <iostream>


void GLClearError() {
    // Clear all errors
    while (glGetError() != GL_NO_ERROR) {

    }
}

bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << function << " , " << file << " , " << line << std::endl;
        return false;
    }
    return true;
}