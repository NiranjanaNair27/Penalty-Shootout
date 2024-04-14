#include "post.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "imageio.h"

/*Post::Post()
{
    // Number of cube faces
    this->numFaces = 6;

    // Load texture
    loadTexture("post_texture.png");
}

void Post::loadTexture(const char* filename)
{
    // Generate texture ID
    glGenTextures(1, &textureID);

    // Bind texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load image
    int width, height, bpp;
    unsigned char* image = loadImageRGBA(filename, &width, &height, &bpp);

    // Assign texture image
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    // Free image data
    freeImage(image);

    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Post::drawPostIndex()
{
    glBindTexture(GL_TEXTURE_2D, textureID);
    for (int i = 0; i < this->numFaces; i++)
    {
        this->drawPost(i);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Post::drawPost(int index)
{
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(this->postVerts[index][0]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(this->postVerts[index][1]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(this->postVerts[index][2]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(this->postVerts[index][3]);
    glEnd();
}*/

Post::Post()
{
    //set post colors to white
    this->setColors(1,1,1);

    //number of cube faces
    this->numFaces = 6;
}


void Post::drawPostIndex()
{
    for (int i = 0; i < this->numFaces; i++)
    {

        glColor3fv(this->postColor);
        this->drawPost(i);
    }
}


void Post::drawNet()
{
    //top
    glPushMatrix();
        glTranslatef(18, 0, 2.42);
        glScalef(1.0, 30, 1.5);
        this->drawPostIndex();
    glPopMatrix();

    //right
    glPushMatrix();
        glTranslatef(18, 2.9, 1.2);
        glScalef(1.0, 1.0, 11.75);
        this->drawPostIndex();
    glPopMatrix();

    //left
    glPushMatrix();
        glTranslatef(18, -2.9, 1.2);
        glScalef(1.0, 1.0, 11.75);
        this->drawPostIndex();
    glPopMatrix();
}



void Post::setColors(float r, float g, float b)
{
    this->postColor[0] = r;
    this->postColor[1] = g;
    this->postColor[2] = b;
}


void Post::drawPost(int index)
{
    glBegin(GL_POLYGON);
    for (int i = 0; i < 4; i++)
    {
        int vIndex = this->indices[index][i];
        glVertex3fv(this->postVerts[vIndex]);
    }
    glEnd();
}


