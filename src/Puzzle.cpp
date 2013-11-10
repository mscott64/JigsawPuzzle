#include "Puzzle.h"

Puzzle::Puzzle(int num_pieces) {
	mPieces.resize(num_pieces);
	for(int i=0; i < num_pieces; i++) {
		mPieces[i] = new Piece(i+1); // to match stencil buffer
		mPieces[i]->setPos(i-2.0f, 2.0f, i-2.0f);
		mPieces[i]->setSize(0.75f, 0.75f);
	}
}

void Puzzle::draw() {
	float c = 0.0f;
	float change = 1.0f / mPieces.size();;
	for(unsigned int i = 0; i < mPieces.size(); i++) {
		mPieces[i]->draw(c,c,c);
		c += change;
	}
}

Piece *Puzzle::getPiece(unsigned int id) {
	if(id <= 0 || id > mPieces.size()) 
		return NULL;

	return mPieces[id-1];
}