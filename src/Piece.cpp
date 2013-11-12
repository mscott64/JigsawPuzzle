#include "Piece.h"
#include "Coord.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

const float def_size = 1.0f;
const float half_depth = 0.125f;

Piece::Piece(int id) {
	mId = id;
	mTruePos = new Coord();
	mPos = new Coord();
	mw = mh = def_size;
}

void Piece::draw() {
	glStencilFunc(GL_ALWAYS, mId, -1);
	
	//ccw
	//Top
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);glVertex3f(mPos->mx-mw, mPos->my+half_depth, mPos->mz-mh);
	glTexCoord2f(0.0f, 0.0f);glVertex3f(mPos->mx-mw, mPos->my+half_depth, mPos->mz+mh);
	glTexCoord2f(1.0f, 0.0f);glVertex3f(mPos->mx+mw, mPos->my+half_depth, mPos->mz+mh);
	glTexCoord2f(1.0f, 1.0f);glVertex3f(mPos->mx+mw, mPos->my+half_depth, mPos->mz-mh);
	glEnd();
	
	//Bottom - ignoring because cannot be seen currently
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);glVertex3f(mPos->mx-mw, mPos->my-half_depth, mPos->mz-mh);
	glTexCoord2f(1.0f, 1.0f);glVertex3f(mPos->mx+mw, mPos->my-half_depth, mPos->mz-mh);
	glTexCoord2f(1.0f, 0.0f);glVertex3f(mPos->mx+mw, mPos->my-half_depth, mPos->mz+mh);
	glTexCoord2f(0.0f, 0.0f);glVertex3f(mPos->mx-mw, mPos->my-half_depth, mPos->mz+mh);
	glEnd();

	//Front
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);glVertex3f(mPos->mx-mw, mPos->my+half_depth, mPos->mz+mh);
	glTexCoord2f(0.0f, 0.0f);glVertex3f(mPos->mx-mw, mPos->my-half_depth, mPos->mz+mh);
	glTexCoord2f(1.0f, 0.0f);glVertex3f(mPos->mx+mw, mPos->my-half_depth, mPos->mz+mh);
	glTexCoord2f(1.0f, 0.0f);glVertex3f(mPos->mx+mw, mPos->my+half_depth, mPos->mz+mh);
	glEnd();

	//Back 
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f);glVertex3f(mPos->mx+mw, mPos->my+half_depth, mPos->mz-mh);
	glTexCoord2f(1.0f, 1.0f);glVertex3f(mPos->mx+mw, mPos->my-half_depth, mPos->mz-mh);
	glTexCoord2f(0.0f, 1.0f);glVertex3f(mPos->mx-mw, mPos->my-half_depth, mPos->mz-mh);
	glTexCoord2f(0.0f, 1.0f);glVertex3f(mPos->mx-mw, mPos->my+half_depth, mPos->mz-mh);
	glEnd();

	//Left
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);glVertex3f(mPos->mx-mw, mPos->my+half_depth, mPos->mz-mh);
	glTexCoord2f(0.0f, 1.0f);glVertex3f(mPos->mx-mw, mPos->my-half_depth, mPos->mz-mh);
	glTexCoord2f(0.0f, 0.0f);glVertex3f(mPos->mx-mw, mPos->my-half_depth, mPos->mz+mh);
	glTexCoord2f(0.0f, 0.0f);glVertex3f(mPos->mx-mw, mPos->my+half_depth, mPos->mz+mh);
	glEnd();

	//Right
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f);glVertex3f(mPos->mx+mw, mPos->my+half_depth, mPos->mz+mh);
	glTexCoord2f(1.0f, 0.0f);glVertex3f(mPos->mx+mw, mPos->my-half_depth, mPos->mz+mh);
	glTexCoord2f(1.0f, 1.0f);glVertex3f(mPos->mx+mw, mPos->my-half_depth, mPos->mz-mh);
	glTexCoord2f(1.0f, 1.0f);glVertex3f(mPos->mx+mw, mPos->my+half_depth, mPos->mz-mh);
	glEnd();
}

void Piece::setTruePos(float x, float y, float z) {
	mTruePos->setCoord(x, y, z);
}

void Piece::setPos(float x, float y, float z) {
	mPos->setCoord(x, y, z);
}

void Piece::move(float dx, float dy, float dz) {
	mPos->mx += dx;
	mPos->my += dy;
	mPos->mz += dz;
}