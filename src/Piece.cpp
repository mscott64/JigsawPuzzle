#include "Piece.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Joined.h"
#include <iostream> // TODO: get rid of this

const float def_size = 1.0f;
const float half_depth = 0.015625f;

Piece::Piece(int id) {
	mId = id;
	mPos = new Coord();
	mLow = new Coord();
	mHigh = new Coord();
	mw = mh = def_size;
	mJoined = NULL;
	mGroup = NULL;
	mFlipAngleX = 0.0f;
	mFlipAngleY = 0.0f;
	mRotateAngle = 0.0f;
	//Animation
	inMotion = false;
	mFrame = 0;
	d_x = d_y = d_z = 0;
}

Piece::~Piece() {
	delete mPos;
	delete mLow;
	delete mHigh;
}

float Piece::getRotateAngle() {
	if(mJoined == NULL)
		return mRotateAngle;
	return mJoined->getRotateAngle();
}

float Piece::getFlipAngleX() {
	if(mJoined == NULL)
		return mFlipAngleX;
	return mJoined->getFlipAngleX();
}

float Piece::getFlipAngleY() {
	if(mJoined == NULL)
		return mFlipAngleY;
	return mJoined->getFlipAngleY();
}

void Piece::draw() {
	const int frames = 100;
	const float small = 0.1f / frames; 
	const float big = 0.8f / frames;
	if(inMotion) {
		if(mFrame < frames) { // 0.1 -> 100
			move(small*d_x, small*d_y, small*d_z);
		} else if(mFrame < 2 * frames) { // 0.8 -> 100
			move(big*d_x, big*d_y, big*d_z);
		} else if(mFrame < 3 * frames) { // 0.1 -> 100
			move(small*d_x, small*d_y, small*d_z);
		}
		if(mFrame == 3 * frames)
			inMotion = false;
		mFrame++;
	}

	glStencilFunc(GL_ALWAYS, mId, -1);
	glPushMatrix();
	if(mJoined == NULL) {
		glTranslatef(mPos->mx, mPos->my, mPos->mz);
		glRotatef(mRotateAngle, 0.0f, 1.0f, 0.0f);
		glRotatef(mFlipAngleX, 0.0f, 0.0f, 1.0f);
		glRotatef(mFlipAngleY, 1.0f, 0.0f, 0.0f);
	} else {
		Coord *center = mJoined->getCenter();
		glTranslatef(center->mx, center->my, center->mz);
		glRotatef(mJoined->getRotateAngle(), 0.0f, 1.0f, 0.0f);
		glRotatef(mJoined->getFlipAngleX(), 0.0f, 0.0f, 1.0f);
		glRotatef(mJoined->getFlipAngleY(), 1.0f, 0.0f, 0.0f);
		glTranslatef(mPos->mx - center->mx, 0.0f, mPos->mz - center->mz);
	}
	//ccw
	//Top
	glBegin(GL_QUADS);
	glTexCoord2f(mLow->mx, mLow->my);glVertex3f(-mw, half_depth, mh);
	glTexCoord2f(mHigh->mx, mLow->my);glVertex3f(mw, half_depth, mh);
	glTexCoord2f(mHigh->mx, mHigh->my);glVertex3f(mw, half_depth, -mh);
	glTexCoord2f(mLow->mx, mHigh->my);glVertex3f(-mw, half_depth, -mh);

	//Bottom
	glTexCoord2f(mLow->mx, mLow->my);glVertex3f(-mw, -half_depth, mh);
	glTexCoord2f(mHigh->mx, mLow->my);glVertex3f(mw, -half_depth, mh);
	glTexCoord2f(mHigh->mx, mHigh->my);glVertex3f(mw, -half_depth, -mh);
	glTexCoord2f(mLow->mx, mHigh->my);glVertex3f(-mw, -half_depth, -mh);

	//Front
	glTexCoord2f(mLow->mx, mLow->my);glVertex3f(-mw, -half_depth, mh);
	glTexCoord2f(mHigh->mx, mLow->my);glVertex3f(mw, -half_depth, mh);
	glTexCoord2f(mHigh->mx, mLow->my);glVertex3f(mw, half_depth, mh);
	glTexCoord2f(mLow->mx, mLow->my);glVertex3f(-mw, half_depth, mh);

	//Back 
	glTexCoord2f(mLow->mx, mHigh->my);glVertex3f(-mw, -half_depth, -mh);
	glTexCoord2f(mHigh->mx, mHigh->my);glVertex3f(mw, -half_depth, -mh);
	glTexCoord2f(mHigh->mx, mHigh->my);glVertex3f(mw, half_depth, -mh);
	glTexCoord2f(mLow->mx, mHigh->my);glVertex3f(-mw, half_depth, -mh);

	//Left
	glTexCoord2f(mLow->mx, mHigh->my);glVertex3f(-mw, -half_depth, -mh);
	glTexCoord2f(mLow->mx, mLow->my);glVertex3f(-mw, -half_depth, mh);
	glTexCoord2f(mLow->mx, mLow->my);glVertex3f(-mw, half_depth, mh);
	glTexCoord2f(mLow->mx, mHigh->my);glVertex3f(-mw, half_depth, -mh);

	//Right
	glTexCoord2f(mHigh->mx, mLow->my);glVertex3f(mw, -half_depth, mh);
	glTexCoord2f(mHigh->mx, mHigh->my);glVertex3f(mw, -half_depth, -mh);
	glTexCoord2f(mHigh->mx, mHigh->my);glVertex3f(mw, half_depth, -mh);
	glTexCoord2f(mHigh->mx, mLow->my);glVertex3f(mw, half_depth, mh);
	glEnd();
	glPopMatrix();
}

