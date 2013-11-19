#include <stdlib.h>
#include <math.h>
#include "Puzzle.h"
#include "Joined.h"
#include <iostream> //TODO: get rid of this
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

float cx=0.0f, cy=10.0f, cz=1.0f;
float dx=0.0f, dy=-10.0f, dz=-1.0f;
const float fraction = 0.1f;
float zoom = 0.0f;
Puzzle *puzzle;
Piece *piece;
Group *group;
int prev_x, prev_y;
unsigned int group_button_texture;
bool move = false;

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

void drawHighlightRect(float r, float g, float b, Piece *piece) {
	const float extra = 0.05f;
	glStencilFunc(GL_ALWAYS, 0, -1);
	float w, h;
	Coord *pos = piece->getPos(false);
	w = piece->getWidth() + extra;
	h = piece->getHeight() + extra;
	glPushMatrix();
	if(piece->mJoined == NULL) {
		glTranslatef(pos->mx, pos->my, pos->mz);
		glRotatef(piece->getRotateAngle(), 0.0f, 1.0f, 0.0f);
	} else {
		Coord *center = piece->mJoined->getCenter();
		glTranslatef(center->mx, center->my, center->mz);
		glRotatef(piece->mJoined->getRotateAngle(), 0.0f, 1.0f, 0.0f);
		glRotatef(piece->mJoined->getFlipAngleX(), 0.0f, 0.0f, 1.0f);
		glRotatef(piece->mJoined->getFlipAngleY(), 1.0f, 0.0f, 0.0f);
		glTranslatef(pos->mx - center->mx, 0.0f, pos->mz - center->mz);
	}
	glColor3f(r, g, b);
	glBegin(GL_QUADS);
	glVertex3f(-w, 0.0f, h);
	glVertex3f(w, 0.0f, h);
	glVertex3f(w, 0.0f, -h);
	glVertex3f(-w, 0.0f, -h);
	glEnd();
	glPopMatrix();
}

void drawHighlight() {
	if(piece != NULL) {
		if(piece->mJoined != NULL) {
			Joined *join = piece->mJoined;
			for(unsigned int i = 0; i < join->getNumPieces(); i++)
				drawHighlightRect(0.75f, 0.75f, 0.0f, join->getPiece(i));
		} else {
			drawHighlightRect(0.75f, 0.75f, 0.0f, piece);
		}
	}

	if(group != NULL) {
		for(unsigned int i = 0; i < group->getNumPieces(); i++) {
			Piece *p = group->getPiece(i);
			if(p->mJoined == NULL)
				drawHighlightRect(0.0f, 0.0f, 0.8f, group->getPiece(i));
			else {
				Joined *join = p->mJoined;
				for(unsigned int j = 0; j < join->getNumPieces(); j++)
					drawHighlightRect(0.0f, 0.0f, 0.8f, join->getPiece(j));
			}
		}
	}
}

void draw() {

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
	glColor3f(0.25f, 0.25f, 0.25f);
	glBegin(GL_QUADS);
		glVertex3f(-100.0f, 0.0f, -100.0f);
		glVertex3f(-100.0f, 0.0f,  100.0f);
		glVertex3f( 100.0f, 0.0f,  100.0f);
		glVertex3f( 100.0f, 0.0f, -100.0f);
	glEnd();
	drawHighlight();

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
	case 'f':
	case 'F':
		if(piece != NULL)
			piece->flipX();
		break;
	case 'd':
	case 'D':
		if(piece != NULL)
			piece->flipY();
		break;
	case 'r':
	case 'R':
		if(piece != NULL) {
			piece->rotate(-1.0f);
			puzzle->check(piece);
		}
		break;
	case 'e':
	case 'E':
		if(piece != NULL) {
			piece->rotate(1.0f);
			puzzle->check(piece);
		}
		break;
	case 'g':
	case 'G':
		group = NULL; // end the existing group
		break;
	case 's':
	case 'S':
		puzzle->solve(cx+dx, cy+dy, cz+dz);
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
	prev_x = x; prev_y = y;

	if(state == GLUT_DOWN) {
		int height = glutGet(GLUT_WINDOW_HEIGHT);
		GLuint piece_id;
		glReadPixels(x, height-y-1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &piece_id);
		if(piece != NULL && piece->getID() == (int)piece_id) {
			piece = NULL;
			return;
		}
		piece = puzzle->getPiece(piece_id);
		if(piece !=NULL && button == GLUT_RIGHT_BUTTON) {
			if(piece->mGroup == NULL) {
				if(group == NULL) {
					group = new Group(piece);
					puzzle->addGroup(group);
				} else {
					group->addPiece(piece);
				}
			} else {
				group = piece->mGroup;
				if(glutGetModifiers() == GLUT_ACTIVE_CTRL) {
					// prep to move so do nothing
				} else 
					group->fan();
			}
			piece = NULL;
		} else {
			move = button == GLUT_RIGHT_BUTTON;
			group = NULL;
		}
	}
}

void mouseMoved(int x, int y) {
	float fac = 0.001f * cy;
	float delta_x = fac * (x - prev_x); 
	float delta_z = fac * (y - prev_y);
	if(piece != NULL) {
		if(piece->mGroup != NULL) {
			Group *g = piece->mGroup;
			bool empty = g->removePiece(piece);
			if(empty)
				puzzle->deleteGroup(g);
		}
		piece->move(delta_x, 0.0f, delta_z);
		puzzle->check(piece);
		if(piece->mGroup == NULL) {
			group = puzzle->checkGroups(piece);
			if(group != NULL)
				piece = NULL;
		}
	} else if(group != NULL) {
		group->move(delta_x, 0.0f, delta_z);	
	} else {
		if(move) {
			cx+=-fraction * (x - prev_x);
			cz+=-fraction * (y - prev_y);
			
		} else {
			if(y - prev_y < 0)
				zoom = 0.01f * (y - prev_y);
			else
				zoom = 0.01f * (y - prev_y);
		}
	}
	prev_x = x; prev_y = y;
}

float norm(float dx, float dy) {
	return sqrt(dx * dx + dy * dy);
}

void passiveMouseMoved(int x, int y) {

	if(piece == NULL)
		return;
	
	float delta_x, delta_y;
	delta_x = (float)(x - prev_x);
	delta_y = (float)(y - prev_y);
	float len = norm(delta_x, delta_y);
	delta_x = delta_x / len; // normalized
	delta_y = delta_y / len; 

	float left = -1.0f * delta_x + 0.0f * delta_y;
	if(abs(left - 1.0f) < 1e-6f) {
		piece->rotate(10.0f);
		puzzle->check(piece);
	}
	float right = 1.0f * delta_x + 0.0f * delta_y;
	if(abs(right - 1.0f) < 1e-6f) {
		piece->rotate(-10.0f);
		puzzle->check(piece);
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
	puzzle = new Puzzle(EASY, 0);
	piece = NULL;

	// register callbacks
	glutDisplayFunc(draw);
	glutReshapeFunc(reshape);
	glutIdleFunc(draw);
	glutKeyboardFunc(keyPressed);
	glutSpecialFunc(specKeyPressed);
	glutMouseFunc(mousePressed);
	glutMotionFunc(mouseMoved);
	glutPassiveMotionFunc(passiveMouseMoved);

	// OpenGL init
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	// enter GLUT event processing cycle
	glutMainLoop();
	return 0;
}
