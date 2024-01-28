#include "VertexBuffer.h"

#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
    glGenBuffers(1, &m_RendererID);                             // create buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);                // select buffer
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);  // add data
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &m_RendererID);                          // delete buffer
}

void VertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);                // select buffer
}

void VertexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);                           // unselect buffer
}
