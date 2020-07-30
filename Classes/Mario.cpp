#include "stdafx.h"
#define FRAME_TIME 0.05f
#define SHOOT_DELAY 0.1
#define STRONG_TIME 1.0f
#define STAR_DELAY 0.1

void Mario::m_init(Vec2 Pos, Layer * layer, Vector<Block*> *v, Vector<Fire*> *fv)
{
	mario = Sprite::create("mario_kid0.png");
	mario->setPosition(Pos);
	mario->setAnchorPoint(Vec2(0.5, 0));
	layer->addChild(mario);

	actioning = false;
	light = false;
	star = false;
	dead = false;
	Clear = false;
	pressshoot = false;
	heroPos = Pos;
	GLayer = layer;
	Collision = false;
	Info = 1;
	movePower = 0.0f;
	nowFrame = 0;
	strong = false;
	falling = false;
	starTime = 12.0f;
	stardelay = STAR_DELAY;
	strongTime = STRONG_TIME;
	m_time = FRAME_TIME;
	m_shootDelay = 0.0f;
	changeFrame = false;
	firev = fv;
	bv = v;
}

Vec2 Mario::m_update(float dt, float MapPosX)
{
	log("%f", dt);
	if (dead == false && Clear == false)
	{

		Vec2 hPos = heroPos;
		m_shootDelay -= dt;

		if (pressshoot == true)
		{
			m_shoot();
			pressshoot = false;
		}

		if (star == true)
		{
			starTime -= dt;
			stardelay -= dt;
			if (stardelay <= 0.0f)
			{
				light = !light;
				stardelay = STAR_DELAY;
			}
		}
		if (starTime <= 0.0f)
		{
			star = false;
			light = false;
		}

		if (strong)
			strongTime -= dt;
		if (strongTime <= 0.0f)
		{
			strong = false;
			strongTime = STRONG_TIME;
		}

		if (Info == 1)
			charHigh = 16.0;
		else
			charHigh = 32.0;

		if (hero_leftsee == true && nowjump == false)
			mario->setScaleX(-1.0f);
		else if (hero_leftsee == false && nowjump == false)
			mario->setScaleX(1.0f);

		if (hero_leftmove || hero_rightmove)
			movePower += dt*2.0f;
		else
			movePower -= dt;

		if (movePower >= 1.0f)
			movePower = 1.0f;
		if (movePower <= 0.0f)
			movePower = 0.0f;
		if (hero_leftmove)
		{
			if (LeftMoveCollisionCheck(hPos, MapPosX) == false)
			{
				hPos.x -= (MoveSpeed * movePower);
				heroPos = hPos;
				hero_leftsee = true;
			}
		}
		if (hero_rightmove)
		{
			if (RightMoveCollisionCheck(hPos) == false)
			{
				hPos.x += (MoveSpeed * movePower);
				heroPos = hPos;
				hero_leftsee = false;
			}
		}
		if (nowjump == false && touchjumpbutton1 == true)
		{
			nowjump = true;
			JumpForce = 8.0;
			JumpTime = 0.f;
		}
		else if (nowjump == false)
		{
			falling = false;
			Vec2 tempPos = heroPos;
			tempPos.y -= 1.0f;

			if (BottomCollisionCheck(tempPos) == false)
			{
				nowjump = true;
				JumpForce = 0.f;
				JumpTime = 0.f;
				gravitypower = 2.5;
			}
		}
		if (touchjumpbutton1)
			gravitypower -= dt * 7;
		if (gravitypower <= 1.5f)
			gravitypower = 1.5f;
		if (nowjump == true)
		{
			JumpTime += dt;

			Vec2 tempPos = heroPos;
			somePos = heroPos;
			tempPos.y += JumpForce - (9.8*JumpTime) * gravitypower;
			if (0 >= JumpForce - (9.8*JumpTime) * gravitypower)
				falling = true;

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

				nowTouchBlock.front()->ActionBlock();
				nowTouchBlock.front()->CrashBlock(Info);
				nowTouchBlock.clear();
			}
		}
		nowTouchBlock.clear();




		m_time -= dt;
		if (m_time <= 0.0f)
		{
			m_time = FRAME_TIME;
			changeFrame = true;
			if (nowjump == true)
				nowFrame = 1;
			else if (nowjump == false && (hero_rightmove || hero_leftmove) == false)
				nowFrame = 0;
			else if ((hero_rightmove || hero_leftmove) && nowjump == false)
			{
				if (nowFrame >= 2 && nowFrame <= 4)
				{
					nowFrame++;
					if (nowFrame > 4)
						nowFrame = 2;
				}
				else
					nowFrame = 2;
			}
			else
				nowFrame = 0;
		}
		if (changeFrame == true)
		{

			ChangeSprite();
		}

		if (heroPos.y <= -120.0f)
			m_dead();

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
	else if (Clear == true && dead == false)
	{
		star = false;
		light = false;
		m_clearUpdate(dt);
	}
}

