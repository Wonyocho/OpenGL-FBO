#ifndef PILOTI_HPP
#define PILOTI_HPP

#include <GL/glew.h>
#include <vector>

class Piloti {
public:
    Piloti(float x, float z, float height, float radius);
    ~Piloti();

    bool loadTexture(const char* ppmFilename);
    void draw() const;

private:
    float x_, z_;
    float height_, radius_;
    GLuint texID_;
};

std::vector<Piloti*> CreatePilotiList();

#endif // PILOTI_HPP
