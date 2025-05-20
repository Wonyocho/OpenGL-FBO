#include "SceneRenderer.hpp"
#include "MouseHandler.hpp"
#include <GL/freeglut.h>
#include <cmath>
#include <iostream>

bool isDepthMode = false;  // <-- 정의 (초기화는 여기서만)

SceneRenderer::SceneRenderer(int winWidth, int winHeight)
    : winWidth_(winWidth), winHeight_(winHeight),
      terrain_(nullptr), firstFloor_(nullptr), secondFloor_(nullptr),
      fenceBack_(nullptr), fenceFront_(nullptr), fenceLeft_(nullptr), fenceRight_(nullptr),
      windowFront_(nullptr), windowBack_(nullptr), windowRight_(nullptr),
      sceneFBO_(nullptr)
{
    // FBO 해상도 (고정 or winWidth_, winHeight_와 별개로 써도 OK)
    fboWidth_ = 512;
    fboHeight_ = 512;
    sceneFBO_ = new SceneRenderTarget(fboWidth_, fboHeight_);

    // 오브젝트 팩토리 함수로 생성
    terrain_     = CreateTerrainDefault();
    pilotiList_ = CreatePilotiList();
    firstFloor_ = CreateFirstFloorFromPilotiLayout();
    secondFloor_= CreateSecondFloor();
    fenceBack_  = CreateFenceForBuildingBack();
    fenceFront_ = CreateFenceForBuildingFront();
    fenceLeft_  = CreateFenceForBuildingLeft();
    fenceRight_ = CreateFenceForBuildingRight();
    windowFront_ = CreateWindowForSecondFloorFront();
    windowBack_  = CreateWindowForSecondFloorBack();
    windowRight_ = CreateWindowForSecondFloorRight();
}

SceneRenderer::~SceneRenderer() {
    for (auto p : pilotiList_) delete p;
    delete terrain_;
    delete firstFloor_;
    delete secondFloor_;
    delete fenceBack_;
    delete fenceFront_;
    delete fenceLeft_;
    delete fenceRight_;
    delete windowFront_;
    delete windowBack_;
    delete windowRight_;
    delete sceneFBO_;
}

void SceneRenderer::drawSceneObjects() {
    // 카메라 셋업
    float az = MouseHandler::getAzimuth();
    float el = MouseHandler::getElevation();
    float r = 10.0f;
    float radAz = az * M_PI / 180.0f;
    float radEl = el * M_PI / 180.0f;
    float camX = r * cos(radEl) * sin(radAz);
    float camY = r * sin(radEl);
    float camZ = r * cos(radEl) * cos(radAz);

    gluLookAt(camX, camY + 5, camZ, 0, 0, 0, 0, 1, 0);

    // 좌표축 (terrain 한 쪽 귀퉁이)
    glPushMatrix();
    glTranslatef(-3.0f, 3.0f, -3.0f);
    glLineWidth(2.5f);
    glBegin(GL_LINES);
    glColor3f(1, 0, 0); glVertex3f(0, 0, 0); glVertex3f(2, 0, 0);
    glColor3f(0, 1, 0); glVertex3f(0, 0, 0); glVertex3f(0, 2, 0);
    glColor3f(0, 0, 1); glVertex3f(0, 0, 0); glVertex3f(0, 0, 2);
    glEnd();
    glLineWidth(1.0f);
    glColor3f(1, 1, 1);
    glPopMatrix();

    // 씬 오브젝트들 draw
    if (terrain_) terrain_->draw();
    for (auto p : pilotiList_) if (p) p->draw();
    if (firstFloor_)  firstFloor_->draw();
    if (secondFloor_) secondFloor_->draw();
    if (fenceBack_)   fenceBack_->draw();
    if (fenceLeft_)   fenceLeft_->draw();
    if (fenceRight_)  fenceRight_->draw();
    if (fenceFront_)  fenceFront_->draw();
    if (windowFront_) windowFront_->draw();
    if (windowBack_)  windowBack_->draw();
    if (windowRight_) windowRight_->draw();
}

void SceneRenderer::display() {
    // 1. FBO에 씬 렌더
    sceneFBO_->bind();
    glViewport(0, 0, fboWidth_, fboHeight_);
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)fboWidth_ / fboHeight_, 2.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    drawSceneObjects(); // 실제 씬 드로우

    sceneFBO_->unbind();

    // 2. 텍스처 quad로 화면 전체에 출력
    glViewport(0, 0, winWidth_, winHeight_);
    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 투영행렬 ortho로 변경 (2D quad)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = (float)winWidth_ / winHeight_;
    if (aspect >= 1.0f)
        glOrtho(-aspect, aspect, -1, 1, -1, 1);
    else
        glOrtho(-1, 1, -1.0f/aspect, 1.0f/aspect, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (isDepthMode)
        sceneFBO_->drawDepthTextureQuad(aspect); // 깊이버퍼
    else
        sceneFBO_->drawTextureQuad(aspect);      // 컬러
    glutSwapBuffers();
}

void SceneRenderer::reshape(int w, int h) {
    winWidth_ = w;
    winHeight_ = h;
    glViewport(0, 0, w, h);
}
