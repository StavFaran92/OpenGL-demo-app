#include "DebugHelper.h"

#include <GL/glew.h>


void DebugHelper::drawPoint(const glm::vec3& pos)
{
    //glClear(GL_COLOR_BUFFER_BIT); // Clear the screen
    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    //glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0); // Set orthographic projection
    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();

    //glPointSize(100);       // Set the size of the point
    //glColor3f(1.f, 0.f, 0.f);           // Set the color of the point

    //glBegin(GL_POINTS);           // Begin drawing points
    //glVertex2f(50, 50);             // Specify the point's position
    //glEnd();                      // End drawing
}

void DebugHelper::drawLine(const glm::vec3& p1, const glm::vec3& p2)
{
}

void DebugHelper::drawTriangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
{
}
