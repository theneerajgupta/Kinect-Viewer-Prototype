#include "renderer.h"
#include "IndexBuffer.h"


IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
    : m_Count(count) {
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    glGenBuffers(1, &m_RendererID);                                                             // create buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);                                        // select buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);  // supply data
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &m_RendererID);      // delete buffer
}

void IndexBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);        // Select Buffer      
}

void IndexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
