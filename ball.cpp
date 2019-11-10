#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include "ball.h"

Ball::Ball(){
    this->position = Point3D(0,0,0);
    this->direction = Vec3D();
    this->friction = 95;
    this->speed = 0;
}

Ball::Ball(Point3D pos, Vec3D dir, float friction, float spd){
    this->position = pos;
    this->direction = dir;
    this->friction = friction;
    this->speed = spd;
}

void Ball::draw(){
    glPushMatrix();
        glColor3f(0.1, 0.1, 0.1);
        glTranslatef(this->position.px, this->position.py, this->position.pz);
        glPushMatrix();
            glutWireSphere(0.3, 10, 10);
        glPopMatrix();
    glPopMatrix();
}