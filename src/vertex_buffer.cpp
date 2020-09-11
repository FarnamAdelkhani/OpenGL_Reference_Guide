#include "vertex_buffer.h"

#include "renderer.h"


vertex_buffer::vertex_buffer(const void* data, unsigned int size)
{
    GLCall(glGenBuffers(1, &m_RendererID));  //Gen Buffer
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));  //Bind Buffer

    //Copy positions into buffer with ptr and specifying size
    GLCall(glBufferData(GL_ARRAY_BUFFER,        //target
                        size,                   //size
                        data,                   //data
                        GL_STATIC_DRAW));       //usage
}

vertex_buffer::~vertex_buffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void vertex_buffer::bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));  //Bind Buffer
}

void vertex_buffer::unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));  //Bind Buffer
}
