#include "Piece.h"
#include "Coord.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

const float def_size = 1.0f;
const float half_depth = 0.25f;

Piece::Piece(int id) {
	mId = id;
	mTruePos = new Coord();
	mPos = new Coord();
	mw = mh = def_size;
}

void Piece::draw(float r, float g, float b) {
	glStencilFunc(GL_ALWAYS, mId, -1);
	glColor3f(r, g, b);
	//ccw
	//Top
	glBegin(GL_QUADS);
	glVertex3f(mPos->mx-mw, mPos->my+half_depth, mPos->mz-mh);
	glVertex3f(mPos->mx-mw, mPos->my+half_depth, mPos->mz+mh);
	glVertex3f(mPos->mx+mw, mPos->my+half_depth, mPos->mz+mh);
	glVertex3f(mPos->mx+mw, mPos->my+half_depth, mPos->mz-mh);
	glEnd();

	//Bottom
	glBegin(GL_QUADS);
	glVertex3f(mPos->mx-mw, mPos->my-half_depth, mPos->mz-mh);
	glVertex3f(mPos->mx-mw, mPos->my-half_depth, mPos->mz+mh);
	glVertex3f(mPos->mx+mw, mPos->my-half_depth, mPos->mz+mh);
	glVertex3f(mPos->mx+mw, mPos->my-half_depth, mPos->mz-mh);
	glEnd();

	//Front
	glBegin(GL_QUADS);
	glVertex3f(mPos->mx-mw, mPos->my+half_depth, mPos->mz+mh);
	glVertex3f(mPos->mx-mw, mPos->my-half_depth, mPos->mz+mh);
	glVertex3f(mPos->mx+mw, mPos->my-half_depth, mPos->mz+mh);
	glVertex3f(mPos->mx+mw, mPos->my+half_depth, mPos->mz+mh);
	glEnd();

	//Back - ignoring because camera angle makes it impossible to see
	glBegin(GL_QUADS);
	glVertex3f(mPos->mx+mw, mPos->my+half_depth, mPos->mz-mh);
	glVertex3f(mPos->mx+mw, mPos->my-half_depth, mPos->mz-mh);
	glVertex3f(mPos->mx+mw, mPos->my-half_depth, mPos->mz-mh);
	glVertex3f(mPos->mx-mw, mPos->my+half_depth, mPos->mz-mh);
	glEnd();

	//Left
	glBegin(GL_QUADS);
	glVertex3f(mPos->mx-mw, mPos->my+half_depth, mPos->mz-mh);
	glVertex3f(mPos->mx-mw, mPos->my-half_depth, mPos->mz-mh);
	glVertex3f(mPos->mx-mw, mPos->my-half_depth, mPos->mz+mh);
	glVertex3f(mPos->mx-mw, mPos->my+half_depth, mPos->mz+mh);
	glEnd();

	//Right
	glBegin(GL_QUADS);
	glVertex3f(mPos->mx+mw, mPos->my+half_depth, mPos->mz+mh);
	glVertex3f(mPos->mx+mw, mPos->my-half_depth, mPos->mz+mh);
	glVertex3f(mPos->mx+mw, mPos->my-half_depth, mPos->mz-mh);
	glVertex3f(mPos->mx+mw, mPos->my+half_depth, mPos->mz-mh);
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