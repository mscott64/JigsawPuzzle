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
	mFlipAngleX = 0.0f;
	mFlipAngleY = 0.0f;
	mRotateAngle = 0.0f;
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
	if(mJoined == NULL)
		mRotateAngle += angle; 
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

	/*std::cout << "Pos " << pos[0] << "," << pos[1] << "," << pos[2] << "," << pos[3] << std::endl;
	std::cout << "Out " << out[0] << "," << out[1] << "," << out[2] << "," << out[3] << std::endl;

	std::cout << "Model" << std::endl;
	std::cout << model[0][0] << "," << model[1][0] << "," << model[2][0] << "," << model[3][0] << std::endl;
	std::cout << model[0][1] << "," << model[1][1] << "," << model[2][1] << "," << model[3][1] << std::endl;
	std::cout << model[0][2] << "," << model[1][2] << "," << model[2][2] << "," << model[3][2] << std::endl;
	std::cout << model[0][3] << "," << model[1][3] << "," << model[2][3] << "," << model[3][3] << std::endl;*/
	return new Coord(out[0], out[1], out[2]);
}