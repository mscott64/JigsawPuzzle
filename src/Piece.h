#ifndef PIECE_H
#define PIECE_H

class Coord;

class Piece {
public:
	Piece(int id);
	~Piece();
	void draw();
	void setPos(float x, float y, float z);
	void setTextureBounds(float lx, float ly, float hx, float hy);
	Coord *getPos() { return mPos; };
	void setSize(float w, float h) { mw = w/2.0f; mh = h/2.0f; }
	float getWidth() { return mw * 2.0f; }
	float getHeight() { return mh * 2.0f; }
	int getID() { return mId; }
	void move(float dx, float dy, float dz);

private:
	int mId;
	float mw, mh; // half of width and height
	Coord *mPos;
	Coord *mLow, *mHigh;
};

#endif /* Piece.h */