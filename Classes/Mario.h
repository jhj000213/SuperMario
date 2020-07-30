#pragma once
#include "stdafx.h"

class Mario : public Node
{
private:
	Sprite * mario;
	Layer * GLayer;
	
	bool actioning;
	bool Clear;
	bool light;
	float stardelay;
	float starTime;
	bool dead;
	float strongTime;
	bool pressshoot;
	float m_shootDelay;
	float m_time;
	int nowFrame;
	bool changeFrame;

	int Info;
	float charHigh;
	float movePower;
	bool ActionTouch = false;
	bool hero_leftmove = false;
	bool hero_rightmove = false;
	bool hero_leftsee = false;//왼쪽보고시작
	
	bool touchjumpbutton1 = false;
	float	JumpForce;
	float	JumpTime;
	Vec2 heroPos;
	Vec2 somePos;
	bool Collision;
	float MoveSpeed = 2.5;
	float gravitypower;

	Vector<Block*> nowTouchBlock;

	Vector<Block*> *bv;
	Vector<Fire*> *firev;
public:
	bool strong;
	bool star;
	bool falling;
	bool nowjump = false;

	void m_init(Vec2 Pos, Layer * layer, Vector<Block*> *v, Vector<Fire*> *fv);
	void m_jump();
	void m_releaseJump();
	void m_touchLeft();
	void m_touchRight();
	void m_touchShoot();
	void m_releaseLeft();
	void m_releaseRight();
	int m_getInfo();
	Vec2 m_update(float dt, float MapPosX);
	Rect m_getRect();
	Rect m_getFootRect();
	void m_UPgrade();
	void m_Downgrade();
	void m_killjump();

	void ChangeSprite();

	void m_setSprite1();
	void m_setSprite2();
	void m_setSprite3();
	void m_setSprite4();
	void m_setStrong();

	void m_ClearAnima();
	void m_setCA();
	void m_clearUpdate(float dt);

	void m_star();

	void m_shoot();
	void m_dead();
	void end();

	bool BottomCollisionCheck(Vec2 HPos);
	bool TopCollisionCheck(Vec2 HPos);
	bool LeftMoveCollisionCheck(Vec2 HPos, float MapPosX);
	bool RightMoveCollisionCheck(Vec2 HPos);




	static Mario * m_create(Vec2 Pos, Layer * layer, Vector<Block*> *v, Vector<Fire*> *fv)
	{
		auto ref = new Mario();
		ref->m_init(Pos, layer,v,fv);
		ref->autorelease();

		return ref;
	}
};