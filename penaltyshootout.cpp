#include <stdlib.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <cstring>


#include <bits/stdc++.h> 

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include "goalkeeper.h"
#include "ball.h"
#include "puck.h"
#include "post.h"
#include "hockeystick.h"
#include "imageio.h"
using namespace std;

bool isPaused = false;
int gHeight = 400;
int gWidth = 400;
int ang = 0;	
int cnt = 0;
float eye[3] = {20,20,20}; //point of view initialised

//lighting metrics
float light_pos[2][4] = {{3,5,7,1}, {10,10,10,1} };
float amb[2][4] = { { 0.9, 0.8,1, 1} , {0.6,0.3,0.5,1.0} };
float diff[2][4] = { {0,0,1,0.5} , {0.9,0.9,0.5,1.0}  };
float spec[2][4] = { {1,1,1,1}, {1,1,1,1}  };

//mouse controls
float mouseX = 0;
float mouseY = 0;

//booleans for game states
bool AtMenu = true;
bool gameOngoing = false;
bool gameOverScreen = true;
int textX = 0;
bool isLeftPressed = false;
bool isRightPressed = false;
bool isWhiteGround = false;

//speed and direction initialisation
int score = 0;
const float SPEEDXMINIMUM = 0.3;
const float DIRYMINIMUM = 0;
const float FORCEZMINIMUM = 0;
float speedX = SPEEDXMINIMUM;
float forceZ = FORCEZMINIMUM;
float dirY = DIRYMINIMUM;
float skey = false;




Vec3D currRing = Vec3D();

//Ball and Puck initialised
Ball hockeyBall(0,0,0);
Puck hockeyPuck(0,0,0);

//Hockeystick initialised
Hockeystick hockeystick;

//Goalkeeper Initialization
Goalkeeper gk = Goalkeeper(0.05, 1.35);

//Post Initalization
Post post = Post();

GLfloat materialAmbient[3][4] = {
    { 0.2, 0.2, 1, 1.0 },
    { 0.0215, 0.1745, 1, 1.0 },
     { 0.0, 0.019, 1, 1.0 }
};
GLfloat materialDiffuse[3][4] = {
    { 0.8, 0.8, 0.8, 1.0 },
    { 0.07568, 0.61424, 0.07568, 1.0 },
    { 0.04, 0.12, 0.07568, 1.0 }

};
GLfloat materialSpecular[3][4] = {
    { 0, 0, 1, 1 },
    { 0.11, 0.20, 0.7, 1 },
    { 0.2, 0.2, 1, 1 }
};
GLfloat materialShiny[3] = {
    0,
    0.6,
    0.9
};

GLfloat planeMaterialAmbient[] = { 0, 0 ,0, 1 };
GLfloat planeMaterialDiffuse[] = { 0.184313725, 0.458823529 ,0.196078431, 1 };
GLfloat planeMaterialSpecular[] = { 0.0, 0.0, 0.0, 1 };
GLfloat planeMaterialShiny = 120.0;


//texture global variables
unsigned char *image;
int width, height, bpp;
int texImageWidth;
int texImageHeight;

//spotlight function
void setupSpotlight() {
    GLfloat spot_direction[] = { -0.5, 1.0, 0.0, 0.0 };
    GLfloat spot_cutoff[] = { 60.0 };
    GLfloat spot_exponent[] = { 2.0 };
    GLfloat spot_attenuation[] = { 0.0, 0.1, 0.0 };

    // enable lighting 
    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_POSITION, light_pos[1]);
    glLightfv(GL_LIGHT1, GL_AMBIENT, amb[1]);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diff[1]);
    glLightfv(GL_LIGHT1, GL_SPECULAR, spec[1]);
    
    // spotlight parameters
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
    glLightfv(GL_LIGHT1, GL_SPOT_CUTOFF, spot_cutoff);
    glLightfv(GL_LIGHT1, GL_SPOT_EXPONENT, spot_exponent);
    glLightfv(GL_LIGHT1, GL_CONSTANT_ATTENUATION, &spot_attenuation[0]); // Pass address
    glLightfv(GL_LIGHT1, GL_LINEAR_ATTENUATION, &spot_attenuation[1]);   // Pass address
    glLightfv(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, &spot_attenuation[2]); // Pass address
}


//image to texture
GLubyte *makeTexImage(char *file){
	
	int width, height;
	GLubyte *texImage;
	texImage = loadImageRGBA((char*) file, &width, &height);	
	texImageWidth = width;
	texImageHeight = height;
	return texImage;
	
	
}

