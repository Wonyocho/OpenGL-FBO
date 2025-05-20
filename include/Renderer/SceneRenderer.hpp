#pragma once

#include "Terrain.hpp"
#include "Piloti.hpp"
#include "FirstFloor.hpp"
#include "SecondFloor.hpp"
#include "Fence.hpp"
#include "Window.hpp"
#include "SceneRenderTarget.hpp"
#include <vector>

extern bool isDepthMode;

class SceneRenderer {
public:
    SceneRenderer(int winWidth, int winHeight);
    ~SceneRenderer();

    void display();
    void reshape(int w, int h);

private:
    int winWidth_, winHeight_;
    // 씬 오브젝트들
    Terrain* terrain_;
    std::vector<Piloti*> pilotiList_;
    FirstFloor* firstFloor_;
    SecondFloor* secondFloor_;
    Fence* fenceBack_, *fenceFront_, *fenceLeft_, *fenceRight_;
    Window* windowFront_, *windowBack_, *windowRight_;

    // Render-to-Texture용
    SceneRenderTarget* sceneFBO_;
    int fboWidth_, fboHeight_;

    // 내부 씬 전체 그리기
    void drawSceneObjects();
};
