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
        // NOTICE:: for retina display
        glViewport(0, 0, 2 * width, 2 * height); 

        Window *const instance { static_cast<Window*>(glfwGetWindowUserPointer(window)) };
        if (instance != NULL)
        {
            instance->size[0] = static_cast<GLfloat>(width);
            instance->size[1] = static_cast<GLfloat>(height);
        }
    }

    // on mouse wheeled
    static void wheel(GLFWwindow* window, double x, double y)
    {
        Window *const instance { static_cast<Window*>(glfwGetWindowUserPointer(window)) };
        if (instance != NULL)
        {
            instance->scale += static_cast<GLfloat>(y);
        }
    }

    // on keyboard handled
    static void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        Window* const instance { static_cast<Window*>(glfwGetWindowUserPointer(window)) };

        if (instance != NULL)
        {
            instance->keyStatus = action;
        }
    }

private:
    GLFWwindow *const window;    // handler
    GLfloat size[2]     {};      // contentSize
    GLfloat scale       { 0.f }; // scale from world 
    GLfloat location[2] {};      // location 
    int keyStatus       { 0 };   // keyboard status

public:
    // constructor
    Window(int width = 640, int height = 480, const char* title = "Hello!")
        : window(glfwCreateWindow(width, height, title, NULL, NULL))
        , scale(100.f)
        , location {0, 0}
        , keyStatus(GLFW_RELEASE)
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

        // register on mouse wheeled
        glfwSetScrollCallback(window, wheel);

        // register on key
        glfwSetKeyCallback(window, keyboard);

        // register this
        glfwSetWindowUserPointer(window, this);

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
        return glfwWindowShouldClose(this->window) || glfwGetKey(window, GLFW_KEY_ESCAPE);
    }

    void swapBuffers()
    {
        // swap color buffer
        glfwSwapBuffers(this->window);

        if (keyStatus == GLFW_RELEASE)
        {
            glfwWaitEvents();
        }
        else
        {
            glfwPollEvents();
        }

        // move
        constexpr float MOVE_DISTANCE { 2.0f }; 
        // check keyboard status
        if (glfwGetKey(window, GLFW_KEY_LEFT) != GLFW_RELEASE)
        {
            this->location[0] -= MOVE_DISTANCE / size[0];
        }
        else if (glfwGetKey(window, GLFW_KEY_RIGHT) != GLFW_RELEASE)
        {
            this->location[0] += MOVE_DISTANCE / size[0];
        }
    
        if (glfwGetKey(window, GLFW_KEY_DOWN) != GLFW_RELEASE)
        {
            this->location[1] -= MOVE_DISTANCE / size[1];
        }
        else if (glfwGetKey(window, GLFW_KEY_UP) != GLFW_RELEASE)
        {
            this->location[1] += MOVE_DISTANCE / size[1];
        }


        GLint MOUSE_LEFT_BUTTON { GLFW_MOUSE_BUTTON_1 };
        if (glfwGetMouseButton(window, MOUSE_LEFT_BUTTON) != GLFW_RELEASE)
        {
            double x, y;
            glfwGetCursorPos(this->window, &x, &y);

            // normalize
            location[0] = static_cast<GLfloat>(x) * 2.0f / size[0] - 1.0f;
            location[1] = 1.0f - static_cast<GLfloat>(y) * 2.0f / size[1];
        }
   }

    const GLfloat* getSize() const { return this->size; }

    const GLfloat* getLocation() const { return location; }

    GLfloat getScale() const { return scale; }
};

#endif
