#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

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
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);

        std::cout << "Failed to compile " <<
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;

        std::cout << message << std::endl;

        glDeleteShader(id);
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

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void)
{
    GLFWwindow* window;

    //Initialize the library 
    if (!glfwInit())
        return -1;

    //Create a 1080 sized window
    window = glfwCreateWindow(1440, 1080, "Rasterizing a square", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    //Make the window's context current 
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error!";
    }

    //Print the OpenGl version being utilized
    std::cout << glGetString(GL_VERSION) << std::endl;

    //buffer index
    float positions[] = {
        -0.5f, -0.5f, //1
         0.5f, -0.5f, //2
         0.5f,  0.5f, //3
        -0.5f,  0.5f, //4
    };

    unsigned int indices[] = {
        0,1,2,
        2,3,0
    };

    //Setting up a static buffer to draw triangle
    unsigned int buffer;
    //Gen Buffer
    glGenBuffers(1, &buffer);
    //Bind Buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    //Copy positions into buffer with ptr and specifying size
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0); //Enable index 0
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0); //Reminder, stide is the total byte size accross all attributes

    unsigned int ibo; //index buffer object
    //Gen Buffer
    glGenBuffers(1, &ibo);
    //Bind Buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    //Copy positions into buffer with ptr and specifying size
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    ShaderProgramSource source = IterateShader("basic.shader");

    unsigned int shader = createShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);

    //glBindBuffer(GL_ARRAY_BUFFER, buffer);

    // Loop until the user closes the window j
    while (!glfwWindowShouldClose(window))
    {
        // Render here 
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        // Swap front and back buffers 
        glfwSwapBuffers(window);

        // Poll for and process events 
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}