void Mario::m_clearUpdate(float dt)
{
	m_time -= dt;
	if (m_time <= 0.0f)
	{
		if (actioning == true)
		{
			m_time = FRAME_TIME;
			changeFrame = true;
			nowFrame++;
			if (nowFrame > 4)
				nowFrame = 2;
		}
		else
		{
			m_time = FRAME_TIME;
			changeFrame = true;
			nowFrame++;
			if (nowFrame > 1)
				nowFrame = 0;
		}
		
	}
	if (changeFrame == true)
	{
		changeFrame = false;
		ChangeSprite();
	}
}

void Mario::m_ClearAnima()
{
	star = false;
	light = false;
	Clear = true;
	nowFrame = 0;
	float per;
	if (heroPos.y>180)
		per = (mario->getPositionY() - 64) / 128;
	else
		per = (mario->getPositionY() - 48) / 128;

	auto ac = MoveBy::create(1 * per, Vec2(0, -(mario->getPositionY() - 48)));
	auto call = CallFunc::create(CC_CALLBACK_0(Mario::m_setCA, this));
	auto ac1 = ScaleTo::create(0, 1.0f);
	auto ac2 = MoveBy::create(0.5, Vec2(40, 0));
	auto ac3 = MoveBy::create(0.5, Vec2(0, -16));
	auto ac4 = MoveBy::create(1.2, Vec2(60, 0));
	auto ac5 = FadeTo::create(0, 0);
	auto seq = Sequence::create(ac, DelayTime::create(1 - (1 * per)), call, 
		ac1,ac2,ac3,ac4,ac5,DelayTime::create(1.5f),
		CallFunc::create(CC_CALLBACK_0(Mario::end,this)), NULL);
	mario->runAction(seq);
}

void Mario::m_setCA()
{
	m_time = 0.05f;
	actioning = true;
	nowFrame = 2;
	mario->setAnchorPoint(Vec2(1, 0));
	mario->setScaleX(-1.0f);
}

void Mario::ChangeSprite()
{


	if (Clear == true && actioning == false)
	{
		if (Info == 1)
		{
			char str[256];
			sprintf(str, "mario_kid_hang%d.png", nowFrame);
			mario->setTexture(str);
		}
		else if (Info == 2)
		{
			char str[256];
			sprintf(str, "mario_man_hang%d.png", nowFrame);
			mario->setTexture(str);
		}
		else if (Info == 3)
		{
			char str[256];
			sprintf(str, "mario_fire_hang%d.png", nowFrame);
			mario->setTexture(str);
		}
	}
	else if (Clear == false || actioning == true)
	{
		//log("%d", nowFrame);
		if (Info == 1)
		{
			if (light == true)
			{
				char str[256];
				sprintf(str, "mario_kid_star%d.png", nowFrame);
				mario->setTexture(str);
			}
			else
			{
				char str[256];
				sprintf(str, "mario_kid%d.png", nowFrame);
				mario->setTexture(str);
			}
		}
		else if (Info == 2)
		{
			if (light == true)
			{
				char str[256];
				sprintf(str, "mario_man_star%d.png", nowFrame);
				mario->setTexture(str);
			}
			else
			{
				char str[256];
				sprintf(str, "mario_man%d.png", nowFrame);
				mario->setTexture(str);
			}
		}
		else if (Info == 3)
		{
			if (light == true)
			{
				char str[256];
				sprintf(str, "mario_man_star%d.png", nowFrame);
				mario->setTexture(str);
			}
			else
			{
				char str[256];
				sprintf(str, "mario_fire%d.png", nowFrame);
				mario->setTexture(str);
			}
		}
	}
}

void Mario::m_killjump()
{
	JumpTime = 0.0f;
	nowjump = true;
	JumpForce = 6.0;
	nowFrame = 1;
	ChangeSprite();
	touchjumpbutton1 = true;
	gravitypower = 5.0;
}

void Mario::m_jump()
{

	if (nowjump == false)
	{
		nowFrame = 1;
		ChangeSprite();
		touchjumpbutton1 = true;
		gravitypower = 3.8;
	}
}

void Mario::m_releaseJump()
{
	touchjumpbutton1 = false;
}

bool Mario::BottomCollisionCheck(Vec2 HPos)
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

bool Mario::TopCollisionCheck(Vec2 HPos)
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

