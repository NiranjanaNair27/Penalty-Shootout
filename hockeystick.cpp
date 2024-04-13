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

objLoad loadHockeystick = objLoad("./hockey_stick.obj");

Hockeystick::Hockeystick() {
    this->position = Point3D(0,0,0.5);
}

void Hockeystick::draw() {
    glPushMatrix();
    glTranslatef(this->position.px, this->position.py, this->position.pz);
    glScalef(1,1,1);
    loadHockeystick.draw();
    glPopMatrix();
}


