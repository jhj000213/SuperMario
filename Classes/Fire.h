#pragma once
#include "stdafx.h"


class Fire : public Node
{
private:
	Sprite * mario;
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
	float MoveSpeed = 3.0;
	float gravitypower;

	Vector<Block*> nowTouchBlock;

	Vector<Block*> *bv;
	Vector<Fire*> *sv;
public:
	bool del;
	bool out;
	void m_init(Vec2 Pos, Layer * layer,bool left, Vector<Block*> *v);
	void m_jump();
	void m_setMoving();
	void removethis();
	Vec2 m_update(float dt, float MapPosX);
	Rect m_getRect();
	void m_setVisivle();

	void m_boom();

	bool BottomCollisionCheck(Vec2 HPos);
	bool TopCollisionCheck(Vec2 HPos);
	bool LeftMoveCollisionCheck(Vec2 HPos, float MapPosX);
	bool RightMoveCollisionCheck(Vec2 HPos, float MapPosX);




	static Fire * m_create(Vec2 Pos, Layer * layer, bool left, Vector<Block*> *v)
	{
		auto ref = new Fire();
		ref->m_init(Pos, layer,left, v);
		ref->autorelease();

		return ref;
	}
};