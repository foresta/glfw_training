#ifndef Shape__
#define Shape__

#include <memory>

#include "Object.h"

//
// shape data
//
class Shape
{
// ---------- member variables
private:
    std::shared_ptr<const Object> object {};

protected:
    const GLsizei vertexCount { 0 }; // vertex count

public:
    // constructor
    //   size: vertex position dimension
    //   vertexCount: number of vertexes
    //   vertex: array of vertex attribute
    Shape(GLint size, GLsizei vertexCount, const Object::Vertex* vertex)
        : object(new Object(size, vertexCount, vertex))
        , vertexCount(vertexCount)
    {
    }

    void draw() const
    {
        this->object->bind();

        execute();
    }

    virtual void execute() const
    {
        glDrawArrays(GL_LINE_LOOP, 0, vertexCount);
    }
};


#endif