bool Mario::LeftMoveCollisionCheck(Vec2 HPos, float MapPosX)
{
	

	HPos.x -= MoveSpeed * movePower;
	if (HPos.x - 8 <= abs(MapPosX))
		return true;
	//log(" m      %f   %f", HPos.x, -MapPosX);
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

bool Mario::RightMoveCollisionCheck(Vec2 HPos)
{
	HPos.x += MoveSpeed * movePower;
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

void Mario::m_touchLeft()
{
	if (nowjump == false)
		nowFrame = 2;

	movePower /= 2.0f;
	hero_leftsee = true;
	hero_rightmove = false;
	hero_leftmove = true;
}

void Mario::m_touchRight()
{
	if (nowjump == false)
		nowFrame = 2;

	movePower /= 2.0f;
	hero_leftsee = false;
	hero_leftmove = false;
	hero_rightmove = true;
}

void Mario::m_releaseLeft()
{
	if (nowjump == false)
	{
		nowFrame = 0;
		ChangeSprite();
	}
	hero_leftmove = false;
}

void Mario::m_releaseRight()
{
	if (nowjump == false)
	{
		nowFrame = 0;
		ChangeSprite();
	}
	hero_rightmove = false;
}

int Mario::m_getInfo()
{
	return Info;
}

Rect Mario::m_getRect()
{
	return mario->boundingBox();
}

Rect Mario::m_getFootRect()
{
	Rect rect = Rect(mario->getPosition().x - 9, mario->getPosition().y, 16, 1);
	return rect;
}

void Mario::m_UPgrade()
{
	Info++;
	if (Info == 4)
		Info = 3;

	auto ac1 = CallFunc::create(CC_CALLBACK_0(Mario::m_setSprite1, this));
	auto ac2 = CallFunc::create(CC_CALLBACK_0(Mario::m_setSprite2, this));
	auto de = DelayTime::create(0.1f);
	auto seq = Sequence::create(ac1, de, ac2, de, ac1, de, ac2, de, ac1, de, ac2, NULL);
	this->runAction(seq);
}

void Mario::m_Downgrade()
{
	Info--;
	if (Info == 0)
	{
		m_dead();
	}
	else
	{
		auto ac1 = CallFunc::create(CC_CALLBACK_0(Mario::m_setSprite3, this));
		auto ac2 = CallFunc::create(CC_CALLBACK_0(Mario::m_setSprite4, this));
		auto call = CallFunc::create(CC_CALLBACK_0(Mario::m_setStrong, this));
		auto de = DelayTime::create(0.1f);
		auto seq = Sequence::create(ac1, de, ac2, de, ac1, de, ac2, de, ac1, de, ac2,de,call, NULL);
		this->runAction(seq);
	}
}

void Mario::m_setSprite1()
{
	if (Info == 2)
	{
		char str[256];
		sprintf(str, "mario_kid%d.png", nowFrame);
		mario->setTexture(str);
	}
	else if (Info == 3)
	{
		char str[256];
		sprintf(str, "mario_man%d.png", nowFrame);
		mario->setTexture(str);
	}
	
}

void Mario::m_setSprite2()
{
	if (Info == 2)
	{
		char str[256];
		sprintf(str, "mario_man%d.png", nowFrame);
		mario->setTexture(str);
	}
	else if (Info == 3)
	{
		char str[256];
		sprintf(str, "mario_fire%d.png", nowFrame);
		mario->setTexture(str);
	}
}

void Mario::m_setSprite3()
{
	if (Info == 2)
	{
		char str[256];
		sprintf(str, "mario_fire%d.png", nowFrame);
		mario->setTexture(str);
	}
	else if (Info == 1)
	{
		char str[256];
		sprintf(str, "mario_man%d.png", nowFrame);
		mario->setTexture(str);
	}

}

void Mario::m_setSprite4()
{
	if (Info == 2)
	{
		char str[256];
		sprintf(str, "mario_man%d.png", nowFrame);
		mario->setTexture(str);
	}
	else if (Info == 1)
	{
		char str[256];
		sprintf(str, "mario_kid%d.png", nowFrame);
		mario->setTexture(str);
	}
}

void Mario::m_setStrong()
{
	strong = true;

	auto ac = FadeTo::create(0.0f, 0);
	auto ac1 = FadeTo::create(0.0f, 255);
	auto de = DelayTime::create(0.2);

	auto seq = Sequence::create(ac, de, ac1, de, ac, de, ac1, de, ac, de, ac1, NULL);
	mario->runAction(seq);
}

void Mario::m_shoot()
{
	if (Info == 3)
	{
		Fire * fire = Fire::m_create(mario->getPosition(), GLayer, hero_leftsee, bv);
		firev->pushBack(fire);
	}
}

void Mario::m_touchShoot()
{
	if (m_shootDelay <= 0.0f && pressshoot == false)
	{
		pressshoot = true;
		m_shootDelay = SHOOT_DELAY;
	}
}

void Mario::m_dead()
{
	dead = true;
	mario->setTexture("mario_dead.png");
	auto ac = JumpBy::create(0.8, Vec2(-64, -128), 120, 1);
	auto call = CallFunc::create(CC_CALLBACK_0(Mario::end, this));
	auto seq = Sequence::create(DelayTime::create(0.6), ac,call, NULL);
	mario->runAction(seq);
}

void Mario::end()
{
	Director::getInstance()->end();
}

void Mario::m_star()
{
	star = true;
}