#pragma once
#include <GL/glew.h>

class Window {
public:
    Window(float cx, float cy, float cz, float width, float height, float depth, bool rotate90 = false);
    ~Window();
    bool loadTexture(const char* filename);
    void draw() const;
private:
    float cx_, cy_, cz_, width_, height_, depth_;
    bool rotate90_;
    GLuint texID_;
};

Window* CreateWindowForSecondFloorFront();
Window* CreateWindowForSecondFloorBack();
Window* CreateWindowForSecondFloorRight();
