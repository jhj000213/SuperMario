#pragma once
#include "stdafx.h"

class Block;

class Flower : public Node
{
private:
	Sprite * mush;
	Layer * GLayer;

	bool moving;
	bool del;
	int Info;

	float charHigh;


	
	float gravitypower;

	Vector<Block*> nowTouchBlock;

	Vector<Block*> *bvv;
	Vector<Flower*> *mushv;

public:
	void m_init(Vec2 Pos, Layer * layer, Vector<Block*> *v, Vector<Flower*> *mv);
	void m_update(float dt, float MapPosX);
	void removethis();
	Rect m_getRect();
	void m_setVisivle();

	static Flower * m_create(Vec2 Pos, Layer * layer, Vector<Block*> *v, Vector<Flower*> *mv)
	{
		auto ref = new Flower();
		ref->m_init(Pos, layer, v, mv);
		ref->autorelease();

		return ref;
	}
};