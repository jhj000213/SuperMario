#pragma once
#include "stdafx.h"

class Block;

class Mush : public Node
{
private:
	Sprite * mush;
	Layer * GLayer;

	bool moving;
	
	int Info;

	float charHigh;

	float movePower;

	bool ActionTouch = false;
	bool hero_leftmove = false;
	bool hero_rightmove = false;
	bool hero_leftsee = false;//왼쪽보고시작
	bool nowjump = false;
	bool touchjumpbutton1 = false;
	float	JumpForce;
	float	JumpTime;
	Vec2 heroPos;
	Vec2 somePos;
	bool Collision;
	float MoveSpeed = 1.0;
	float gravitypower;

	Vector<Block*> nowTouchBlock;

	Vector<Block*> *bvv;
	Vector<Mush*> *mushv;

public:
	bool del;

	void m_init(Vec2 Pos, Layer * layer, int info, Vector<Block*> *v, Vector<Mush*> *mv);
	void m_update(float dt, float MapPosX);
	void m_setMoving();
	void removethis();
	Rect m_getRect();
	int m_getInfo();
	void m_setVisivle();

	bool BottomCollisionCheck(Vec2 HPos);
	bool LeftMoveCollisionCheck(Vec2 HPos, float MapPosX);
	bool RightMoveCollisionCheck(Vec2 HPos, float MapPosX);




	static Mush * m_create(Vec2 Pos, Layer * layer, int info, Vector<Block*> *v, Vector<Mush*> *mv)
	{
		auto ref = new Mush();
		ref->m_init(Pos, layer,info, v,mv);
		ref->autorelease();

		return ref;
	}
};