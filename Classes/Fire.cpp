#include "stdafx.h"


void Fire::m_init(Vec2 Pos, Layer * layer, bool left, Vector<Block*> *v)
{
	mario = Sprite::create("fire.png");
	mario->setPosition(Pos.x, Pos.y + 12);
	layer->addChild(mario);

	GLayer = layer;
	Collision = false;

	movePower = 1.0;
	bv = v;
	heroPos = mario->getPosition();
	auto ro = RotateBy::create(0.1f, 90);
	mario->runAction(CCRepeatForever::create(ro));
	
	if (left)
		hero_leftmove = true;
	else
		hero_rightmove = true;

	out = false;
	nowjump = true;
	JumpForce = 0.0f;
	JumpTime = 0.0f;
	charHigh = 4;
	GLayer = layer;
	Collision = false;
	movePower = 1.0f;
	del = false;
	gravitypower = 3.8;
}

Vec2 Fire::m_update(float dt, float MapPosX)
{


		m_jump();
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
				//
				removethis();
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
				//
				removethis();
			}
			
		}

		if (nowjump == false && touchjumpbutton1 == true)
		{
			nowjump = true;
			JumpForce = 5.0;
			JumpTime = 0.f;
		}
		else if (nowjump == false)
		{
			Vec2 tempPos = heroPos;
			tempPos.y -= 1.0f;

			if (BottomCollisionCheck(tempPos) == false)
			{
				nowjump = true;
				JumpForce = 0.f;
				JumpTime = 0.f;
				gravitypower = 3.8;
			}
		}

		if (nowjump == true)
		{
			JumpTime += dt*1.0f;

			Vec2 tempPos = heroPos;
			somePos = heroPos;
			tempPos.y += JumpForce - (9.8*JumpTime) * gravitypower;
			if (BottomCollisionCheck(tempPos) == false)//블록하나당 생성
			{
				heroPos = tempPos;
				Collision = false;

			}
			
			if (BottomCollisionCheck(tempPos) == true)//블록 하나당 하나씩 추가
			{
				nowjump = false;
				touchjumpbutton1 = false;
				heroPos.y = nowTouchBlock.front()->m_getPosition().y + 20;
				nowTouchBlock.clear();
			}
		
		}
		nowTouchBlock.clear();








		if (Collision)
		{
			mario->setPosition(somePos);
			return somePos;
		}
		else if (!Collision)
		{
			mario->setPosition(heroPos);
			return heroPos;
		}
	

}
void Fire::m_jump()
{

	if (nowjump == false)
	{
		touchjumpbutton1 = true;
		gravitypower = 3.8;
	}
}



bool Fire::BottomCollisionCheck(Vec2 HPos)
{
	for (Block * bl : *bv)
	{
		if (bl->m_getCrash() == false)
		{
			if (HPos.y - 4 < bl->m_getPosition().y + bl->m_getRect().size.height &&
				HPos.y + 4 > bl->m_getPosition().y + bl->m_getRect().size.height && (
				HPos.x + 3.5 >= bl->m_getPosition().x &&
				HPos.x - 3.5<= bl->m_getPosition().x + bl->m_getRect().size.width))
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


bool Fire::LeftMoveCollisionCheck(Vec2 HPos, float MapPosX)
{


	HPos.x -= MoveSpeed * movePower;
	if (HPos.x + 4 <= abs(MapPosX))
	{
		out = true;
		removethis();
		return false;
	}
	log(" m      %f   %f", HPos.x, -MapPosX);
	for (Block * bl : *bv)
	{
		if (bl->m_getCrash() == false)
		{
			Rect rect = Rect(HPos.x - 3.5, HPos.y - 3.5, 8, 8);
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

bool Fire::RightMoveCollisionCheck(Vec2 HPos, float MapPosX)
{
	HPos.x += MoveSpeed * movePower;
	if (HPos.x - 4 >= abs(MapPosX) + D_DESIGN_WIDTH)
	{
		out = true;
		removethis();
		return false;
	}
	for (Block * bl : *bv)
	{
		if (bl->m_getCrash() == false)
		{
			Rect rect = Rect(HPos.x - 3.5, HPos.y - 3.5, 8, 8);
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


Rect Fire::m_getRect()
{
	return mario->boundingBox();
}

void Fire::removethis()
{
	mario->setVisible(false);
	del = true;
}


void Fire::m_setMoving()
{
	nowjump = true;
	moving = true;
	heroPos = mario->getPosition();
}

void Fire::m_setVisivle()
{
	mario->setVisible(true);
}

void Fire::m_boom()
{
	mario->setVisible(false);
	Sprite * boom = Sprite::create();
	boom->setPosition(mario->getPosition());
	GLayer->addChild(boom, 1);

	Vector<SpriteFrame *>aniFrames1;
	char str1[256];
	for (int i = 0; i < 3; i++)
	{
		sprintf(str1, "boom%d.png", i);

		SpriteFrame * frame1 = SpriteFrameCache::getInstance()->getSpriteFrameByName(str1);
		aniFrames1.pushBack(frame1);
	}
	Animation * ani = Animation::createWithSpriteFrames(aniFrames1, 0.07, 1);
	Animate * animate = Animate::create(ani);
	auto * seq = Sequence::create(animate,RemoveSelf::create(), NULL);


	boom->runAction(seq);
}