//texture for ice and grass
unsigned int texture1;
unsigned int texture2;
unsigned int textureSky;
void load_texture1(const char* filename){
    
    glGenTextures(1, &texture1);
    GLubyte *texImage = makeTexImage((char*)filename);
    if (!texImage) {
        printf("\nError reading image \n");
        return;
    }
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set texture wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texImageWidth, texImageWidth, 0, GL_RGBA, GL_UNSIGNED_BYTE, texImage);
    delete[] texImage;    
}


void load_texture2(const char* filename){
    
    glGenTextures(1, &texture2);
    GLubyte *texImage = makeTexImage((char*)filename);
    if (!texImage) {
        printf("\nError reading image \n");
        return;
    }
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texImageWidth, texImageWidth, 0, GL_RGBA, GL_UNSIGNED_BYTE, texImage);
    delete[] texImage;    
}
void loadGroundTexture(bool isWhiteGround) {
    if (isWhiteGround) {
        load_texture2("ice_texture.png"); // Load ice texture
    } else {
        load_texture1("grass2.png"); // Load grass texture
    }
}

void drawObject() {
    if (isWhiteGround) {
        hockeyPuck.draw();  // Draw puck
    } else {
        hockeyBall.draw();  // Draw ball
    }
}
void setMaterials(unsigned int index) {
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialAmbient[index]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialDiffuse[index]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpecular[index]);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, materialShiny[index]);

}

//3D path of ball/puck drawn
void drawPath(){
    glPointSize(5);
    glBegin(GL_POINTS);
    glColor3f(0,1,1);

    float x = 0;
    float y = 0;
    float z = 0.5;
    float initialGravity = 1 - forceZ;
    float gravity = 1;

    for ( int i = 0; i < 1000; i++){
        if ( i % 2 == 0){
            x = x + speedX;
            y = y + dirY;

            if (z < 0.1){
                initialGravity *= 0.9;
                gravity = -initialGravity;
                }
            else {
                gravity += 0.2;
            }
                z = z - 0.05*gravity;

            glVertex3f(x,y,z);

        }
        
        
        
    }
    glEnd();
}

//Heads-Up display for information
void drawHUD(){
    glPushMatrix();
    glTranslatef(60,20,15);
    glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
    glRasterPos2i(0, 0);
    glPopMatrix();

    if ( AtMenu){ //game hasn't started

            const char textIntro[] 
                = "WELCOME TO HOCKEY SHOOT OUT! \n  \nTO GET STARTED PRESS THE 'S' KEY!";
            for (int i = 0; i < strlen(textIntro); i++){
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, textIntro[i]);
            }
    }

    else if ( gameOngoing) {

        unsigned char PointsQ[] 
                = "CURRENT POINTS:";
        unsigned char timeQ[] 
                = "TIME REMAINING:";
        unsigned char time[2];
        time[0] = cnt & 0xFF;
        time[1] = (cnt >> 8) & 0xFF;

        char buf[256];
        snprintf(buf, sizeof(buf) - 1, "YOU HAVE %d SECONDS REMAINING, %d POINTS SCORED", 60 - cnt/60, score);
        int track = 60 - cnt/60;
        for (int i = 0; i < strlen(buf); i++){
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, buf[i]);
            }

        if (track == 0){
            gameOverScreen = true;
            gameOngoing = false;
        }
    }

    else if (gameOverScreen){

        char buf[256];
        snprintf(buf, sizeof(buf) - 1, "GAME OVER!!\nYou scored %d points!", score);
        for (int i = 0; i < strlen(buf); i++){
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, buf[i]);
            }

    }
}


//plane with texture initialised
void createPlane(){
    loadGroundTexture(isWhiteGround);
    glPushMatrix();

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, planeMaterialAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, planeMaterialDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, planeMaterialSpecular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, planeMaterialShiny);

    //texture mapping
    glEnable(GL_TEXTURE_2D); 
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    glBegin(GL_QUADS);

    glNormal3f(0,0,1);

    //texture coordinates
    glTexCoord2f(0, 0);
    glVertex3f(-20,-10,0);
    //glVertex3f(-10,-5,0);

    glTexCoord2f(0.5, 0);
    glVertex3f(20,-10,0);
    //glVertex3f(10,-5,0);

    glTexCoord2f(0.5, 0.5);
    glVertex3f(20,10,0);
    //glVertex3f(10,5,0);

    glTexCoord2f(0, 0.5);
    glVertex3f(-20,10, 0);
    //glVertex3f(-10,5,0);

    glEnd();

    glDisable(GL_TEXTURE_2D); //disable texture mapping 
    glPopMatrix();
}



