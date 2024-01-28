#pragma once
#include "Renderer.h"

class IndexBuffer {
private:
	unsigned int m_RendererID;
	unsigned int m_Count;				// Element Count
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCOunt() const { return m_Count; }
};