void Piece::setPos(float x, float y, float z) {
	mPos->setCoord(x, y, z);
}

void Piece::move(float dx, float dy, float dz) {
	mPos->mx += dx;
	mPos->my += dy;
	mPos->mz += dz;

	if(mJoined == NULL)
		return;
	// move all attached pieces
	for(unsigned int i = 0; i < mJoined->getNumPieces(); i++) {
		Piece *p = mJoined->getPiece(i);
		if(p == this)
			continue;
		p->mPos->mx += dx;
		p->mPos->my += dy;
		p->mPos->mz += dz;
	}
	mJoined->computeCenter();
}

void Piece::moveAnimated(float dx, float dy, float dz) {
	inMotion = true;
	d_x = dx; d_y = dy; d_z = dz;
	mFrame = 0;
}

void Piece::setTextureBounds(float lx, float ly, float hx, float hy) {
	mLow->mx = lx;
	mLow->my = ly;
	mHigh->mx = hx;
	mHigh->my = hy;
}

void Piece::removeNeighbor(Piece *p) {
	for(unsigned int i = 0; i < mNeighbors.size(); i++) {
		if(mNeighbors[i] == p) {
			removeNeighbor(i);
			break;
		}
	}
}

void Piece::flipX() {
	if(mJoined == NULL) { // single piece
		if(mFlipAngleX > 0.0f)
			mFlipAngleX -= 180.0f;
		else
			mFlipAngleX += 180.0f;
	} else {
		mJoined->flipX();
	}
}

void Piece::flipY() {
	if(mJoined == NULL) { // single piece
		if(mFlipAngleY > 0.0f)
			mFlipAngleY -= 180.0f;
		else
			mFlipAngleY += 180.0f;
	} else {
		mJoined->flipY();
	}
}

void Piece::rotate(float angle) { 
	if(mJoined == NULL) {
		mRotateAngle += angle; 
		// Keep angles in range [0, 360)
		if(mRotateAngle >= 360.0f)
			mRotateAngle -= 360.0f;
		if(mRotateAngle < 0)
			mRotateAngle += 360.0f;
	}
	else { // rotate about the center
		mJoined->rotate(angle);
	}
}

Coord *Piece::getPos(bool compute) { 
	if(mJoined == NULL || !compute)
		return mPos; 
	Coord *center = mJoined->getCenter();
	glm::vec4 pos = glm::vec4(0.0, 0.0, 0.0, 1.0);
	glm::mat4 model = glm::translate(glm::mat4(1.0), glm::vec3(center->mx, center->my, center->mz));
	model = glm::rotate(model, mJoined->getRotateAngle(), glm::vec3(0.0, 1.0, 0.0));
	model = glm::rotate(model, mJoined->getFlipAngleX(), glm::vec3(0.0, 0.0, 1.0));
	model = glm::rotate(model, mJoined->getFlipAngleY(), glm::vec3(1.0, 0.0, 0.0));
	model = glm::translate(model, glm::vec3(mPos->mx - center->mx, 0.0, mPos->mz - center->mz));
	glm::vec4 out = model * pos;
	return new Coord(out[0], out[1], out[2]);
}