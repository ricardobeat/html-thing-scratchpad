#include <GLFW/glfw3.h>
#include <Yoga.h>
#include <stdlib.h>
#include <cairo/cairo.h>
#include <nanovg.h>

// #define NANOVG_GL2_IMPLEMENTATION
// #include "nanovg_gl.h"

bool dirty = false;

void paint (GLFWwindow* window) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    
    /* Make it 2D */
    glMatrixMode(GL_PROJECTION); glLoadIdentity(); glOrtho(0, windowWidth, windowHeight, 0, 0, 1);
    
    YGConfigRef config = YGConfigNew();
    YGNodeRef root = YGNodeNewWithConfig(config);
    YGNodeStyleSetFlexDirection(root, YGFlexDirectionRow);
    YGNodeStyleSetPadding(root, YGEdgeAll, 20);
    YGNodeStyleSetMargin(root, YGEdgeAll, 20);
    
    YGNodeRef image = YGNodeNew();
    YGNodeStyleSetWidth(image, 80);
    YGNodeStyleSetHeight(image, 80);
    YGNodeStyleSetAlignSelf(image, YGAlignCenter);
    YGNodeStyleSetMargin(image, YGEdgeEnd, 20);
    
    YGNodeRef text = YGNodeNew();
    YGNodeStyleSetHeight(text, 25);
    YGNodeStyleSetAlignSelf(text, YGAlignCenter);
    YGNodeStyleSetFlexGrow(text, 1);
    
    YGNodeInsertChild(root, image, 0);
    YGNodeInsertChild(root, text, 1);
    
    YGNodeCalculateLayout(root, windowWidth, windowHeight, YGDirectionLTR);
    
    // /* Render here */
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    // glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    
    glColor3f(1.0, 1.0, 1.0);
    glRecti(
        YGNodeLayoutGetLeft(root),
        YGNodeLayoutGetTop(root),
        YGNodeLayoutGetLeft(root) + YGNodeLayoutGetWidth(root),
        YGNodeLayoutGetTop(root) + YGNodeLayoutGetHeight(root)
    );
    
    glColor3f(0.0, 0.0, 1.0);
    glRecti(
            YGNodeLayoutGetLeft(root) + YGNodeLayoutGetLeft(text),
            YGNodeLayoutGetTop(root) + YGNodeLayoutGetTop(text),
            YGNodeLayoutGetLeft(root) + YGNodeLayoutGetLeft(text) + YGNodeLayoutGetWidth(text),
            YGNodeLayoutGetTop(root) + YGNodeLayoutGetTop(text) + YGNodeLayoutGetHeight(text)
    );
    
    glColor3f(1.0, 0.0, 0.0);
    glRecti(
        YGNodeLayoutGetLeft(root) + YGNodeLayoutGetLeft(image),
        YGNodeLayoutGetTop(root) + YGNodeLayoutGetTop(image),
        YGNodeLayoutGetLeft(root) + YGNodeLayoutGetLeft(image) + YGNodeLayoutGetWidth(image),
        YGNodeLayoutGetTop(root) + YGNodeLayoutGetTop(image) + YGNodeLayoutGetHeight(image)
    );
    glFlush();
    
    YGNodeFreeRecursive(root);
    YGConfigFree(config);

    // nvgBeginFrame(vg, windowWidth, windowHeight, 1);

    // nvgBeginPath(vg);
    // nvgRect(vg, 100,100, 120,30);
    // nvgFillColor(vg, nvgRGBA(255,192,0,255));
    // nvgFill(vg);

    // nvgEndFrame(vg);


    glfwSwapBuffers(window);
}

int last_width;
int last_height;

void onrefresh (GLFWwindow* window) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    if (last_width != width || last_height != height) {
        dirty = true;
    }

    if (dirty) {
        // paint(window);
    }

    last_width = width;
    last_height = height;
}

bool paused = false;

void onfocus (GLFWwindow* window, int focused) {
    paused = !focused;
}

int main(void) {
    GLFWwindow* window;
    
    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // NVGcontext* vg = nvgCreateGL2(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
    
    glfwMakeContextCurrent(window);

    glfwSetWindowRefreshCallback(window, &onrefresh);
    glfwSetWindowFocusCallback(window, &onfocus);
    glfwSwapInterval(2);
    
    while (!glfwWindowShouldClose(window)) {
        if (paused || !dirty) {
            glfwWaitEvents(); // saves a LOT of cpu
        } else {
            paint(window);
            glfwPollEvents();
        }
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}