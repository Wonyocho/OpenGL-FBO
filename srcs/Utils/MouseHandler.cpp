#include "MouseHandler.hpp"
#include <GL/freeglut.h>

namespace {
    int lastX = 0, lastY = 0;
    bool dragging = false;
    float azimuth = 0.0f;   // 수평 회전 (Y축 기준)
    float elevation = 20.0f; // 수직 각도 (X축 기준)

    void onMouse(int button, int state, int x, int y) {
        if (button == GLUT_LEFT_BUTTON) {
            if (state == GLUT_DOWN) {
                dragging = true;
                lastX = x;
                lastY = y;
            } else {
                dragging = false;
            }
        }
    }

    void onMotion(int x, int y) {
        if (dragging) {
            int dx = x - lastX;
            int dy = y - lastY;
            azimuth += dx * 0.5f;
            elevation += dy * 0.5f;
            if (elevation > 89.0f) elevation = 89.0f;
            if (elevation < -89.0f) elevation = -89.0f;
            lastX = x;
            lastY = y;
            glutPostRedisplay();
        }
    }
}

void MouseHandler::InitMouseHandler() {
    glutMouseFunc(onMouse);
    glutMotionFunc(onMotion);
}

float MouseHandler::getAzimuth() {
    return azimuth;
}
float MouseHandler::getElevation() {
    return elevation;
}
