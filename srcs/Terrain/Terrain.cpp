#include "Terrain.hpp"
#include "PPMImageLoader.hpp"
#include <GL/freeglut.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

Terrain::Terrain(int rows, int cols, float size)
  : rows_(rows), cols_(cols), size_(size), texID_(0)
{
    // seed는 한 번만
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    heights_.resize(rows_ * cols_);
    for (int r = 0; r < rows_; ++r) {
        for (int c = 0; c < cols_; ++c) {
            float h = (std::rand() / (float)RAND_MAX - 0.5f) * 0.1f;
            heights_[r * cols_ + c] = h;
        }
    }
}

Terrain::~Terrain() {
    if (texID_ != 0) glDeleteTextures(1, &texID_);
}

bool Terrain::loadTexture(const char* ppmFilename) {
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, data.data());
    glBindTexture(GL_TEXTURE_2D, 0);
    return true;
}

void Terrain::draw() const {
    if (texID_) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texID_);
    }
    float du = 1.0f/(cols_-1);
    float dv = 1.0f/(rows_-1);
    for (int r = 0; r < rows_-1; ++r) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int c = 0; c < cols_; ++c) {
            float x = (c - cols_/2)*size_;
            float z0 = (r - rows_/2)*size_;
            float z1 = ((r+1) - rows_/2)*size_;
            float y0 = heights_[r*cols_ + c];
            float y1 = heights_[(r+1)*cols_ + c];
            float u = c*du;
            float v0 = r*dv;
            float v1 = (r+1)*dv;
            glTexCoord2f(u, v0); glVertex3f(x, y0, z0);
            glTexCoord2f(u, v1); glVertex3f(x, y1, z1);
        }
        glEnd();
    }
    if (texID_) {
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
    }
}

Terrain* CreateTerrainDefault() {
    int rows = 100, cols = 100;
    float size = 0.2f;
    Terrain* t = new Terrain(rows, cols, size);
    t->loadTexture("resources/Terrain.ppm");
    return t;
}
