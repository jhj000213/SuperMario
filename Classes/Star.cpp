#include "stdafx.h"


void Star::m_init(Vec2 Pos, Layer * layer, Vector<Block*> *v)
{
	mario = Sprite::create();
	mario->setPosition(Pos.x + 8, Pos.y + 1);
	mario->setAnchorPoint(Vec2(0.5, 0));
	layer->addChild(mario);

	GLayer = layer;
	Collision = false;

	movePower = 1.0;
	bv = v;

	mario->setVisible(false);

	Vector<SpriteFrame *>aniFrames1;
	char str1[256];
	for (int i = 0; i < 4; i++)
	{
		sprintf(str1, "star%d.png", i);

		SpriteFrame * frame1 = SpriteFrameCache::getInstance()->getSpriteFrameByName(str1);
		aniFrames1.pushBack(frame1);
	}
	Animation * ani = Animation::createWithSpriteFrames(aniFrames1, 0.1, 1);
	Animate * animate = Animate::create(ani);
	auto * seq = Sequence::create(animate, NULL);


	mario->runAction(CCRepeatForever::create(seq));


	JumpForce = 0.0f;
	JumpTime = 0.0f;
	charHigh = 16;
	hero_rightmove = true;
	GLayer = layer;
	Collision = false;
	moving = false;
	movePower = 1.0f;
	del = false;
	gravitypower = 1.5;
	auto ac = MoveBy::create(0.5, Vec2(0, 16));
	auto ac1 = MoveBy::create(0.5, Vec2(18, 0));
	auto call = CallFunc::create(CC_CALLBACK_0(Star::m_setMoving, this));
	auto seqq = Sequence::create(ac,ac1, call, NULL);
	mario->runAction(seqq);

	auto cc = CallFunc::create(CC_CALLBACK_0(Star::m_setVisivle, this));
	auto ss = Sequence::create(DelayTime::create(0.13), cc, NULL);
	mario->runAction(ss);
}

Vec2 Star::m_update(float dt, float MapPosX)
{
	if (moving == true)
	{
		
		m_jump();
		Vec2 hPos = heroPos;

		if (hero_rightmove)
		{
			if (RightMoveCollisionCheck(hPos,MapPosX) == false)
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
		
		if (nowjump == false && touchjumpbutton1 == true)
		{
			nowjump = true;
			JumpForce = 2.8;
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
				gravitypower = 1.5;
			}
		}

		if (nowjump == true)
		{
			JumpTime += dt*1.0f;

			Vec2 tempPos = heroPos;
			somePos = heroPos;
			tempPos.y += JumpForce - (4.0*JumpTime) * gravitypower;
			if (BottomCollisionCheck(tempPos) == false)//블록하나당 생성
			{
				heroPos = tempPos;
				Collision = false;

			}
			else if (TopCollisionCheck(tempPos) == 0)
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
			else if (TopCollisionCheck(tempPos) == 1)
			{
				JumpForce = 0.0f;
				JumpTime = 0.f;
				Collision = true;
				heroPos.y = nowTouchBlock.front()->m_getPosition().y - charHigh;


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
}
void Star::m_jump()
{

	if (nowjump == false)
	{
		touchjumpbutton1 = true;
		gravitypower = 1.5;
	}
}



bool Star::BottomCollisionCheck(Vec2 HPos)
{
	for (Block * bl : *bv)
	{
		if (bl->m_getCrash() == false)
		{
			if (HPos.y <= bl->m_getPosition().y + bl->m_getRect().size.height &&
				HPos.y + charHigh > bl->m_getPosition().y + bl->m_getRect().size.height && (
				HPos.x + 7.5 >= bl->m_getPosition().x &&
				HPos.x - 7.5 <= bl->m_getPosition().x + bl->m_getRect().size.width))
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

bool Star::TopCollisionCheck(Vec2 HPos)
{
	bool hit = false;
	Vector<Block*> tempv;
	for (Block * bl : *bv)
	{
		if (bl->m_getCrash() == false)
		{
			if (HPos.y + charHigh >= bl->m_getPosition().y &&
				HPos.y < bl->m_getPosition().y && (
				HPos.x + 7.5 >= bl->m_getPosition().x &&
				HPos.x - 7.5 <= bl->m_getPosition().x + bl->m_getRect().size.width))
			{
				if (HPos.x >= bl->m_getPosition().x && HPos.x <= bl->m_getPosition().x + 16)
				{
					if (nowTouchBlock.empty() == true)
						nowTouchBlock.pushBack(bl);
				}

				else
				{
					if (nowTouchBlock.empty() == true)
						tempv.pushBack(bl);
				}
				hit = true;
			}
		}

	}
	if (hit == true && nowTouchBlock.empty() == true)
	{
		nowTouchBlock.pushBack(tempv.front());
	}
	return hit;
}

bool Star::LeftMoveCollisionCheck(Vec2 HPos, float MapPosX)
{


	HPos.x -= MoveSpeed * movePower;
	if (HPos.x + 8 <= abs(MapPosX))
		removethis();
	log(" m      %f   %f", HPos.x, -MapPosX);
	for (Block * bl : *bv)
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

bool Star::RightMoveCollisionCheck(Vec2 HPos, float MapPosX)
{
	HPos.x += MoveSpeed * movePower;
	if (HPos.x - 8 >= abs(MapPosX) + D_DESIGN_WIDTH)
	{
		removethis();
		return false;
	}
	for (Block * bl : *bv)
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


Rect Star::m_getRect()
{
	return mario->boundingBox();
}

void Star::removethis()
{
	mario->setVisible(false);
	del = true;
}


void Star::m_setMoving()
{
	nowjump = true;
	moving = true;
	heroPos = mario->getPosition();
}

void Star::m_setVisivle()
{
	mario->setVisible(true);
}