#include "Puzzle.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <iostream> // TODO: get rid of this
#include "Joined.h"
#define IMG_PATH "../images/"

typedef unsigned char Uint8;
const char *img_nm = "castle.bmp";//"ga_tech.bmp";//"atl_falcons.bmp";
const float eps = 0.04f;

int LoadBMP(const char* location, GLuint &texture) {
	Uint8* datBuff[2] = {nullptr, nullptr}; // Header buffers
	Uint8* pixels = nullptr;

	BITMAPFILEHEADER* bmpHeader = nullptr; // Header
	BITMAPINFOHEADER* bmpInfo   = nullptr; // Info 

	// The file... We open it with it's constructor
	std::ifstream file(location, std::ios::binary);
	if(!file)
	{
		std::cout << "Failure to open bitmap file.\n";
		return 1;
	}

	// Allocate byte memory that will hold the two headers
	datBuff[0] = new Uint8[sizeof(BITMAPFILEHEADER)];
	datBuff[1] = new Uint8[sizeof(BITMAPINFOHEADER)];

	file.read((char*)datBuff[0], sizeof(BITMAPFILEHEADER));
	file.read((char*)datBuff[1], sizeof(BITMAPINFOHEADER));

	// Construct the values from the buffers
	bmpHeader = (BITMAPFILEHEADER*) datBuff[0];
	bmpInfo   = (BITMAPINFOHEADER*) datBuff[1];

	// First allocate pixel memory
	pixels = new Uint8[bmpInfo->biSizeImage];

	// Go to where image data starts, then read in image data
	file.seekg(bmpHeader->bfOffBits);
	file.read((char*)pixels, bmpInfo->biSizeImage);

	// We're almost done. We have our image loaded, however it's not in the right format.
	// .bmp files store image data in the BGR format, and we have to convert it to RGB.
	// Since we have the value in bytes, this shouldn't be to hard to accomplish
	Uint8 tmpRGB = 0; // Swap buffer
	for (unsigned long i = 0; i < bmpInfo->biSizeImage; i += 3)
	{
		tmpRGB        = pixels[i];
		pixels[i]     = pixels[i + 2];
		pixels[i + 2] = tmpRGB;
	}

	// Set width and height to the values loaded from the file
	GLuint w = bmpInfo->biWidth;
	GLuint h = bmpInfo->biHeight;

	/*******************GENERATING TEXTURES*******************/

	glGenTextures(1, &texture);             // Generate a texture
	glBindTexture(GL_TEXTURE_2D, texture); // Bind that texture temporarily

	GLint mode = GL_RGB;                   // Set the mode

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	 
	// Create the texture. We get the offsets from the image, then we use it with the image's
	// pixel data to create it.
	glTexImage2D(GL_TEXTURE_2D, 0, mode, w, h, 0, mode, GL_UNSIGNED_BYTE, pixels);

	// Unbind the texture
	glBindTexture(GL_TEXTURE_2D, NULL);

	// Output a successful message
	std::cout << "Texture \"" << location << "\" successfully loaded.\n";

	// Delete the two buffers.
	delete[] datBuff[0];
	delete[] datBuff[1];
	delete[] pixels;

	return 0; // Return success code 
}

Puzzle::Puzzle(int num_pieces) {
	const float space = 0.1f;
	int piece_count = num_pieces * num_pieces;
	mPieces.resize(piece_count);
	int index = 0;
	mPieceSize = 5.0f / num_pieces;
	float start = -num_pieces * (mPieceSize + space) / 2.0f;
	float tex_sz = 1.0f / num_pieces;
	for(int i = 0; i < num_pieces; i++) {
		for(int j = 0; j < num_pieces; j++) {
			index = i * num_pieces + j;
			mPieces[index] = new Piece(index+1); // to match stencil buffer
			mPieces[index]->setPos(start + i*(mPieceSize + space), 1.0f, start + j*(mPieceSize + space));
			mPieces[index]->setSize(mPieceSize, mPieceSize);
			mPieces[index]->setTextureBounds(i*tex_sz, 1.0f-(j+1)*tex_sz, (i+1)*tex_sz, 1.0f-j*tex_sz);
		}
	}

	for(int i = 0; i < num_pieces; i++) {
		for(int j = 0; j < num_pieces; j++) {
			int index = i * num_pieces + j;
			Piece *p = mPieces[index];
			int u_n = index - 1;
			int d_n = index + 1;
			int l_n = index - num_pieces;
			int r_n = index + num_pieces;

			if(u_n >= 0) {
				p->addNeighbor(mPieces[u_n]);
				p->addDirection(ABOVE);
			}
			if(d_n < piece_count) {
				p->addNeighbor(mPieces[d_n]);
				p->addDirection(BELOW);
			}
			if(l_n >= 0) {
				p->addNeighbor(mPieces[l_n]);
				p->addDirection(LEFT);
			}
			if(r_n < piece_count) {
				p->addNeighbor(mPieces[r_n]);
				p->addDirection(RIGHT);
			}
		}
	}

	char full_img_nm[50];
	sprintf_s(full_img_nm, "%s%s", IMG_PATH, img_nm);
	int e = LoadBMP(full_img_nm, mTexture);
	if(e != 0) {
		std::cout << "Error loading texture" << std::endl;
		exit(1);
	}
}

