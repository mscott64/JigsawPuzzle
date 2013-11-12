#ifndef PUZZLE_H
#define PUZZLE_H

#include "Piece.h"
#include <vector>

class Puzzle {
public:
	Puzzle(int num_pieces);
	void draw();
	Piece *getPiece(unsigned int id);
private:
	std::vector<Piece *> mPieces;
	unsigned int mTexture;
};

#endif /* Puzzle.h */