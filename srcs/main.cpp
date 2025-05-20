#include <GL/glew.h>
#include <GL/freeglut.h>
#include "SceneRenderer.hpp"
#include "MouseHandler.hpp"
#include <iostream>

SceneRenderer* renderer = nullptr;

// 디스플레이 콜백
void display() {
    if (renderer) renderer->display();
}

// 리쉐이프 콜백
void reshape(int w, int h) {
    if (renderer) renderer->reshape(w, h);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'd' || key == 'D') { // 'd'키를 누르면 토글!
        isDepthMode = !isDepthMode;
        glutPostRedisplay();
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("과제3");

    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW init error\n";
        return -1;
    }
    glEnable(GL_DEPTH_TEST);

    MouseHandler::InitMouseHandler();
    glutKeyboardFunc(keyboard);
    renderer = new SceneRenderer(800, 600);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    

    glutMainLoop();

    delete renderer;
    return 0;
}
