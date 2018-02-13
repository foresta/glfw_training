#ifndef Window__
#define Window__

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//
// Window
//
class Window
{
public:
    // resize window
    static void resize(GLFWwindow *const window, int width, int height)
    {
        glViewport(0, 0, width, height); 
    }

private:
    GLFWwindow *const window; // handler

public:
    // constructor
    Window(int width = 640, int height = 480, const char* title = "Hello!")
        : window(glfwCreateWindow(width, height, title, NULL, NULL))
    {
        if (this->window == nullptr)
        {
            std::cerr << "Can't create GLFW window." << std::endl;
            exit(1);
        }

        // current
        glfwMakeContextCurrent(this->window);

        // initialize
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK)
        {
            std::cerr << "Can't initialize GLEW" << std::endl;
            exit(1);
        }

        // wait V-Sync
        glfwSwapInterval(1);

        // register on window size changed
        glfwSetWindowSizeCallback(window, resize);

        // resize for initialization
        resize(window, width, height);
    }

    // destructor
    virtual ~Window()
    {
        glfwDestroyWindow(this->window);
    }

    int shouldClose() const
    {
        return glfwWindowShouldClose(this->window);
    }

    void swapBuffers()
    {
        // swap color buffer
        glfwSwapBuffers(this->window);

        glfwWaitEvents();
    }
};

#endif
