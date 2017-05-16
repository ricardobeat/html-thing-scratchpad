#include <GLFW/glfw3.h>
#include <Yoga.h>
#include <stdlib.h>

int main(void)
{
    GLFWwindow* window;
    
    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
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
        
        /* Render here */
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
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
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        
        /* Poll for and process events */
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}