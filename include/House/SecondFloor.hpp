#ifndef SECONDFLOOR_HPP
#define SECONDFLOOR_HPP

#include <GL/glew.h>

class SecondFloor {
public:
    SecondFloor(float x, float z, float width, float depth, float height);
    ~SecondFloor();

    bool loadTexture(const char* ppmFilename);
    void draw() const;

private:
    float x_, z_, width_, depth_, height_;
    GLuint texID_;
};

SecondFloor* CreateSecondFloor();

#endif // SECONDFLOOR_HPP
