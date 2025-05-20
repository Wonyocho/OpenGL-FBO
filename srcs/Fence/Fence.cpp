#include "Fence.hpp"
#include "PPMImageLoader.hpp"
#include <GL/freeglut.h>
#include <vector>

// Fence 클래스 구현
Fence::Fence(float cx, float cy, float cz, float width, float height, float depth, bool rotate90)
    : cx_(cx), cy_(cy), cz_(cz), width_(width), height_(height), depth_(depth), rotate90_(rotate90), texID_(0) {}

Fence::~Fence() {
    if (texID_ != 0) glDeleteTextures(1, &texID_);
}

bool Fence::loadTexture(const char* ppmFilename) {
    unsigned w, h;
    std::vector<unsigned char> data;
    if (!loadPPM(ppmFilename, w, h, data)) return false;

    glGenTextures(1, &texID_);
    glBindTexture(GL_TEXTURE_2D, texID_);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());
    glBindTexture(GL_TEXTURE_2D, 0);
    return true;
}

void Fence::draw() const {
    if (texID_) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texID_);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glColor3f(1,1,1);
    }

    float w = width_ * 0.5f, d = depth_ * 0.5f, h = height_ * 0.5f;

    // 타일 반복수
    float repeatX = width_ / 1.0f; // 1.0f == 1m에 1타일, 조절 가능
    float repeatY = 1.0f;

    glPushMatrix();
    glTranslatef(cx_, cy_, cz_);
    if (rotate90_)
        glRotatef(90, 0, 1, 0);

    glBegin(GL_QUADS);
    // 정면(+Z)
    glTexCoord2f(0,0);             glVertex3f(-w, -h, d);
    glTexCoord2f(repeatX,0);       glVertex3f( w, -h, d);
    glTexCoord2f(repeatX,repeatY); glVertex3f( w,  h, d);
    glTexCoord2f(0,repeatY);       glVertex3f(-w,  h, d);
    // 후면(-Z)
    glTexCoord2f(0,0);             glVertex3f( w, -h, -d);
    glTexCoord2f(repeatX,0);       glVertex3f(-w, -h, -d);
    glTexCoord2f(repeatX,repeatY); glVertex3f(-w,  h, -d);
    glTexCoord2f(0,repeatY);       glVertex3f( w,  h, -d);
    // 오른쪽(+X)
    glTexCoord2f(0,0);             glVertex3f( w, -h, d);
    glTexCoord2f(repeatY,0);       glVertex3f( w, -h, -d);
    glTexCoord2f(repeatY,repeatX); glVertex3f( w,  h, -d);
    glTexCoord2f(0,repeatX);       glVertex3f( w,  h, d);
    // 왼쪽(-X)
    glTexCoord2f(0,0);             glVertex3f(-w, -h, -d);
    glTexCoord2f(repeatY,0);       glVertex3f(-w, -h, d);
    glTexCoord2f(repeatY,repeatX); glVertex3f(-w,  h, d);
    glTexCoord2f(0,repeatX);       glVertex3f(-w,  h, -d);
    // 윗면(+Y)
    glTexCoord2f(0,0);             glVertex3f(-w,  h, d);
    glTexCoord2f(repeatX,0);       glVertex3f( w,  h, d);
    glTexCoord2f(repeatX,repeatY); glVertex3f( w,  h, -d);
    glTexCoord2f(0,repeatY);       glVertex3f(-w,  h, -d);
    // 아랫면(-Y)
    glTexCoord2f(0,0);             glVertex3f(-w, -h, -d);
    glTexCoord2f(repeatX,0);       glVertex3f( w, -h, -d);
    glTexCoord2f(repeatX,repeatY); glVertex3f( w, -h, d);
    glTexCoord2f(0,repeatY);       glVertex3f(-w, -h, d);
    glEnd();
    glPopMatrix();

    if (texID_) {
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
    }
}

static const float fence_height = 0.8f;
static const float fence_depth  = 0.3f;
static const float box_w = 16.0f; // 가로(좌우)
static const float box_d = 12.0f;  // 세로(앞뒤)
static const char* fence_tex = "resources/Fence.ppm";

Fence* CreateFenceForBuildingBack() {
    float fx = 0.0f;
    float fy = fence_height * 0.5f;
    float fz = box_d * 0.5f;
    float length = box_w + fence_depth;
    Fence* fence = new Fence(fx, fy, fz, length, fence_height, fence_depth, false);
    fence->loadTexture(fence_tex);
    return fence;
}
Fence* CreateFenceForBuildingFront() {
    float fx = 0.0f;
    float fy = fence_height * 0.5f;
    float fz = -box_d * 0.5f;
    float length = box_w + fence_depth;
    Fence* fence = new Fence(fx, fy, fz, length, fence_height, fence_depth, false);
    fence->loadTexture(fence_tex);
    return fence;
}
Fence* CreateFenceForBuildingLeft() {
    float fx = -box_w * 0.5f;
    float fy = fence_height * 0.5f;
    float fz = 0.0f;
    float length = box_d + fence_depth;
    Fence* fence = new Fence(fx, fy, fz, length, fence_height, fence_depth, true);
    fence->loadTexture(fence_tex);
    return fence;
}
Fence* CreateFenceForBuildingRight() {
    float fx = +box_w * 0.5f;
    float fy = fence_height * 0.5f;
    float fz = 0.0f;
    float length = box_d + fence_depth;
    Fence* fence = new Fence(fx, fy, fz, length, fence_height, fence_depth, true);
    fence->loadTexture(fence_tex);
    return fence;
}