void reshape( int w, int h){
    gWidth = w;
    gHeight = h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,w,0,h);

    glMatrixMode(GL_MODELVIEW);
    glViewport(0,0,w,h);

    glutPostRedisplay();
}


void initObject() {
    if (isWhiteGround) {
        hockeyPuck = Puck(forceZ , dirY, speedX);  // Initialize puck
    } else {
        hockeyBall = Ball(forceZ , dirY, speedX);  // Initialize ball
    }
}

//target initialisation and control
void targets(){
    int track = 60 - cnt/60;
    switch (track){
    
    case 50 ... 60:
    case 20 ... 29:
    currRing = Vec3D(20.0,0.0,1.0);
    glPushMatrix();
    glTranslatef(20.0,0.0,1.0);
    glRotatef(90.0,0.0,1.0,0);
    glBegin(GL_POINTS);
    for(int i=0;i<1000;++i)
    {
    glVertex3f(cos(2*3.14159*i/1000.0),sin(2*3.14159*i/1000.0),0);
    }
    glEnd();
    glPopMatrix();;   
    break;
    case 40 ... 49:
    case 10 ... 19:
    currRing = Vec3D(20.0,2.0,1.0);
    glPushMatrix();
    glTranslatef(20.0,2.0,1.0);
    glRotatef(90.0,0.0,1.0,0);
    glBegin(GL_POINTS);
    for(int i=0;i<1000;++i)
    {
    glVertex3f(cos(2*3.14159*i/1000.0),sin(2*3.14159*i/1000.0),0);
    }
    glEnd();
    glPopMatrix();;   
    break;

    case 30 ... 39:
    case 0 ... 9:
    currRing = Vec3D(20.0,-2.0,1.0);
    glPushMatrix();
    glTranslatef(20.0,-2.0,1.0);
    glRotatef(90.0,0.0,1.0,0);
    glBegin(GL_POINTS);
    for(int i=0;i<1000;++i)
    {
    glVertex3f(cos(2*3.14159*i/1000.0),sin(2*3.14159*i/1000.0),0);
    }
    glEnd();
    glPopMatrix();;   
    break;
    
    }
}


//updation for both ball/puck handled
void update() {
    if (gameOngoing) {
        if (isWhiteGround) {
        	//puck logic
        	if (hockeyPuck.position.px >= 45) {
                initObject();
                skey = false;
                score += 1;
            } else if (hockeyPuck.speed <= -0.5) {
                initObject();
                skey = false;
            } else if (skey) {
                hockeyPuck.update();
                Point3D temppos = hockeyPuck.position;
                if (hockeyPuck.speed <= 0)
                    hockeyPuck.speed -= 0.005;

                if (fabs(temppos.py - gk.position.py) <= 0.01) {
                    //COLLISION!
                    hockeyPuck.speed *= -1.0;
                } else if (fabs(currRing.dy - hockeyPuck.position.py) <= 0.5 && fabs(currRing.dx - hockeyPuck.position.px) <= 0.5) {
                    initObject();
                    skey = false;
                    score += 2;
                }
            }
        } else {
            //ball logic
            if (hockeyBall.position.px >= 45) {
                initObject();
                skey = false;
                score += 1;
            } else if (hockeyBall.speed <= -0.5) {
                initObject();
                skey = false;
            } else if (skey) {
                hockeyBall.update();
                Point3D temppos = hockeyBall.position;
                if (hockeyBall.speed <= 0)
                    hockeyBall.speed -= 0.005;

                if (fabs(temppos.py - gk.position.py) <= 0.01) {
                    //COLLISION!
                    hockeyBall.speed *= -1.0;
                } else if (fabs(currRing.dy - hockeyBall.position.py) <= 0.5 && fabs(currRing.dx - hockeyBall.position.px) <= 0.5) {
                    initObject();
                    skey = false;
                    score += 2;
                }
            }
        }
    
    if (cnt / 60 == 60) {
        gameOverScreen = true;
        gameOngoing = false;
    }
}
}


void draw3DScene(){
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1, 1, 100);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    float eyeY = 20 * sin(ang * 3.14159 / 180); //Y coordinate of eye changing with angle
    gluLookAt(0, eyeY, 2, //updated eye position
              20, 0 , 0,     
              0, 0, 1);
    
    //objects initialised in order
    glPushMatrix();   
    glTranslatef(2,0,1);
    update();
    hockeystick.draw();
    drawObject();
    glPopMatrix(); 
    targets();
    gk.drawGK();
    gk.gkSideways();
    createPlane();
    drawHUD();
    drawPath();
    post.drawNet();
}


