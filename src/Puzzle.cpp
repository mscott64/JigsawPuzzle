#include "Puzzle.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <iostream> // TODO: get rid of this
#define IMG_PATH "../images/"

typedef unsigned char Uint8;
const char *img_nm = "ga_tech.bmp";//"atl_falcons.bmp";

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
	mPieces.resize(num_pieces * num_pieces);
	int index = 0;
	float piece_sz = 5.0f / num_pieces;
	float start = -num_pieces * (piece_sz + space) / 2.0f;
	float tex_sz = 1.0f / num_pieces;
	for(int i = 0; i < num_pieces; i++) {
		for(int j = 0; j < num_pieces; j++) {
			index = i * num_pieces + j;
			mPieces[index] = new Piece(index+1); // to match stencil buffer
			mPieces[index]->setPos(start + i*(piece_sz + space), 1.0f, start + j*(piece_sz + space));
			mPieces[index]->setSize(piece_sz, piece_sz);
			mPieces[index]->setTextureBounds(i*tex_sz, 1.0f-(j+1)*tex_sz, (i+1)*tex_sz, 1.0f-j*tex_sz);
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