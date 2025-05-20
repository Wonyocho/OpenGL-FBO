#include "SecondFloor.hpp"
#include "PPMImageLoader.hpp"
#include <GL/freeglut.h>
#include <vector>
#include <iostream>

SecondFloor::SecondFloor(float x, float z, float width, float depth, float height)
    : x_(x), z_(z), width_(width), depth_(depth), height_(height), texID_(0) {}

SecondFloor::~SecondFloor() {
    if (texID_ != 0) glDeleteTextures(1, &texID_);
}

bool SecondFloor::loadTexture(const char* ppmFilename) {
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

void SecondFloor::draw() const {
    if (texID_) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texID_);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glColor3f(1,1,1);
    }
    float w = width_ * 0.5f, d = depth_ * 0.5f, h = height_;
    glPushMatrix();
    // 2층은 1층 바로 위에 쌓이게 하려면 y=h*1.5(1층과 동일) 위치
    glTranslatef(x_, h * 1.5f, z_);
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

SecondFloor* CreateSecondFloor() {
    float start = -2.0f, gap = 1.3f;
    float height = 1.0f;         // ← Piloti/FirstFloor와 동일
    float floor_w = gap * 3.5;     // x방향 3칸 (j: 0~3)
    float floor_d = gap * 4;     // z방향 3칸 (i: 0~3) → 4칸 길이
    float floor_cx = start + gap * 1.25f; // (0~3의 중앙) = 1.5
    float floor_cz = start + gap * 1.5f;
    SecondFloor* f = new SecondFloor(floor_cx, floor_cz, floor_w, floor_d, height);
    f->loadTexture("resources/Concrete.ppm");
    return f;
}
