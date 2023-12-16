#include "VertexArray.h"

#include "VertexBufferLayout.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &(this->m_RendererID)));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &(this->m_RendererID)));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	this->Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		//here the binding is done
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i)); //enabled vertexAttrib 0, if we had more we wanted to use, would need to activat also those
		//The first value is the index number of the attrib within the vertex, pos in our case is at index 0, if we had a 3d normal after, for the
		//normal it would be 1, and so on. Also, this indicates the index of where to bind that attribute inside the VAO. LOL :3
		//the size is an element count, not a byte size
		//if it were a 3d vector is would be 3, etc --> this refers to the attribute/s of a specific vertex, each vertex can contain multiple
		//attributes
		//If we had a color attribute, this would need to be normalized to be between 0...1, so GL_TRUE
		//The stride refers to the distance between WHOLE consecutive vertices, not attributes (so know how to jump to same next attrib in next vertex)
		//The pointer is the pointer inside the space of the vertex, so if I have a 2d pos and a 3d normal --> for 2d pos it's 0, for 3d normal it's (const void*)sizeof(float)*2
		//(const void*) [byte amount] as it expects a 'const void*', deal with it :3
		//stride specifies the byte stride from one attribute to the next, pointer is where the attrib starts within the vertex
		//This actually enables to bind the currently bound vbo to the currently bound vao, by specifying how to interpet the data
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void *)offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
	
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(this->m_RendererID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}