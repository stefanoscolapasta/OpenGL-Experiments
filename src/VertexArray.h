#pragma once
#include "VertexBuffer.h"

class VertexBufferLayout; //To avoid having to include it, which caused circular dependencies. It is called forward class definition.

class VertexArray
{
private:
	unsigned int m_RendererID;
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	void Bind() const;
	void Unbind() const;
};