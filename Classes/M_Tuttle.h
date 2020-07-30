#pragma once
#include "stdafx.h"

class Block;

class M_Tuttle : public Node
{
private:
	Sprite * mush;
	Layer * GLayer;

	bool moving;


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
	float MoveSpeed = 0.7;
	float gravitypower;

	Vector<Block*> nowTouchBlock;

	Vector<Block*> *bvv;
	Vector<M_Tuttle*> *mushv;

public:
	bool del;
	bool burn;

	void m_init(Vec2 Pos, Layer * layer, Vector<Block*> *v, Vector<M_Tuttle*> *mv);
	void m_update(float dt, float MapPosX);
	void m_setMoving();
	void removethis();
	Rect m_getRect();
	void m_setVisivle();

	void m_dead();

	bool BottomCollisionCheck(Vec2 HPos);
	bool LeftMoveCollisionCheck(Vec2 HPos, float MapPosX);
	bool RightMoveCollisionCheck(Vec2 HPos, float MapPosX);




	static M_Tuttle * m_create(Vec2 Pos, Layer * layer, Vector<Block*> *v, Vector<M_Tuttle*> *mv)
	{
		auto ref = new M_Tuttle();
		ref->m_init(Pos, layer, v, mv);
		ref->autorelease();

		return ref;
	}
};