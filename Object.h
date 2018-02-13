#ifndef Object__
#define Object__

#include <GL/glew.h>

//
// 図形データ
//
class Object
{
private:
    GLuint vao { 0 };
    GLuint vbo { 0 };

public:
    struct Vertex
    {
        GLfloat position[2];
    };

    // constructor
    Object(GLint size, GLsizei vertexCount, const Vertex* vertex)
    {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertex, GL_STATIC_DRAW);

        glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
    };

    // destructor
    virtual ~Object()
    {
        glDeleteBuffers(1, &vao);
        glDeleteBuffers(1, &vbo);
    }

private:
    Object(const Object &o);
    Object &operator=(const Object &o);

public:
    void bind() const
    {
        glBindVertexArray(vao);
    }
};

#endif
