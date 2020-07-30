#pragma once
#include "stdafx.h"

class Block : public Node
{
private:
	Sprite * block;
	Sprite * Skin;
	Layer * GLayer;
	int X;
	int Y;
	bool upping;
	char Info;
	bool Action;
	bool Crash;
	bool Solid;

	int hitnum;

	void (*makeobj) (int,Vec2);
	
public:
	void m_init(int x, int y, char info, Layer * layer, void pfunc(int , Vec2));
	Rect m_getRect();
	Vec2 m_getPosition();
	bool returninfo();
	void ActionBlock();
	void CrashBlock(int charinfo);
	bool m_getCrash();
	bool m_getUpping();
	void m_setUpping();

	static Block * m_create(int x, int y, char info, Layer * layer, void pfunc(int, Vec2))
	{
		auto ref = new Block();
		ref->m_init(x, y, info, layer,pfunc);
		ref->autorelease();

		return ref;
	}
};