#include <Windows.h>
#include <Mmsystem.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <time.h>;

using namespace std;

const float PI = 3.14, DEPTH = 7;
// Start and end of camera movement
const int ENDX = 10, STARTX = -500;
// Angle of branches, and depth of tree
float ANGLE = 20, depth = 0;
vector<string> *trees = new vector<string>();


double lastTime = 0, elapsedTime = 0, lastElapsedTime = 0;

bool cam = false;

float eyeX, eyeY, eyeZ, lookX, lookY, lookZ,
    upX, upY, upZ, fieldOfView, length = 0.001, num = 0,
        incr = 0.1;

float lineWidth = 5;
// L-System
string str = "FFFFX";

void push(){
    glPushMatrix();
    if (lineWidth > 0)
        lineWidth -= 1;


}

void pop(){
    glPopMatrix();
    lineWidth += 1;

}
void rotR2D()
{
    glRotatef(ANGLE, 0, 0, 1);
}

void rotL2D() {
    glRotatef(-ANGLE, 0, 0, 1);
}
void pitchDown(){
            glRotatef(360-ANGLE*4, 0, 1, 0);
}
void pitchUp(){
            glRotatef(ANGLE*4, 0, 1, 0);
}

void drawLine(){
    glPushAttrib(GL_LIGHTING_BIT);//saves current lighting stuff

            GLfloat ambient[4] = {0.55, 0.27, 0.07};    // ambient reflection
            GLfloat specular[4] = {0.55, 0.27, 0.07};   // specular reflection
            GLfloat diffuse[4] = {0.55, 0.27, 0.07};   // diffuse reflection

            // set the ambient reflection for the object
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
            // set the diffuse reflection for the object
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
            // set the specular reflection for the object
            glLineWidth(lineWidth);

            glBegin(GL_LINES);

                glVertex3f(0, 0, 0);
                glVertex3f(0, length, 0);
            glEnd();

            glTranslatef(0, length, 0);
    glPopAttrib();
}


void draw(){

    string ch = "";
    string LSystem = trees->at(depth);
    for (int i = 0; i < LSystem.length(); i++){
        ch = LSystem.at(i);

        if (ch.compare("F") == 0 || ch.compare("X") == 0){
            drawLine();
        } else if (ch.compare("[") == 0){
            push();
        } else if (ch.compare("]") == 0){
            pop();
        } else if (ch.compare("^") == 0){
            pitchUp();
        } else if (ch.compare("&") == 0){
            pitchDown();
        }
        else if (ch.compare("+") == 0){
            rotR2D();
        }
        else if (ch.compare("-") == 0){
            rotL2D();
        }
    }
}

void expand(float num){
    string ch = "";

    for (int i = 0; i < str.length(); i++){
        ch = str.at(i);

        if (ch.compare("X") == 0 ){
            str.replace(i, 1, "[Y][^^^Y][&&&Y]");
            i = i + 14;
        }
        else if (ch.compare("Y") == 0){
            str.replace(i, 1, "[++G][--H]TY");
            i = i + 11;
        }
        else if (ch.compare("H") == 0){
            str.replace(i, 1, "-G[+H]L");
            i = i + 6;
        }
        else if (ch.compare("G") == 0) {
            str.replace(i, 1, "+H[-G]L");
            i = i + 6;
        }
        else if (ch.compare("T") == 0) {
            str.replace(i, 1, "TL");
            i = i + 1;
        }
        else if (ch.compare("L") == 0) {
            str.replace(i, 1, "[[-FF][+FF]][^^^[-FF][+FF]][&&&[-FF][+FF]]F");
            i = i + 42;
        }
    }
    trees->push_back(str);
}




void display(void){
    // start by clearing the screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective (fieldOfView, 1.0, 1, 2000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eyeX, eyeY, eyeZ, lookX, lookY, lookZ, 0, 1, 0);



    glPushMatrix();

    glPushAttrib(GL_LIGHTING_BIT); //saves current lighting stuff
    GLfloat ambient[4] = { 0.82, 0.41, 0.12 };    // ambient reflection
    GLfloat diffuse[4] = { 0.82, 0.41, 0.12};   // diffuse reflection
        // set the ambient reflection for the object
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
        // set the diffuse reflection for the object
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);

    glBegin(GL_TRIANGLES);
        glVertex3f(-20,0,-20);
        glVertex3f(20,0,-20);
        glVertex3f(20,0,20);
        glVertex3f(-20,0,20);
        glVertex3f(-20,0,-20);
        glVertex3f(20,0,20);

    glEnd();
    glPopMatrix();
    glPopAttrib();

    draw();
    glutSwapBuffers();
    glutPostRedisplay();
}

void animate(){

    if (lastTime == 0)
        lastTime = timeGetTime();

    elapsedTime = timeGetTime()-lastTime;

    if (depth < DEPTH)
        length += 0.001;

    if(elapsedTime-lastElapsedTime > 2000 && depth < DEPTH){
        depth++;
        lastElapsedTime = elapsedTime;
        cout <<"a ";

    }
    elapsedTime = elapsedTime/5000;
    float t = (sin((elapsedTime*PI-PI/2))+1)/2;
    float p = (1-t)*STARTX + t*ENDX;

    if (cam)
        eyeX = p;
    glutPostRedisplay();

}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 113:         // q - Exit the program
         exit(0);
         break;
      case 119:         // w - Reset the camera
          fieldOfView = 45;
          eyeX = 150;
          eyeY = 100;
          eyeZ = 150;
          lookX = 0;
          lookY = 50;
          lookZ = 0;
          break;
      case 122:         // z - Reduce the field of view of the camera
          fieldOfView -= 5;
          glutPostRedisplay();
          break;
      case 120:         // x - Increase the field of view of the camera
          fieldOfView += 5;
          glutPostRedisplay();
          break;
    case 115:           // s - Stop moving the camera
          cam = false;
          break;
     case 97:           // a - Move the camera
          cam = true;
          break;
    case 100:           // d - Increase camera X-coordinate
          eyeX++;
          break;
     case 102:          // f - Decrease camera X-coordinate
          eyeX--;
          break;
   }
}

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (1000, 800);
    glutInitWindowPosition (0, 0);
    glutCreateWindow ("Pine Tree");

    fieldOfView = 45;
    eyeX = 250;
    eyeY = 100;
    eyeZ = 100;
    lookX = 0;
    lookY = 50;
    lookZ = 0;
    srand (time(NULL));
    num = (float) rand()/RAND_MAX;


    // set the lighting
    glShadeModel(GL_SMOOTH);
    GLfloat lightP[4] = {0.0, 800.0, 0.0,1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, lightP);

    // set the ambient light colour
    GLfloat lightA[4] = {0.0,0.9,0.9,1};
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightA);

    // set the specular light colour
    GLfloat lightS[4] = {0.9,0.9,0.9,1.0};
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightS);

    // set the diffuse light colour
    GLfloat lightD[4] = {0.9,0.9,0.9,1.0};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightD);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);


    /* Use depth buffering for hidden surface elimination. */
    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(animate);
    trees = new vector<string>();
    for(int i = 0; i <= DEPTH; i++){
        expand(num);
    }
    glutMainLoop();
    return 0;
}
