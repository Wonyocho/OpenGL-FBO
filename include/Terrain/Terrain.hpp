#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include <vector>
#include <GL/glew.h>

class Terrain {
public:
    Terrain(int rows, int cols, float size);
    ~Terrain();

    bool loadTexture(const char* ppmFilename);
    void draw() const;

private:
    int rows_, cols_;
    float size_;
    std::vector<float> heights_;
    GLuint texID_;
};

Terrain* CreateTerrainDefault();

#endif // TERRAIN_HPP
