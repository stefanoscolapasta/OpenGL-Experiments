#pragma once

#include <GL/glew.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define DEBUG 1
//__ lets us know that it is a compiler, specific to msvc, wont work in clang or others
#define ASSERT(x) if(!(x)) __debugbreak(); 

#if DEBUG
#define GLCall(x) \
        GLClearError();\
        x;\
        ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else
#define GLCall(x) x
#endif
//in macros #x turns it into a string

//remember error codes in glew are written as hex, but output error is an int
void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
};
