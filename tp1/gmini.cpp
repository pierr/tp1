// -------------------------------------------
// gMini : a minimal OpenGL/GLUT application
// for 3D graphics.
// Copyright (C) 2006-2008 Tamy Boubekeur
// All rights reserved.
// -------------------------------------------

// -------------------------------------------
// Disclaimer: this code is dirty in the
// meaning that there is no attention paid to
// proper class attribute access, memory
// management or optimisation of any kind. It
// is designed for quick-and-dirty testing
// purpose.
// -------------------------------------------

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#if __APPLE__
#	include <GLUT/glut.h>
#else
#	include <GL/glut.h>
#endif
#include "Vec3D.h"
#include "Camera.h"
#include <cmath>
#include "Vertex.h"
#include "Triangle.h"
#include "Zone.h"
#include "Grid.h"
#include "Mesh.h"
#define N_MER 5 //Le nombre de méridiens sur la sphère
#define N_PAR 5 // Le nombre de parallèles sur la sphère
using namespace std;



// -------------------------------------------
// OpenGL/GLUT application code.
// -------------------------------------------

static GLint window;
static unsigned int SCREENWIDTH = 640;
static unsigned int SCREENHEIGHT = 480;
static Camera camera;
static bool mouseRotatePressed = false;
static bool mouseMovePressed = false;
static bool mouseZoomPressed = false;
static int lastX=0, lastY=0, lastZoom=0;
static unsigned int FPS = 0;
static bool fullScreen = false;

typedef enum {Wireframe, Flat, Gouraud} PolygonMode;
static PolygonMode polygonMode = Flat;

static Mesh mesh;

void printUsage () {
    cerr << endl 
    << "gMini: a minimal OpenGL/GLUT application" << endl
    << "for 3D graphics." << endl 
    << "Author : Tamy Boubekeur (http://www.labri.fr/~boubek)" << endl << endl
    << "Usage : ./gmini [<file.off>]" << endl
    << "Keyboard commands" << endl 
    << "------------------" << endl
    << " ?: Print help" << endl 
    << " w: Toggle Wireframe/Flat/Gouraud Rendering Mode" << endl 
    << " f: Toggle full screen mode" << endl 
    << " <drag>+<left button>: rotate model" << endl 
    << " <drag>+<right button>: move model" << endl
    << " <drag>+<middle button>: zoom" << endl
    << " q, <esc>: Quit" << endl
    <<	"1 , 2, 3  are done to use the smooth feature with a parameter of value 0.1 , 0.5, 0.75" << endl 
    << "r reload the original mesh" << endl
    << "4, 5, 6 in order to do a simplification with a resolution 64x64x64, 32x32x32 et 16x16x16." << endl 
    << endl;
}

void usage () {
    printUsage ();
    exit (EXIT_FAILURE);
}



// ------------------------------------

void initLight (unsigned int nLight) {
    GLfloat light_position1[4] = {22.0f, 16.0f, 50.0f, 0.0f};
    GLfloat direction1[3] = {-52.0f,-16.0f,-50.0f};
    //GLfloat color1[4] = {0.5f, 1.0f, 0.5f, 1.0f};
    GLfloat ambient[4] = {0.3f, 0.3f, 0.3f, 0.5f};
    GLfloat color[4];
    switch (nLight) {
        case 1:
            color[0] = 0.5f;
            color[1] = 1.0f;
            color[2] = 0.5f;
            color[3] = 1.0f;
            break;
        case 2:
            color[0] = 0.7f;
            color[1] = 0.2f;
            color[2] = 0.3f;
            color[3] = 1.0f;
            break;
        case 3:
            color[0] = 0.2f;
            color[1] = 0.5f;
            color[2] = 0.7f;
            color[3] = 1.0f;
            break;
        default:
            break;
    }
    
    
    glLightfv (GL_LIGHT1, GL_POSITION, light_position1);
    glLightfv (GL_LIGHT1, GL_SPOT_DIRECTION, direction1);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, color);
    glLightfv (GL_LIGHT1, GL_SPECULAR, color);
    glLightModelfv (GL_LIGHT_MODEL_AMBIENT, ambient);
    glEnable (GL_LIGHT1);
    glEnable (GL_LIGHTING);
}

