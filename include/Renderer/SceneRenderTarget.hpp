#pragma once
#include <GL/glew.h>

class SceneRenderTarget {
public:
    SceneRenderTarget(int width, int height);
    ~SceneRenderTarget();

    void bind();
    void unbind();
    GLuint getTextureID() const;
    GLuint getDepthTextureID() const;     // depth 텍스처 반환

    void drawTextureQuad(float aspect = 1.0f);
    void drawDepthTextureQuad(float aspect = 1.0f); // 깊이버퍼 표시용

private:
    GLuint fbo_;
    GLuint colorTex_;
    GLuint depthTex_;      // 깊이버퍼 텍스처
    int width_, height_;
};
