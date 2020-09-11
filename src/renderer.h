#pragma once

#include <GL/glew.h>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLCallLog(#x, __FILE__, __LINE__))


void GLClearError();

bool GLCallLog(const char* function, const char* file, int line);