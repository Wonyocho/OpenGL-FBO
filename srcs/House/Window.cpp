#include "Window.hpp"
#include "PPMImageLoader.hpp"
#include <GL/freeglut.h>
#include <vector>

// 생성자/소멸자/텍스처 로드
Window::Window(float cx, float cy, float cz, float w, float h, float d, bool rotate90)
    : cx_(cx), cy_(cy), cz_(cz), width_(w), height_(h), depth_(d), rotate90_(rotate90), texID_(0) {}

Window::~Window() {
    if (texID_) glDeleteTextures(1, &texID_);
}

bool Window::loadTexture(const char* filename) {
    unsigned w, h;
    std::vector<unsigned char> data;
    if (!loadPPM(filename, w, h, data)) return false;
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

void Window::draw() const {
    if (texID_) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texID_);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glColor3f(1,1,1);
    }
    float w = width_ * 0.5f, d = depth_ * 0.5f, h = height_ * 0.5f;
    glPushMatrix();
    glTranslatef(cx_, cy_, cz_);
    if (rotate90_) glRotatef(90, 0, 1, 0);
    glBegin(GL_QUADS);
    // 정면(+Z)
    glTexCoord2f(0,0); glVertex3f(-w, -h, d);
    glTexCoord2f(1,0); glVertex3f( w, -h, d);
    glTexCoord2f(1,1); glVertex3f( w,  h, d);
    glTexCoord2f(0,1); glVertex3f(-w,  h, d);
    // 후면(-Z)
    glTexCoord2f(0,0); glVertex3f( w, -h, -d);
    glTexCoord2f(1,0); glVertex3f(-w, -h, -d);
    glTexCoord2f(1,1); glVertex3f(-w,  h, -d);
    glTexCoord2f(0,1); glVertex3f( w,  h, -d);
    // 오른쪽(+X)
    glTexCoord2f(0,0); glVertex3f( w, -h, d);
    glTexCoord2f(1,0); glVertex3f( w, -h, -d);
    glTexCoord2f(1,1); glVertex3f( w,  h, -d);
    glTexCoord2f(0,1); glVertex3f( w,  h, d);
    // 왼쪽(-X)
    glTexCoord2f(0,0); glVertex3f(-w, -h, -d);
    glTexCoord2f(1,0); glVertex3f(-w, -h, d);
    glTexCoord2f(1,1); glVertex3f(-w,  h, d);
    glTexCoord2f(0,1); glVertex3f(-w,  h, -d);
    // 윗면(+Y)
    glTexCoord2f(0,0); glVertex3f(-w,  h, d);
    glTexCoord2f(1,0); glVertex3f( w,  h, d);
    glTexCoord2f(1,1); glVertex3f( w,  h, -d);
    glTexCoord2f(0,1); glVertex3f(-w,  h, -d);
    // 아랫면(-Y)
    glTexCoord2f(0,0); glVertex3f(-w, -h, -d);
    glTexCoord2f(1,0); glVertex3f( w, -h, -d);
    glTexCoord2f(1,1); glVertex3f( w, -h, d);
    glTexCoord2f(0,1); glVertex3f(-w, -h, d);
    glEnd();
    glPopMatrix();

    if (texID_) {
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
    }
}

// SecondFloor의 실제 치수와 위치 맞춰서
Window* CreateWindowForSecondFloorFront() {
    float x = -0.4f;
    float y = 1.5f; // SecondFloor 중심과 동일하게
    float z = 2.0f + 0.5f + 0.04f; // SecondFloor 앞면 + thickness/2 + epsilon
    float width = 4.2f;  // SecondFloor보다 살짝 작게
    float height = 0.8f; // SecondFloor 높이보다 살짝 작게
    float depth = 0.08f; // 얇게
    Window* w = new Window(x, y, z, width, height, depth, false);
    w->loadTexture("resources/Window.ppm");
    return w;
}

Window* CreateWindowForSecondFloorBack() {
    float x = -0.4f;
    float y = 1.5f;
    float z = -(2.1f + 0.5f + 0.04f); // Front와 대칭 (z좌표 -)
    float width = 4.2f;
    float height = 0.8f;
    float depth = 0.08f;
    Window* w = new Window(x, y, z, width, height, depth, false);
    w->loadTexture("resources/Window.ppm");
    return w;
}

Window* CreateWindowForSecondFloorRight() {
    float x = 1.35f + 0.5f + 0.04f; // SecondFloor 오른쪽 + thickness/2 + epsilon
    float y = 1.0f;
    float z = -0.05f;
    float width = 4.8f;  // SecondFloor 깊이보다 살짝 작게
    float height = 1.8f;
    float depth = 0.08f;
    Window* w = new Window(x, y, z, width, height, depth, true); // rotate 90
    w->loadTexture("resources/Window.ppm");
    return w;
}
