#include "piloti.hpp"
#include "PPMImageLoader.hpp"
#include <GL/freeglut.h>
#include <vector>
#include <cmath>
#include <iostream>

Piloti::Piloti(float x, float z, float height, float radius)
    : x_(x), z_(z), height_(height), radius_(radius), texID_(0) {}

Piloti::~Piloti() {
    if (texID_ != 0) glDeleteTextures(1, &texID_);
}

bool Piloti::loadTexture(const char* ppmFilename) {
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

void Piloti::draw() const {
    const int slices = 32;
    if (texID_) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texID_);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glColor3f(1,1,1);
    }
    glPushMatrix();
    glTranslatef(x_, height_ * 0.5f, z_); // 밑면이 y=0에 딱 붙도록!
    float halfH = height_ * 0.5f;
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= slices; ++i) {
        float theta = (float)i / slices * 2.0f * M_PI;
        float nx = cos(theta), nz = sin(theta);
        float u = (float)i / slices;

        glTexCoord2f(u, 0.0f); glVertex3f(nx*radius_, -halfH, nz*radius_);
        glTexCoord2f(u, 1.0f); glVertex3f(nx*radius_, +halfH, nz*radius_);
    }
    glEnd();
    glPopMatrix();

    if (texID_) {
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
    }
}

std::vector<Piloti*> CreatePilotiList() {
    std::vector<Piloti*> list;
    float start = -2.0f, gap = 1.3f;
    float height = 1.0f, radius = 0.25f;
    for (int i = 0; i < 4; ++i) {
        int j = 0;
        float px = start + gap * j;
        float pz = start + gap * i;
        Piloti* p = new Piloti(px, pz, height, radius);
        p->loadTexture("resources/Concrete.ppm");
        list.push_back(p);
    }
    return list;
}