void init (const char * modelFilename) {
    camera.resize (SCREENWIDTH, SCREENHEIGHT);
	mesh.fileName = modelFilename;
	mesh.loadOFF (modelFilename);	
    initLight (1);
    glCullFace (GL_BACK);
    glEnable (GL_CULL_FACE);
    glDepthFunc (GL_LESS);
    glEnable (GL_DEPTH_TEST);
    glClearColor (0.2f, 0.2f, 0.3f, 1.0f);
}

void initCube(){
	camera.resize (SCREENWIDTH, SCREENHEIGHT);
    mesh.makeCube();
    initLight (1);
    glCullFace (GL_BACK);
    glEnable (GL_CULL_FACE);
    glDepthFunc (GL_LESS);
    glEnable (GL_DEPTH_TEST);
    glClearColor (0.2f, 0.2f, 0.3f, 1.0f);
}

void initSphere(unsigned int resU, unsigned int resV){
	camera.resize (SCREENWIDTH, SCREENHEIGHT);
    mesh.makeSphere(resU, resV);
    initLight (1);
    glCullFace (GL_BACK);
    glEnable (GL_CULL_FACE);
    glDepthFunc (GL_LESS);
    glEnable (GL_DEPTH_TEST);
    glClearColor (0.2f, 0.2f, 0.3f, 1.0f);
}

// ------------------------------------
// Replace the code of this 
// functions for cleaning memory, 
// closing sockets, etc.
// ------------------------------------

void clear () {
    
}

// ------------------------------------
// Replace the code of this 
// functions for alternative rendering.
// ------------------------------------


void draw () {
    //On récupère la matrice objet monde
    GLfloat	modl[16];
    glGetFloatv( GL_MODELVIEW_MATRIX, modl );
    GLfloat lightPos[4];
    glGetLightfv(GL_LIGHT1, GL_POSITION, lightPos);
    
    //Vec3Df lightPosition()
    const vector<Vertex> & V = mesh.V;
    const vector<Triangle> & T = mesh.T;
    glBegin (GL_TRIANGLES);
    for (unsigned int i = 0; i < T.size (); i++) {
        if (polygonMode != Gouraud) {
            Vec3Df e01 = V[T[i].v[1]].p -  V[T[i].v[0]].p;
            Vec3Df e02 = V[T[i].v[2]].p -  V[T[i].v[0]].p;
            Vec3Df n = Vec3Df::crossProduct (e01, e02);
            n.normalize ();
            glNormal3f (n[0], n[1], n[2]);
            //glColorMaterial(<#GLenum face#>, <#GLenum mode#>)
            //modl.V[T[i].v]
        }
        
        //glGetFloatv(<#GLenum pname#>, <#GLfloat *params#>)
        for (unsigned int j = 0; j < 3; j++) {
            const Vertex & v = V[T[i].v[j]];
            if (polygonMode == Gouraud)
                glNormal3f (v.n[0], v.n[1], v.n[2]);
            glVertex3f (v.p[0], v.p[1], v.p[2]);
            
        }
        
    }
    glEnd ();
}

void display () {
    glLoadIdentity ();
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera.apply ();
    draw ();
    glFlush ();
    glutSwapBuffers ();
}

void idle () {
    static float lastTime = glutGet ((GLenum)GLUT_ELAPSED_TIME);
    static unsigned int counter = 0;
    counter++;
    float currentTime = glutGet ((GLenum)GLUT_ELAPSED_TIME);
    if (currentTime - lastTime >= 1000.0f) {
        FPS = counter;
        counter = 0;
        static char winTitle [64];
        unsigned int numOfTriangles =(unsigned int) mesh.T.size ();
        sprintf (winTitle, "gmini - Num. Of Tri.: %d - FPS: %d", numOfTriangles, FPS);
        glutSetWindowTitle (winTitle);
        lastTime = currentTime;
    }
    glutPostRedisplay ();
}

