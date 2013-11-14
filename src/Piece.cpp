#include "Piece.h"
#include "Coord.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

const float def_size = 1.0f;
const float half_depth = 0.015625f;

Piece::Piece(int id) {
	mId = id;
	mPos = new Coord();
	mLow = new Coord();
	mHigh = new Coord();
	mw = mh = def_size;
}

Piece::~Piece() {
	delete mPos;
	delete mLow;
	delete mHigh;
}

void Piece::draw() {
	glStencilFunc(GL_ALWAYS, mId, -1);
	
	//ccw
	//Top
	glBegin(GL_QUADS);
	glTexCoord2f(mLow->mx, mLow->my);glVertex3f(mPos->mx-mw, mPos->my+half_depth, mPos->mz+mh);
	glTexCoord2f(mHigh->mx, mLow->my);glVertex3f(mPos->mx+mw, mPos->my+half_depth, mPos->mz+mh);
	glTexCoord2f(mHigh->mx, mHigh->my);glVertex3f(mPos->mx+mw, mPos->my+half_depth, mPos->mz-mh);
	glTexCoord2f(mLow->mx, mHigh->my);glVertex3f(mPos->mx-mw, mPos->my+half_depth, mPos->mz-mh);

	//Bottom
	glTexCoord2f(mLow->mx, mLow->my);glVertex3f(mPos->mx-mw, mPos->my-half_depth, mPos->mz+mh);
	glTexCoord2f(mHigh->mx, mLow->my);glVertex3f(mPos->mx+mw, mPos->my-half_depth, mPos->mz+mh);
	glTexCoord2f(mHigh->mx, mHigh->my);glVertex3f(mPos->mx+mw, mPos->my-half_depth, mPos->mz-mh);
	glTexCoord2f(mLow->mx, mHigh->my);glVertex3f(mPos->mx-mw, mPos->my-half_depth, mPos->mz-mh);

	//Front
	glTexCoord2f(mLow->mx, mLow->my);glVertex3f(mPos->mx-mw, mPos->my-half_depth, mPos->mz+mh);
	glTexCoord2f(mHigh->mx, mLow->my);glVertex3f(mPos->mx+mw, mPos->my-half_depth, mPos->mz+mh);
	glTexCoord2f(mHigh->mx, mLow->my);glVertex3f(mPos->mx+mw, mPos->my+half_depth, mPos->mz+mh);
	glTexCoord2f(mLow->mx, mLow->my);glVertex3f(mPos->mx-mw, mPos->my+half_depth, mPos->mz+mh);

	//Back 
	glTexCoord2f(mLow->mx, mHigh->my);glVertex3f(mPos->mx-mw, mPos->my-half_depth, mPos->mz-mh);
	glTexCoord2f(mHigh->mx, mHigh->my);glVertex3f(mPos->mx+mw, mPos->my-half_depth, mPos->mz-mh);
	glTexCoord2f(mHigh->mx, mHigh->my);glVertex3f(mPos->mx+mw, mPos->my+half_depth, mPos->mz-mh);
	glTexCoord2f(mLow->mx, mHigh->my);glVertex3f(mPos->mx-mw, mPos->my+half_depth, mPos->mz-mh);

	//Left
	glTexCoord2f(mLow->mx, mHigh->my);glVertex3f(mPos->mx-mw, mPos->my-half_depth, mPos->mz-mh);
	glTexCoord2f(mLow->mx, mLow->my);glVertex3f(mPos->mx-mw, mPos->my-half_depth, mPos->mz+mh);
	glTexCoord2f(mLow->mx, mLow->my);glVertex3f(mPos->mx-mw, mPos->my+half_depth, mPos->mz+mh);
	glTexCoord2f(mLow->mx, mHigh->my);glVertex3f(mPos->mx-mw, mPos->my+half_depth, mPos->mz-mh);

	//Right
	glTexCoord2f(mHigh->mx, mLow->my);glVertex3f(mPos->mx+mw, mPos->my-half_depth, mPos->mz+mh);
	glTexCoord2f(mHigh->mx, mHigh->my);glVertex3f(mPos->mx+mw, mPos->my-half_depth, mPos->mz-mh);
	glTexCoord2f(mHigh->mx, mHigh->my);glVertex3f(mPos->mx+mw, mPos->my+half_depth, mPos->mz-mh);
	glTexCoord2f(mHigh->mx, mLow->my);glVertex3f(mPos->mx+mw, mPos->my+half_depth, mPos->mz+mh);
	glEnd();
}

void Piece::setPos(float x, float y, float z) {
	mPos->setCoord(x, y, z);
}

void Piece::move(float dx, float dy, float dz) {
	mPos->mx += dx;
	mPos->my += dy;
	mPos->mz += dz;
}

void Piece::setTextureBounds(float lx, float ly, float hx, float hy) {
	mLow->mx = lx;
	mLow->my = ly;
	mHigh->mx = hx;
	mHigh->my = hy;
}