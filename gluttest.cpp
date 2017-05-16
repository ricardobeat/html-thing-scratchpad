#include <GL/freeglut.h>
#include <cstdio>
 
int main(int argc, char **argv) {
 
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
 
    glutCreateWindow("Blah");
 
    glutMainLoop();
 
    // if GLEW is found, the program will show a "dirty" window.
    // if not, the window will only be shown only for a split second.
 
    return 0;
}