#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "View.h"
#include "Projection.h"


class Renderer
{
public:
    Renderer(View& view, Projection& projection)
    : _view(view), _projection(projection)  {
        glEnable(GL_DEPTH_TEST);
    }

    void clear() const {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    };

    void setView(View& view) { _view = view; }
    View& getView() { return _view; }

    void setProjection(Projection& projection) { _projection = projection; }
    Projection& getProjection() { return _projection; }

private:
    View& _view;
    Projection& _projection;
};