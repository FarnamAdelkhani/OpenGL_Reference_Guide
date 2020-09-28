# OpenGL reference guide   

---

OpenGL is written by your GPU manufacturer, we interact with the driver API. It is cross-platform friendly!   

- Installing open-source OpenGL   
	- Download GLFW Windows binaries in 32-bit or 64-bit format   
	- Add the include directory and lib file to the VS project folder   

	Within the Visual Studio project:   
	- Add the include directory   
	- Add the library directory   
	- Include the lib under additional dependencies, as well as windows dependencies:   
		- glfw3.lib;opengl32.lib;user32.lib;gdi32.lib;shell32.lib   

... at this point the program will compile, but cannot build until it's fully linked.    

Windows Direct3D only goes up to OpenGL 1.1.  
... but, if we get into our GPU driver's DLL files to retrieve pointers to functions in those libraries, then we can work with newer OpenGL versions.   

We are going to use another library which provides OpenGL declarations in a header file to access the appropriate dll file for the graphics drivers we are using, then load the appropriate library functions.   

- Using GLEW - The OpenGL Extension Wrangler Library   
	- For quick use, download the Windows binaries and again set the include directory, lib directory, and add 'glew32s.lib' to additional dependencies list.   
	- Define 'GLEW_STATIC' under C++ preprocessor definitions   

Be sure to #include <GL/glew.h> before including anything else related to OpenGL   

We need a vertex buffer and shader.   

- Vertex buffer is just a buffer, ie: a stream of bytes. The difference is this buffer is stored in the GPU VRAM   

 Excellent website for OpenGL documentation: http://docs.gl/   

---

 Vertex shaders and fragment shaders are most common    
 - Vertex shaders tells OpenGL where to place the vertex positions. It runs for each vertex.    
 - Fragment shaders runs once for each pixel that needs to be rasterized. Determines the color to draw in the shape. Expensive because it runs for each pixel.    

---

Debugging  

Getting a black screen in OpenGL and not knowing what you did wrong doesn't have to be a nightmare. (maybe)  

Is the vertex data setup correctly? Maybe the shader doesn't work? Is the index buffer incorrect?  

From OpenGL 4.3 onward we can set a function ptr to be called when an error occurs.  

When arriving at an error code, convert the code to hex, then search in the glew header file for meaning of the error code.  

Options to enhance error handling for OpenGL:  

- Creating functions to call errors wrapped in an assert to pause the program at the specific line at failure.  

- Creating custom function to convert the error codes to plain English is also helpful.  

  ... we can do this with C++ macros:  

  


  `#define ASSERT(x) if (!(x)) __debugbreak();  
  #define GLCall(x) GLClearError();\  
      x;\  
      ASSERT(GLCallLog(#x, __FILE__, __LINE__))`  

  `static void GLClearError()`  
  `{`  
      `while (glGetError() != GL_NO_ERROR);`  
  `}` 
  
  `static bool GLCallLog(const char* function, const char* file, int line)`  
  `{`  
      `while (GLenum error = glGetError())`  
      `{`  
          `std::cout << "[OpenGL Error] (" << error << ")" << function <<`   
              `" " << file << ":" << line << std::endl;`  
          `return false;`  
      `}`  
      `return true;`  
  `}`  

  

  The snippet above allows us to then use: `GLCall()` to log errors to the terminal and stop the program once an error occurs.    

  The errors can be set at each GL function call within the program.  

  