void FPS(int val){
    cnt++;
    if (!isPaused){
        glutPostRedisplay();
    }
    glutTimerFunc(17, FPS, 0); // 1sec = 1000, 60fps = 1000/60 = ~17
}




void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw3DScene();
    glutSwapBuffers();
    
}

//keyboard controls
void kbd(unsigned char key, int x, int y)
{
    
    switch(key){
    	case 'd'://a and d for perspective change
            if (ang > -3) 
                ang--;     
            break;
        case 'a':
            if (ang < 3)  
                ang++;     
            break;
        case 't':
            textX--;
            break;
        case 27://esc key
            exit(0);
            break;
        case 's'://start
            if (cnt/60 < 60){
                AtMenu = false;
                gameOngoing = true;
                skey = !skey;
            }
            break;
        case 'x'://increase speed of ball
            speedX += 0.1;
            break;
        case 'c'://decrease speed of ball
            speedX -= 0.1;
            break;
        case ' ':
            if (!AtMenu && gameOngoing){
                initObject();
            }
            break;
        case '1'://Goalkeeper difficulty changes
            gk = Goalkeeper(0.025,1);
            break;
        case '2':
            gk = Goalkeeper(0.05, 1.35);
            break;
        case '3':
            gk = Goalkeeper(0.1, 1.75);
            break;
        case 'r'://reset
            initObject();
            gk = Goalkeeper(0.05, 1.35);
            score = 0;
            cnt = 0;
    }


}

//direction and momentum changers
void SpecialInput(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_UP:
            forceZ += 0.2;
        break;

        case GLUT_KEY_DOWN:
            forceZ -= 0.2;
        break;

        case GLUT_KEY_LEFT:
            dirY += 0.01;
        break;

        case GLUT_KEY_RIGHT:
            dirY -= 0.01;
        break;
    }
}


void updateMousePos(int x, int y){

    mouseX = x;
    mouseY = y;
    
}

//mouse controls
void mouse(int btn, int state, int x, int y){
    mouseX = x;
    mouseY = y;

    switch (btn)
    {
    case GLUT_LEFT_BUTTON:
        isLeftPressed = (state == GLUT_DOWN);
        break;

    case GLUT_RIGHT_BUTTON:
        isRightPressed = (state == GLUT_DOWN);
        break;
    
    default:
        break;
    }

}

//for deleting textures
void cleanUp() {
    glDeleteTextures(1, &texture1);
    glDeleteTextures(1, &texture2);
    glDeleteTextures(1, &textureSky);
}

int main(int argc, char** argv)
{	
    int choice;
    cout << "Choose the game to play: \n";
    cout << "1. Grass hockey\n";
    cout << "2. Ice hockey\n";
    cin >> choice;
    printf("\n"
           "up arrow -> increase upward momentum of the kick\n"
           "down arrow -> decrease upward momentum of the kick\n"
           "left arrow -> angle kick to the left\n"
           "right arrow -> angle kick to the right\n"
           "x -> increase speed of the ball\n"
           "c -> decrease speed of the ball\n"
           "r -> reset\n"
           "s -> to kick\n"
           "1 -> change difficulty to EASY\n"
           "2 -> change difficulty to MEDIUM\n"
           "3 -> change difficulty to HARD\n"
    );

    glutInit(&argc, argv);
    glutInitWindowSize(800, 800);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Penalty Shootout");
    glutTimerFunc(0, FPS, 0);
    glutPassiveMotionFunc(updateMousePos);
    glutMotionFunc(updateMousePos);
    glutMouseFunc(mouse);
    glClearColor(0.5294117647,0.807,0.98, 0.8);

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos[0]);
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb[0]);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diff[0]);
    glLightfv(GL_LIGHT0, GL_SPECULAR, spec[0]);

    glutReshapeFunc(reshape);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace( GL_BACK );
    
    setupSpotlight();//spotlight
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	
    //textures added
    if (choice == 1) {
    isWhiteGround = false;
        
    } else if (choice == 2) {
        isWhiteGround = true;
    } else {
        cout << "Invalid choice. Exiting...\n";
        return 1;
    }
    
    glutKeyboardFunc(kbd);
    glutSpecialFunc(SpecialInput);
    glutDisplayFunc(display);
    atexit(cleanUp);//texture variable deletion
    glutMainLoop();
	
    return 0;
}
