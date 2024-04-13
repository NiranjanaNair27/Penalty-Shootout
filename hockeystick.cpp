#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "objLoad.cpp"
#include "hockeystick.h"

objLoad loadHockeystick = objLoad("./hockey_stick2.obj");

Hockeystick::Hockeystick() {
    this->position = Point3D(0.0,0.7,1.0);
}

void Hockeystick::draw() {
    glPushMatrix();
    glTranslatef(this->position.px, this->position.py, this->position.pz);
    glRotatef(140.0f, 1.0f, 0.0f, 0.0f);
    glScalef(0.001f,0.001f,0.001f);
    loadHockeystick.draw();
    glPopMatrix();
}


