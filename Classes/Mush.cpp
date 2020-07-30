#include "stdafx.h"

void Mush::m_init(Vec2 Pos, Layer * layer,int info, Vector<Block*> *v, Vector<Mush*> *mv)
{
	if (info == 1)
	{
		mush = Sprite::create("mushroom.png");
		mush->setPosition(Pos.x + 8, Pos.y + 1);
		mush->setAnchorPoint(Vec2(0.5, 0));
		layer->addChild(mush, -1);
	}
	else
	{
		mush = Sprite::create("1upmush.png");
		mush->setPosition(Pos.x + 8, Pos.y + 1);
		mush->setAnchorPoint(Vec2(0.5, 0));
		layer->addChild(mush, -1);
	}
	mush->setVisible(false);

	charHigh = 16;
	moving = false;
	heroPos = mush->getPosition();
	GLayer = layer;
	Collision = false;
	Info = info;
	movePower = 1.0f;
	del = false;

	auto ac = MoveBy::create(0.5, Vec2(0, 16));
	auto call = CallFunc::create(CC_CALLBACK_0(Mush::m_setMoving, this));
	auto seq = Sequence::create(ac, call, NULL);
	mush->runAction(seq);

	auto cc = CallFunc::create(CC_CALLBACK_0(Mush::m_setVisivle, this));
	auto ss = Sequence::create(DelayTime::create(0.13), cc, NULL);
	mush->runAction(ss);

	bvv = v;
	mushv = mv;
}

void Mush::m_setMoving()
{
	moving = true;
	hero_rightmove = true;
}

void Mush::m_update(float dt, float MapPosX)
{
	if (moving == true)
	{
		Vec2 hPos = heroPos;
		if (hero_rightmove)
		{
			if (RightMoveCollisionCheck(hPos, MapPosX) == false)
			{
				hPos.x += (MoveSpeed * movePower);
				heroPos = hPos;
				hero_leftsee = false;
			}
			else
			{
				hero_leftmove = true;
				hero_rightmove = false;
			}



		}
		if (hero_leftmove)
		{
			if (LeftMoveCollisionCheck(hPos, MapPosX) == false)
			{
				hPos.x -= (MoveSpeed * movePower);
				heroPos = hPos;
				hero_leftsee = true;
			}
			else
			{
				hero_leftmove = false;
				hero_rightmove = true;
			}



		}




		if (nowjump == false)
		{
			Vec2 tempPos = heroPos;
			tempPos.y -= 1.0f;

			if (BottomCollisionCheck(tempPos) == false)
			{
				nowjump = true;
				JumpForce = 0.f;
				JumpTime = 0.f;
			}
			if (BottomCollisionCheck(tempPos) == true)//블록 하나당 하나씩 추가
			{
				nowjump = false;
				touchjumpbutton1 = false;
				heroPos.y = nowTouchBlock.front()->m_getPosition().y + 16.0f;
				nowTouchBlock.clear();
			}
		}

		//log("%f", gravitypower);
		if (nowjump == true)
		{
			JumpTime += dt*1.0f;

			Vec2 tempPos = heroPos;
			somePos = heroPos;
			tempPos.y += JumpForce - (9.8*JumpTime) * 2.0;
			if (BottomCollisionCheck(tempPos) == false)//블록하나당 생성
			{
				heroPos = tempPos;
				Collision = false;

			}


			if (BottomCollisionCheck(tempPos) == true)//블록 하나당 하나씩 추가
			{
				nowjump = false;
				touchjumpbutton1 = false;
				heroPos.y = nowTouchBlock.front()->m_getPosition().y + 16.0f;
				nowTouchBlock.clear();
			}
		}
		nowTouchBlock.clear();
		if (Collision)
		{
			mush->setPosition(somePos);
			
		}
		else if (!Collision)
		{
			mush->setPosition(heroPos);
			
		}

	}
	
}

bool Mush::BottomCollisionCheck(Vec2 HPos)
{
	for (Block * bl : *bvv)
	{
		if (bl->m_getCrash() == false)
		{
			if (HPos.y <= bl->m_getPosition().y + bl->m_getRect().size.height &&
				HPos.y + charHigh > bl->m_getPosition().y + bl->m_getRect().size.height && (
				HPos.x + 8 > bl->m_getPosition().x &&
				HPos.x - 8 < bl->m_getPosition().x + bl->m_getRect().size.width))
			{
				{
					nowTouchBlock.clear();
					nowTouchBlock.pushBack(bl);
				}
				return true;
			}
		}

	}
	return false;
}

bool Mush::LeftMoveCollisionCheck(Vec2 HPos, float MapPosX)
{
	HPos.x -= MoveSpeed * movePower;
	if (HPos.x + 8 <= abs(MapPosX))
		removethis();
	for (Block * bl : *bvv)
	{
		if (bl->m_getCrash() == false)
		{
			Rect rect = Rect(HPos.x - 8.0f, HPos.y + 0.1f, 16.0f, charHigh);
			if (rect.intersectsRect(bl->m_getRect()))
			{
				{
					if (nowTouchBlock.empty() == true)
					nowTouchBlock.pushBack(bl);
				}
				return true;
			}
		}

	}
	return false;
}

bool Mush::RightMoveCollisionCheck(Vec2 HPos, float MapPosX)
{
	HPos.x += MoveSpeed * movePower;
	if (HPos.x - 8 >= abs(MapPosX) + D_DESIGN_WIDTH)
	{
		removethis();
		return false;
	}
	for (Block * bl : *bvv)
	{
		if (bl->m_getCrash() == false)
		{
			Rect rect = Rect(HPos.x - 8.0f, HPos.y + 0.1f, 16.0f, charHigh);
			if (rect.intersectsRect(bl->m_getRect()))
			{
				{
					if (nowTouchBlock.empty() == true)
					nowTouchBlock.pushBack(bl);
				}
				return true;
			}
		}

	}
	return false;
}

void Mush::removethis()
{
	if (Info == 2)
	{
		Sprite * label = Sprite::create("1up.png");
		label->setPosition(mush->getPosition());
		GLayer->addChild(label, 2);

		auto ac = MoveBy::create(1.0f, Vec2(0, 100));
		label->runAction(Sequence::create(ac, RemoveSelf::create(), NULL));
	}
	mush->setVisible(false);
	del = true;
}

Rect Mush::m_getRect()
{
	return mush->boundingBox();
}

int Mush::m_getInfo()
{
	return Info;
}

void Mush::m_setVisivle()
{
	mush->setVisible(true);
}