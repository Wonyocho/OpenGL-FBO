#include "SceneRenderTarget.hpp"
#include <iostream>

SceneRenderTarget::SceneRenderTarget(int width, int height)
    : fbo_(0), colorTex_(0), depthTex_(0), width_(width), height_(height)
{
    glGenFramebuffers(1, &fbo_);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);

    // 컬러 텍스처
    glGenTextures(1, &colorTex_);
    glBindTexture(GL_TEXTURE_2D, colorTex_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTex_, 0);

    // 깊이버퍼 텍스처
    glGenTextures(1, &depthTex_);
    glBindTexture(GL_TEXTURE_2D, depthTex_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width_, height_, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex_, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "FBO 생성 실패!\n";

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

SceneRenderTarget::~SceneRenderTarget() {
    if (colorTex_) glDeleteTextures(1, &colorTex_);
    if (depthTex_) glDeleteTextures(1, &depthTex_);
    if (fbo_) glDeleteFramebuffers(1, &fbo_);
}

void SceneRenderTarget::bind()   { glBindFramebuffer(GL_FRAMEBUFFER, fbo_); }
void SceneRenderTarget::unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

GLuint SceneRenderTarget::getTextureID() const { return colorTex_; }
GLuint SceneRenderTarget::getDepthTextureID() const { return depthTex_; }

// 일반 컬러 텍스처 출력
void SceneRenderTarget::drawTextureQuad(float aspect)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, colorTex_);
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
    glTexCoord2f(0,0); glVertex2f(-aspect, -1);
    glTexCoord2f(1,0); glVertex2f( aspect, -1);
    glTexCoord2f(1,1); glVertex2f( aspect,  1);
    glTexCoord2f(0,1); glVertex2f(-aspect,  1);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

// 깊이버퍼(흑백) 텍스처 출력
void SceneRenderTarget::drawDepthTextureQuad(float aspect)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, depthTex_);
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
    glTexCoord2f(0,0); glVertex2f(-aspect, -1);
    glTexCoord2f(1,0); glVertex2f( aspect, -1);
    glTexCoord2f(1,1); glVertex2f( aspect,  1);
    glTexCoord2f(0,1); glVertex2f(-aspect,  1);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}
