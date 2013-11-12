#include <stdlib.h>
#include <math.h>
#include "CImg.h"
#include "Puzzle.h"
#include <iostream> //TODO: get rid of this
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace cimg_library;

float cx=0.0f, cy=10.0f, cz=1.0f;
float dx=0.0f, dy=-10.0f, dz=-1.0f;
const float fraction = 0.1f;
float zoom = 0.0f;
Puzzle *puzzle;
Piece *piece;
int prev_x, prev_y;

void reshape(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;
	float ratio =  w * 1.0f / h;

    
	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

    // Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void draw(void) {

	// Clear Color, Depth, and Stencil buffers
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	// Reset transformations
	glLoadIdentity();

	cx += zoom * dx;
	cy += zoom * dy;
	cz += zoom * dz;
	zoom = 0.0f;

	// Set the camera
	gluLookAt(	cx, cy, cz,
				cx+dx, cy+dy, cz+dz,
				0.0f, 1.0f,  0.0f);

    // Draw playing surface
	glStencilFunc(GL_ALWAYS, 0, -1);
	glColor3f(0.9f, 0.9f, 0.9f);
	glBegin(GL_QUADS);
		glVertex3f(-100.0f, 0.0f, -100.0f);
		glVertex3f(-100.0f, 0.0f,  100.0f);
		glVertex3f( 100.0f, 0.0f,  100.0f);
		glVertex3f( 100.0f, 0.0f, -100.0f);
	glEnd();

	puzzle->draw();
	 
	glutSwapBuffers();
}

void keyPressed(unsigned char key, int x, int y) {

	switch(key) {
	case 27: // Escape or q to quit
	case 'q':
	case 'Q':
		exit(0);
		break;
	case 'n': // zoom in
	case 'N':
		zoom = 0.01f;
		break;
	case 'm': // zoom out
	case 'M':
		zoom = -0.01f;
		break;
	}
}

void specKeyPressed(int key, int xx, int yy) {

	switch (key) {
		case GLUT_KEY_LEFT :
			cx-=fraction;
			break;
		case GLUT_KEY_RIGHT :
			cx+=fraction;
			break;
		case GLUT_KEY_UP :
			cz-=fraction;
			break;
		case GLUT_KEY_DOWN :
			cz+=fraction;
			break;
	}
}

void mousePressed(int button, int state, int x, int y) {
	
	if(state == GLUT_UP) {
		piece = NULL;
		return;
	}

	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	GLuint id;
	glReadPixels(x, height-y-1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &id);
	piece = puzzle->getPiece(id);
	prev_x = x; prev_y = y;
}

void mouseMoved(int x, int y) {

	if(piece != NULL) {
		float fac = 0.001f * cy;
		float delta_x = -fac * (prev_x - x); 
		float delta_z = -fac * (prev_y - y);
		piece->move(delta_x, 0.0f, delta_z);
	}
	prev_x = x; prev_y = y;
}

int main(int argc, char **argv) {

	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL);
	glutInitWindowPosition(50,50);
	glutInitWindowSize(1100, 600);
	glutCreateWindow("Jigsaw Puzzle");
	puzzle = new Puzzle(3);
	piece = NULL;

	// register callbacks
	glutDisplayFunc(draw);
	glutReshapeFunc(reshape);
	glutIdleFunc(draw);
	glutKeyboardFunc(keyPressed);
	glutSpecialFunc(specKeyPressed);
	glutMouseFunc(mousePressed);
	glutMotionFunc(mouseMoved);

	// OpenGL init
	glEnable(GL_DEPTH_TEST);

	// enter GLUT event processing cycle
	glutMainLoop();
	return 0;
}
