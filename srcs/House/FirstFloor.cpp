#include "FirstFloor.hpp"
#include "PPMImageLoader.hpp"
#include <GL/freeglut.h>
#include <vector>
#include <iostream>

FirstFloor::FirstFloor(float x, float z, float width, float depth, float height)
    : x_(x), z_(z), width_(width), depth_(depth), height_(height), texID_(0) {}

FirstFloor::~FirstFloor() {
    if (texID_ != 0) glDeleteTextures(1, &texID_);
}

bool FirstFloor::loadTexture(const char* ppmFilename) {
    unsigned w, h;
    std::vector<unsigned char> data;
    if (!loadPPM(ppmFilename, w, h, data)) return false;

    glGenTextures(1, &texID_);
    glBindTexture(GL_TEXTURE_2D, texID_);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());
    glBindTexture(GL_TEXTURE_2D, 0);
    return true;
}

void FirstFloor::draw() const {
    if (texID_) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texID_);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glColor3f(1,1,1);
    }

    float w = width_ * 0.5f, d = depth_ * 0.5f, h = height_;
    glPushMatrix();
    glTranslatef(x_, h * 0.5f, z_);
    glBegin(GL_QUADS);
    // 정면 (+Z)
    glTexCoord2f(0,0); glVertex3f(-w, -h*0.5f, d);
    glTexCoord2f(1,0); glVertex3f( w, -h*0.5f, d);
    glTexCoord2f(1,1); glVertex3f( w,  h*0.5f, d);
    glTexCoord2f(0,1); glVertex3f(-w,  h*0.5f, d);
    // 후면 (-Z)
    glTexCoord2f(0,0); glVertex3f( w, -h*0.5f, -d);
    glTexCoord2f(1,0); glVertex3f(-w, -h*0.5f, -d);
    glTexCoord2f(1,1); glVertex3f(-w,  h*0.5f, -d);
    glTexCoord2f(0,1); glVertex3f( w,  h*0.5f, -d);
    // 오른쪽 (+X)
    glTexCoord2f(0,0); glVertex3f( w, -h*0.5f, d);
    glTexCoord2f(1,0); glVertex3f( w, -h*0.5f, -d);
    glTexCoord2f(1,1); glVertex3f( w,  h*0.5f, -d);
    glTexCoord2f(0,1); glVertex3f( w,  h*0.5f, d);
    // 왼쪽 (-X)
    glTexCoord2f(0,0); glVertex3f(-w, -h*0.5f, -d);
    glTexCoord2f(1,0); glVertex3f(-w, -h*0.5f, d);
    glTexCoord2f(1,1); glVertex3f(-w,  h*0.5f, d);
    glTexCoord2f(0,1); glVertex3f(-w,  h*0.5f, -d);
    // 윗면 (+Y)
    glTexCoord2f(0,0); glVertex3f(-w,  h*0.5f, d);
    glTexCoord2f(1,0); glVertex3f( w,  h*0.5f, d);
    glTexCoord2f(1,1); glVertex3f( w,  h*0.5f, -d);
    glTexCoord2f(0,1); glVertex3f(-w,  h*0.5f, -d);
    // 아랫면 (-Y)
    glTexCoord2f(0,0); glVertex3f(-w, -h*0.5f, -d);
    glTexCoord2f(1,0); glVertex3f( w, -h*0.5f, -d);
    glTexCoord2f(1,1); glVertex3f( w, -h*0.5f, d);
    glTexCoord2f(0,1); glVertex3f(-w, -h*0.5f, d);
    glEnd();
    glPopMatrix();

    if (texID_) {
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
    }
}

FirstFloor* CreateFirstFloorFromPilotiLayout() {
    float start = -2.0f, gap = 1.3f;
    float height = 1.0f; // ← piloti와 반드시 동일하게
    float floor_w = gap * 2;
    float floor_d = gap * 4;
    float floor_cx = start + gap * 2.0f;
    float floor_cz = start + gap * 1.5f;
    FirstFloor* f = new FirstFloor(floor_cx, floor_cz, floor_w, floor_d, height);
    f->loadTexture("resources/Concrete.ppm");
    return f;
}
