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