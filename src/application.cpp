#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "renderer.h"
#include "vertex_buffer.h"
#include "index_buffer.h"

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource IterateShader(const::std::string &filepath)
{
    std::ifstream stream(filepath);

    enum class shaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    //Set the shaders to an external file
    std::string line;
    std::stringstream ss[2];
    shaderType type = shaderType::NONE;
    while (getline(stream, line))
    {
        //Parsing shader file to find the specified sections
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                //Set mode to vertex
                type = shaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                //Set mode to fragment
                type = shaderType::FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }
    return{ ss[0].str(), ss[1].str() };
}

static unsigned int compileShader(unsigned int type, const::std::string& source)
{
    unsigned int id = glCreateShader(type);

    //Make sure this string stays alive to avoid ptr to junk
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));

        std::cout << "Failed to compile " <<
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;

        std::cout << message << std::endl;

        GLCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

static unsigned int createShader(const::std::string& vertexShader, const::std::string& fragmentShader) 
{
    unsigned int program = glCreateProgram();

    //Create two shader objects
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));

    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

int main(void)
{
    GLFWwindow* window;

    //Initialize the library 
    if (!glfwInit())
        return -1;

    //Set GL profile to core:
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Create a 1080 sized window
    window = glfwCreateWindow(1440, 1080, "Rasterizing a square", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    //Make the window's context current 
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error!";
    }

    //Print the OpenGl version being utilized
    std::cout << glGetString(GL_VERSION) << std::endl;

    {
        //Buffer index
        float positions[] = {
            -0.5f, -0.5f, //1
             0.5f, -0.5f, //2
             0.5f,  0.5f, //3
            -0.5f,  0.5f, //4
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        unsigned int vao;
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));

        vertex_buffer vb(positions, 6 * 2 * sizeof(float));
                /*Replaces the following:
                //Setting up a static buffer to draw triangle
                unsigned int buffer;
                GLCall(glGenBuffers(1, &buffer));  //Gen Buffer
                GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));  //Bind Buffer

                //Copy positions into buffer with ptr and specifying size
                GLCall(glBufferData(GL_ARRAY_BUFFER,        //target
                                    6 * 2 * sizeof(float),  //size
                                    positions,              //data
                                    GL_STATIC_DRAW));       //usage
                */
        GLCall(glEnableVertexAttribArray(0)); //Enable index 0

        //This line binds the 'buffer' to 'vao'
        //Reminder, stide is the total byte size accross all attributes
        GLCall(glVertexAttribPointer(0,                  //index
                                     2,                  //size
                                     GL_FLOAT,           //type
                                     GL_FALSE,           //normalized
                                     sizeof(float) * 2,  //stride
                                     0 ));               //pointer

        index_buffer ib(indices, 6);
                /*Replaces the following:
                //index buffer object
                unsigned int ibo; 
                GLCall(glGenBuffers(1, &ibo));  //Gen Buffer
                GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));  //Bind Buffer

                //Copy positions into buffer with ptr and specifying size
                GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER,   //target
                                    6 * sizeof(unsigned int),  //size
                                    indices,                   //data
                                    GL_STATIC_DRAW));          //usage
                */
        ShaderProgramSource source = IterateShader("res/shaders/basic.shader");

        unsigned int shader = createShader(source.VertexSource, source.FragmentSource);
        GLCall(glUseProgram(shader));

        int location = glGetUniformLocation(shader, "u_Color");
        ASSERT(location != -1);

        GLCall(glUniform4f(location, 0.2f, 0.3f, 0.8f, 0.2f));

        //Unbind all objects
        GLCall(glUseProgram(0));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));  //Bind Buffer
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));  //Bind Buffer

        float r = 0.0f;
        float increment = 0.05f;
        // Loop until the user closes the window
        while (!glfwWindowShouldClose(window))
        {
            // Render here 
            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            GLCall(glUseProgram(shader)); //bind shader

            GLCall(glUniform4f(location, r, 0.3f, 0.8f, 0.2f)); //setupi uniforms

            GLCall(glBindVertexArray(vao));  //Bind vertex Buffer

            ib.bind();;  //Bind index Buffer

            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;
        
            r += increment;

            // Swap front and back buffers 
            glfwSwapBuffers(window);

            // Poll for and process events 
            glfwPollEvents();
        }

        GLCall(glDeleteProgram(shader));
    } //This scope is to terminate the instance once window is closed

    glfwTerminate();
    return 0;
}