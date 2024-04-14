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
#include "puck.h"
#include <time.h>
#include <stdlib.h>



Puck::Puck(float forceZ, float dirY, float speedX){
    this->position = Point3D(0,0,0.5);
    // this->direction = Vec3D(1,static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 0.2 - 0.1 ,0);
    this->direction = Vec3D(1, dirY ,0);
    this->friction = 90;
    this->speed = 0.7;
    this->gravity -= forceZ;
}

Puck::Puck(Point3D pos, Vec3D dir, float friction, float spd){
    this->position = pos;
    this->direction = dir;
    this->friction = friction;
    this->speed = spd;
    
}

void Puck::update(){

    if ( this->position.pz < -0.35){
        this->initialGravity *= 0.9;
        this->gravity = -initialGravity;
    }

    else {
        this->gravity += 0.2;
    }


    this->position.py = this->position.py + this->direction.dy*this->speed;
    this->position.px = this->position.px + this->direction.dx*this->speed;
    this->position.pz = this->position.pz - 0.05*this->gravity;



}

void Puck::draw() {
    glPushMatrix();
    glColor3f(0, 0, 0);
    glTranslatef(this->position.px, this->position.py, this->position.pz);
    glScalef(0.1, 0.1, 0.1);


    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= 360; i += 10) {
        float angle = i * 3.1415926 / 180;
        float x = cos(angle);
        float y = sin(angle);

        glVertex3f(x, y, 0.0f);      // Bottom vertex
        glVertex3f(x, y, 0.5f);      // Top vertex
    }
    glEnd();

    //top and bottom faces
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, 0.0f, 0.0f);   //center of the circle
    for (int i = 0; i <= 360; i += 10) {
        float angle = i * 3.1415926 / 180;
        float x = cos(angle);
        float y = sin(angle);

        glVertex3f(x, y, 0.0f);      
    }
    glEnd();
    glColor3f(1, 1, 1);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, 0.0f, 0.5f);   //center of the circle
    for (int i = 0; i <= 360; i += 10) {
        float angle = i * 3.1415926 / 180;
        float x = cos(angle);
        float y = sin(angle);

        glVertex3f(x, y, 0.5f);      
    }
    glEnd();

    glPopMatrix();
}






