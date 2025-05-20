#ifndef FENCE_HPP
#define FENCE_HPP

#include <GL/glew.h>

class Fence {
public:
    Fence(float cx, float cy, float cz, float width, float height, float depth, bool rotate90 = false);
    ~Fence();

    bool loadTexture(const char* ppmFilename);
    void draw() const;

private:
    float cx_, cy_, cz_;
    float width_, height_, depth_;
    bool rotate90_;
    GLuint texID_;
};

Fence* CreateFenceForBuildingBack();
Fence* CreateFenceForBuildingFront();
Fence* CreateFenceForBuildingLeft();
Fence* CreateFenceForBuildingRight();


#endif // FENCE_HPP
