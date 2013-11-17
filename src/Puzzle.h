#ifndef PUZZLE_H
#define PUZZLE_H

#include "Piece.h"
#include <vector>
#define IMG_PATH "../images/"

enum {EASY = 7, MEDIUM = 10, HARD = 14};

class Puzzle {
public:
	Puzzle(int type);
	~Puzzle();
	void draw();
	Piece *getPiece(unsigned int id);
	bool check(Piece *p);
	static int LoadBMP(const char* location, unsigned int &texture);
private:
	void join(Piece *moving, Piece *fixed, int dir);
	bool connect(Piece *p);
	std::vector<Piece *> mPieces;
	unsigned int mTexture;
	float mPieceSize;
};

#endif /* Puzzle.h */