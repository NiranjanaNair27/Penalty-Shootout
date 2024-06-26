#ifndef POST_H
#define POST_H

#include "mathLib3D.h"
#include <time.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#endif

class Post {

    public:
        Post();

        //vertices for the post
        float postVerts[8][3] = {
            {-0.1, -0.1, 0.1},
            {-0.1, 0.1, 0.1},
            {0.1, 0.1, 0.1},
            {0.1, -0.1, 0.1},
            {-0.1, -0.1, -0.1},
            {-0.1, 0.1, -0.1},
            {0.1, 0.1, -0.1},
            {0.1, -0.1, -0.1}
        };

        //indices for each of the faces of the cube
        int indices[6][4] = {
            //top
            {1, 2, 6, 5},
            //left
            {1, 5, 4, 0},
            //back
            {5, 7, 7, 4},
            //right
            {2, 3, 7, 6},
            //bottom
            {0, 4, 7, 3},
            //front
            {1, 0, 3, 2}
        };

        int numFaces;

        float postColor[3];

        void drawPostIndex();
        void setColors(float r, float g, float b);

        void drawNet();
        //void loadTexture(const char* filename)

    private:
        void drawPost(int index);
};

/*class Post {

    public:
        Post();
	
        // Vertices for the post
        float postVerts[8][3] = {
            {-0.1, -0.1, 0.1},
            {-0.1, 0.1, 0.1},
            {0.1, 0.1, 0.1},
            {0.1, -0.1, 0.1},
            {-0.1, -0.1, -0.1},
            {-0.1, 0.1, -0.1},
            {0.1, 0.1, -0.1},
            {0.1, -0.1, -0.1}
        };
        
        float postTexCoords[4][2] = {
        {0.0, 0.0},
        {1.0, 0.0},
        {1.0, 1.0},
        {0.0, 1.0}
    	};

        // Indices for each of the faces of the cube
        int indices[6][4] = {
            // Top
            {1, 2, 6, 5},
            // Left
            {1, 5, 4, 0},
            // Back
            {5, 7, 7, 4},
            // Right
            {2, 3, 7, 6},
            // Bottom
            {0, 4, 7, 3},
            // Front
            {1, 0, 3, 2}
        };

        int numFaces;

        float postColor[3];
	void initTextureCoords();
        void drawPostIndex();
        void setColors(float r, float g, float b);
        void drawNet();
        GLubyte *makeTexImage(char *file);
        void load_texture1(const char *filename);

    private:
        void drawPost(int index);

};*/



#endif
