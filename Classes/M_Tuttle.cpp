#include "stdafx.h"

void M_Tuttle::m_init(Vec2 Pos, Layer * layer, Vector<Block*> *v, Vector<M_Tuttle*> *mv)
{

	mush = Sprite::create("turtle.png");
	mush->setPosition(Pos.x + 8, Pos.y + 1);
	mush->setAnchorPoint(Vec2(0.5, 0));
	layer->addChild(mush);


	hero_leftmove = true;
	charHigh = 16;
	moving = false;
	heroPos = mush->getPosition();
	GLayer = layer;
	Collision = false;
	movePower = 1.0f;
	del = false;



	bvv = v;
	mushv = mv;
}


void M_Tuttle::m_update(float dt, float MapPosX)
{
	if (abs(MapPosX) + D_DESIGN_WIDTH > mush->getPositionX())
		moving = true;
	if (moving == true && del == false)
	{
		Vec2 hPos = heroPos;

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

bool M_Tuttle::BottomCollisionCheck(Vec2 HPos)
{
	for (Block * bl : *bvv)
	{
		if (bl->m_getUpping() == true)
		{
			if (bl->m_getRect().intersectsRect(mush->boundingBox()))
			{
				burn = true;
				del = true;
				break;
			}
		}
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

bool M_Tuttle::LeftMoveCollisionCheck(Vec2 HPos, float MapPosX)
{
	HPos.x -= MoveSpeed * movePower;
	if (HPos.x + 8 <= abs(MapPosX))
	{
		del = true;
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
					if (bl->m_getUpping() == true)
					{
						del = true;
						burn = true;

						break;
					}
					if (nowTouchBlock.empty() == true)
						nowTouchBlock.pushBack(bl);
				}
				return true;
			}
		}

	}
	return false;
}

bool M_Tuttle::RightMoveCollisionCheck(Vec2 HPos, float MapPosX)
{
	HPos.x += MoveSpeed * movePower;
	for (Block * bl : *bvv)
	{
		if (bl->m_getCrash() == false)
		{
			Rect rect = Rect(HPos.x - 8.0f, HPos.y + 0.1f, 16.0f, charHigh);
			if (rect.intersectsRect(bl->m_getRect()))
			{
				{
					if (bl->m_getUpping() == true)
					{
						del = true;
						burn = true;

						break;
					}
					if (nowTouchBlock.empty() == true)
						nowTouchBlock.pushBack(bl);
				}
				return true;
			}
		}

	}
	return false;
}

void M_Tuttle::removethis()
{

	mushv->eraseObject(this);
}

Rect M_Tuttle::m_getRect()
{
	return mush->boundingBox();
}

void M_Tuttle::m_setVisivle()
{
	mush->setVisible(true);
}

void M_Tuttle::m_dead()
{
	if (burn == true)
	{
		mush->setVisible(false);
		Sprite * eff = Sprite::create("turtle.png");
		eff->setAnchorPoint(Vec2(0.5, 0));
		eff->setPosition(mush->getPosition());
		eff->setScaleY(-1.0f);
		GLayer->addChild(eff, 1);
		auto ac = JumpBy::create(0.7f, Vec2(40, -160), 120, 1);
		//auto call = CallFunc::create(CC_CALLBACK_0(M_Tuttle::removethis, this));
		eff->runAction(Sequence::create(ac, RemoveSelf::create(), NULL));
	}
	else
	{
		mush->setVisible(false);
		Sprite * eff = Sprite::create("turtle.png");
		eff->setAnchorPoint(Vec2(0.5, 0));
		eff->setPosition(mush->getPosition());
		eff->setScaleY(0.1f);
		GLayer->addChild(eff, 1);
		eff->runAction(Sequence::create(DelayTime::create(0.7f), RemoveSelf::create(), NULL));
	}
}