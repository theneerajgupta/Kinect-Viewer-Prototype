#pragma once


#include "Renderer.h"

class VertexBufferLayout;
class VertexBuffer;

class VertexArray {
private:
	unsigned int m_RendererID;


public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void Bind() const;
	void Unbind() const;

};