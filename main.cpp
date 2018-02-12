#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// print compile result
//   shader: shader object name
//   str: error string
GLboolean printShaderInfoLog(GLuint shader, const char* str)
{
    // get compile result
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) std::cerr << "Compile Error in " << str << std::endl;

    // get compile log length
    GLsizei bufSize;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufSize);

    if (bufSize > 1)
    {
        // get shader compile log
        std::vector<GLchar> infoLog(bufSize);
        GLsizei length;
        glGetShaderInfoLog(shader, bufSize, &length, &infoLog[0]);
        std::cerr << &infoLog[0] << std::endl;
    }

    return static_cast<GLboolean>(status);
}

// pring program object link resutls
//   program: program object name
GLboolean printProgramInfoLog(GLuint program)
{
    // get link result
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) std::cerr << "Link Error." << std::endl;

    // get log length on link
    GLsizei bufSize;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufSize);

    if (bufSize > 1)
    {
        // get link log
        std::vector<GLchar> infoLog(bufSize);
        GLsizei length;
        glGetProgramInfoLog(program, bufSize, &length, &infoLog[0]);
        std::cerr << &infoLog[0] << std::endl;
    }

    return static_cast<GLboolean>(status);
}

// create program object
//   vsrc: string vertex shader source
//   fsrc: string fragment shader source
GLuint createProgram(const char* vsrc, const char* fsrc)
{
    // create empty program object
    const GLuint program { glCreateProgram() };
    if (vsrc != nullptr)
    {
        const GLuint vobj { glCreateShader(GL_VERTEX_SHADER) };
        glShaderSource(vobj, 1, &vsrc, NULL);
        glCompileShader(vobj);

        // attach vertex shader to program object
        if (printShaderInfoLog(vobj, "vertex shader"))
        {
            glAttachShader(program, vobj);
        }
        glDeleteShader(vobj);
   }

    if (fsrc != nullptr)
    {
        const GLuint fobj { glCreateShader(GL_FRAGMENT_SHADER) };
        glShaderSource(fobj, 1, &fsrc, NULL);
        glCompileShader(fobj);

        // attach fragment shader to program object
        if (printShaderInfoLog(fobj, "fragment shader"))
        {
            glAttachShader(program, fobj);
        }
        glDeleteShader(fobj);
   }

    glBindAttribLocation(program, 0, "position");
    glBindFragDataLocation(program, 0, "fragment");
    glLinkProgram(program);

    if (!printProgramInfoLog(program))
    {
        glDeleteProgram(program);
        return 0;
    }

    return program;
}

// read shader source
//   name: shader source filename
//   buffer: source text
bool readShaderSource(const char* name, std::vector<GLchar> &buffer)
{
    if (name == nullptr) return false;

    std::ifstream file(name, std::ios::binary);
    if (file.fail())
    {
        std::cerr << "Error: Can't open source file: " << name << std::endl;
    }

    file.seekg(0L, std::ios::end);
    GLsizei length = static_cast<GLsizei>(file.tellg());

    buffer.resize(length + 1);

    file.seekg(0L, std::ios::beg);
    file.read(buffer.data(), length);
    buffer[length] = '\0';

    if (file.fail())
    {
        std::cerr << "Error: Could not read source file: " << name << std::endl;
        file.close();
        return false;
    }

    file.close();
    return true;
}

// load source file and create program object
//   vert: vertex shader source filename
//   frag: fragment shader source filename
GLuint loadProgram(const char* vert, const char* frag)
{
    // load source file
    std::vector<GLchar> vsrc;
    const bool vstat { readShaderSource(vert, vsrc) };
    std::vector<GLchar> fsrc;
    const bool fstat { readShaderSource(frag, fsrc) };

    return vstat && fstat ? createProgram(vsrc.data(), fsrc.data()) : 0;
}

int main()
{
    // initialize GLFW
    if (glfwInit() == GL_FALSE)
    {
        std::cerr << "Can't initialize GLFW" << std::endl;

        return 1;
    }

    // register method on terminate
    atexit(glfwTerminate);

    // select OpenGL Version 3.2 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create window 
    GLFWwindow *const window { glfwCreateWindow(640, 480, "Hello!", NULL, NULL) };
    if (window == nullptr)
    {
        // create window is failured
        std::cerr << "Can't crateGLFW window." << std::endl;
        return 1;
    }

    // add window to OpenGL target
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        // failure GLEW initialize
        std::cerr << "Can't initialize GLEW" << std::endl;
        return 1;
    }

    // wait V-Sync min count
    glfwSwapInterval(1);

    // set background color
    glClearColor(1.f, 1.f, 1.f, 0.f);

    // create program object
    const GLuint program { loadProgram("point.vsh", "point.fsh") };

    while (glfwWindowShouldClose(window) == GL_FALSE)
    {
        // clear window(clear only color buffer)
        // frame buffer has color buffer, depth buffer, stencil buffer and etc...
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);

        //
        // draw
        //

        // swap color buffer
        glfwSwapBuffers(window);

        // wait event like mouse, touch..
        glfwWaitEvents(); 
    }

    return 0;
}
