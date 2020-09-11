#include "index_buffer.h"

#include "renderer.h"


index_buffer::index_buffer(const unsigned int* data, unsigned int count)
    : m_Count(count)
{
    GLCall(glGenBuffers(1, &m_RendererID));  //Gen Buffer
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));  //Bind Buffer

    //Copy positions into buffer with ptr and specifying size
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER,         //target
                        count * sizeof(GLuint),  //size
                        data,                    //data
                        GL_STATIC_DRAW));        //usage
}

index_buffer::~index_buffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void index_buffer::bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));  //Bind Buffer
}

void index_buffer::unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));  //Bind Buffer
}