void key (unsigned char keyPressed, int x, int y) {
    switch (keyPressed) {
        case 'f':
            if (fullScreen == true) {
                glutReshapeWindow (SCREENWIDTH, SCREENHEIGHT);
                fullScreen = false;
            } else {
                glutFullScreen ();
                fullScreen = true;
            }      
            break;
        case 'q':
        case 27:
            clear ();
            exit (0);
            break;
        case 'w':
            if (polygonMode == Wireframe) {
                polygonMode = Flat;
                glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
            } else if (polygonMode == Flat) {
                polygonMode = Gouraud;
            } else {
                polygonMode = Wireframe;
                glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
            }
            break;
        case '1':
            mesh.smooth(0.1);
            break;
        case '2':
            mesh.smooth(0.5);
            break;
        case '3':
            mesh.smooth(0.75);
            break;
        case 'r':
            cerr << "reload the oriinal mesh" << endl;
            mesh.reloadOFF();
            break;
        case '4':
            cerr << "Calling simplifyMesh with parameter value of 64" << endl;
            mesh.simplifyMesh(64);
            break;
        case '5':
            cerr << "Calling simplifyMesh with parameter value of 32" << endl;
            mesh.simplifyMesh(32);
            break;
        case '6':
            cerr << "Calling simplifyMesh with parameter value of 16" << endl;
            mesh.simplifyMesh(16);
            break;
        case 'k':
            initLight(1);
            break;
        case 'l':
            initLight(2);
            break;
        case 'm':
            initLight(3);
            break;
        case '?':
        default:
            printUsage ();
            break;
    }
    idle ();
}

void mouse (int button, int state, int x, int y) {
    if (state == GLUT_UP) {
        mouseMovePressed = false;
        mouseRotatePressed = false;
        mouseZoomPressed = false;
    } else {
        if (button == GLUT_LEFT_BUTTON) {
            camera.beginRotate (x, y);
            mouseMovePressed = false;
            mouseRotatePressed = true;
            mouseZoomPressed = false;
        } else if (button == GLUT_RIGHT_BUTTON) {
            lastX = x;
            lastY = y;
            mouseMovePressed = true;
            mouseRotatePressed = false;
            mouseZoomPressed = false;
        } else if (button == GLUT_MIDDLE_BUTTON) {
            if (mouseZoomPressed == false) {
                lastZoom = y;
                mouseMovePressed = false;
                mouseRotatePressed = false;
                mouseZoomPressed = true;
            }
        }
    }
    idle ();
}

void motion (int x, int y) {
    if (mouseRotatePressed == true) {
        camera.rotate (x, y);
    }
    else if (mouseMovePressed == true) {
        camera.move ((x-lastX)/static_cast<float>(SCREENWIDTH), (lastY-y)/static_cast<float>(SCREENHEIGHT), 0.0);
        lastX = x;
        lastY = y;
    }
    else if (mouseZoomPressed == true) {
        camera.zoom (float (y-lastZoom)/SCREENHEIGHT);
        lastZoom = y;
    }
}


void reshape(int w, int h) {
    camera.resize (w, h);
}


int main (int argc, char ** argv) {
    if (argc > 2) {
        printUsage ();
        exit (EXIT_FAILURE);
    }
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize (SCREENWIDTH, SCREENHEIGHT);
    window = glutCreateWindow ("gMini");
  	
	/*Pour charger le modele*/
    init (argc == 2 ? argv[1] : "sphere.off");
	
	/*Pour charger le cube*/
	//initCube();
	
	/*Pour charger la sphère*/
	//initSphere(N_MER,N_PAR);
	
	glutIdleFunc (idle);
    glutDisplayFunc (display);
    glutKeyboardFunc (key);
    glutReshapeFunc (reshape);
    glutMotionFunc (motion);
    glutMouseFunc (mouse);
    key ('?', 0, 0);   
    glutMainLoop ();
    return EXIT_SUCCESS;
}

