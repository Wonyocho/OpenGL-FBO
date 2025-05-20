#ifndef FIRSTFLOOR_HPP
#define FIRSTFLOOR_HPP

#include <GL/glew.h>

class FirstFloor {
public:
    FirstFloor(float x, float z, float width, float depth, float height);
    ~FirstFloor();

    bool loadTexture(const char* ppmFilename);
    void draw() const;

private:
    float x_, z_, width_, depth_, height_;
    GLuint texID_;
};

// 원기둥 생성 규칙(간격, 갯수 등)에 따라 1층 생성하는 팩토리 함수
FirstFloor* CreateFirstFloorFromPilotiLayout();

#endif // FIRSTFLOOR_HPP