Puzzle::~Puzzle() {
	for(unsigned int i = 0; i < mPieces.size(); i++)
		delete mPieces[i];
}

void Puzzle::draw() {
	float c = 0.0f;
	float change = 1.0f / mPieces.size();;
	
	glBindTexture(GL_TEXTURE_2D, mTexture);
	for(unsigned int i = 0; i < mPieces.size(); i++) {
		mPieces[i]->draw();
		c += change;
	}
	glBindTexture(GL_TEXTURE_2D, NULL);
}

Piece *Puzzle::getPiece(unsigned int id) {
	if(id <= 0 || id > mPieces.size()) 
		return NULL;

	return mPieces[id-1];
}

bool Puzzle::check(Piece *p) {
	bool ret = false;
	if(connect(p))
		ret = true;
	
	if(p->mJoined == NULL)
		return ret;

	for(unsigned int i = 0; i < p->mJoined->getNumPieces(); i++)
		if(connect(p->mJoined->getPiece(i)))
			ret = true;

	return ret;
}

bool Puzzle::connect(Piece *p) {
	int dir;
	float dx, dy;
	Piece *neighbor;
	int id = p->getID();
	bool ret = false;
	for(unsigned int i = 0; i < p->getNumNeighbors(); i++) {
		neighbor = p->getNeighbor(i);
		dir = p->getDirection(i);
		dx = neighbor->getPos()->mx - p->getPos()->mx;
		dy = neighbor->getPos()->mz - p->getPos()->mz;
		switch(dir) {
		case BELOW: 
			if(abs(dx) < eps && abs(dy) < eps + mPieceSize && dy >= 0) {
				if(!ret)
					join(p, neighbor, BELOW);
				p->removeNeighbor(i);
				neighbor->removeNeighbor(p);
				ret = true;
			}
			break;
		case ABOVE: 
			if(abs(dx) < eps && abs(dy) < eps + mPieceSize && dy <= 0) {
				if(!ret)
					join(p, neighbor, ABOVE);
				p->removeNeighbor(i);
				neighbor->removeNeighbor(p);
				ret = true;
			}
			break;
		case RIGHT:
			if(abs(dx) < eps + mPieceSize && abs(dy) < eps && dx >= 0) {
				if(!ret)
					join(p, neighbor, RIGHT);
				p->removeNeighbor(i);
				neighbor->removeNeighbor(p);
				ret = true;
			}
			break;
		case LEFT:
			if(abs(dx) < eps + mPieceSize && abs(dy) < eps && dx <= 0) {
				if(!ret)
					join(p, neighbor, LEFT);
				p->removeNeighbor(i);
				neighbor->removeNeighbor(p);
				ret = true;
			}
			break;
		}
	}
	return ret;
}

void Puzzle::join(Piece *moving, Piece *fixed, int dir) {

	float dx, dy, dz;
	switch(dir) {
	case ABOVE:
		dx = fixed->getPos()->mx - moving->getPos()->mx;
		dy = fixed->getPos()->my - moving->getPos()->my;
		dz = fixed->getPos()->mz + mPieceSize - moving->getPos()->mz;
		break;
	case BELOW:
		dx = fixed->getPos()->mx - moving->getPos()->mx;
		dy = fixed->getPos()->my - moving->getPos()->my;
		dz = fixed->getPos()->mz - mPieceSize - moving->getPos()->mz;
		break;
	case LEFT:
		dx = fixed->getPos()->mx + mPieceSize - moving->getPos()->mx;
		dy = fixed->getPos()->my - moving->getPos()->my;
		dz = fixed->getPos()->mz - moving->getPos()->mz;
		break;
	case RIGHT:
		dx = fixed->getPos()->mx - mPieceSize - moving->getPos()->mx;
		dy = fixed->getPos()->my - moving->getPos()->my;
		dz = fixed->getPos()->mz - moving->getPos()->mz;
		break;
	}
	moving->move(dx, dy, dz);
	if(moving->mJoined == NULL && fixed->mJoined == NULL) { // neither is attached
		moving->mJoined = new Joined(moving);
		moving->mJoined->addPiece(fixed);
		fixed->mJoined = moving->mJoined;
	} else if(moving->mJoined != NULL && fixed->mJoined == NULL) { // moving is attached
		moving->mJoined->addPiece(fixed);
		fixed->mJoined = moving->mJoined;
	} else if(moving->mJoined == NULL && fixed->mJoined != NULL) { // fixed is attached
		fixed->mJoined->addPiece(moving);
		moving->mJoined = fixed->mJoined;
	} else if(moving->mJoined != fixed->mJoined) { // both are attached
		Piece *p;
		std::vector<Piece *> pieces = moving->mJoined->getPieces();
		for(unsigned int i = 0; i < pieces.size(); i++) {
			p = pieces[i];
			fixed->mJoined->addPiece(p);
			p->mJoined = fixed->mJoined;
		}
	}
}