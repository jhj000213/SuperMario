#include "stdafx.h"
#include <iostream>
#include <fstream>

Vector<Block*> blockv;
Vector<Mush*> mushv;
Vector<Flower*> flowerv;
Vector<Star*> starv;
Vector<Fire*> firev;
Vector<M_Gumba*> gumbav;
Vector<M_Tuttle*> turtlev;
Layer * GameLayer;
Sprite * Flag;
int HeroInfo;

Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);
	return scene;
}

void MakingObj(int num,Vec2 Pos)
{
	if (num == 1)
	{
		if (HeroInfo == 1)
		{
			Mush * mush = Mush::m_create(Pos, GameLayer,1, &blockv, &mushv);
			mushv.pushBack(mush);
		}
		else
		{
			Flower * mush = Flower::m_create(Pos, GameLayer, &blockv, &flowerv);
			flowerv.pushBack(mush);
		}
	}
	else if (num == 2)
	{
		Mush * mush = Mush::m_create(Pos, GameLayer, 2, &blockv, &mushv);
		mushv.pushBack(mush);
	}
	else if (num == 3)
	{
		Star * star = Star::m_create(Pos, GameLayer, &blockv);
		starv.pushBack(star);
	}
	else if (num == 10)
	{
		M_Gumba * gumba = M_Gumba::m_create(Pos, GameLayer, &blockv, &gumbav);
		gumbav.pushBack(gumba);
	}
	else if (num == 11)
	{
		M_Tuttle * tuttle = M_Tuttle::m_create(Pos, GameLayer, &blockv, &turtlev);
		turtlev.pushBack(tuttle);
	}
	else if (num == 15)
	{
		Flag = Sprite::create("flag.png");
		Flag->setAnchorPoint(Vec2(1, 1));
		Flag->setPosition(Pos);
		GameLayer->addChild(Flag, 1);
	}
}

bool GameScene::init()
{

	if (!LayerColor::initWithColor(Color4B(125,125, 255, 255)))
		return false;

	int n = sqrt(169);

	makingNum = 0;
	GameClear = false;

	GameLayer = Layer::create();
	this->addChild(GameLayer);

	Sprite * spr1 = Sprite::create("castleflag.png");
	spr1->setPosition(Vec2(100,24));
	GameLayer->addChild(spr1, 5000);
	auto asdf = JumpBy::create(1.0f, Vec2(0, 0), 100, 1);
	spr1->runAction(RepeatForever::create(asdf));

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("itembox.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("coin.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boom.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("flower.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("star.plist");

	ifstream fin;
	fin.open("stage1.txt", ios::in);

	if (fin.fail())
		CCLOG("Open Fail");
	else
		CCLOG("Open Success");

	for (int y = MAP_HEIGHT - 1; y >= 0; y--)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			char ch;
			fin >> ch;
			
			if (x == 0 && y == 0)
				log("sfd");

			Block * block = Block::m_create(x, y, ch, GameLayer,&MakingObj);
			if (block->returninfo() == true)
			{
				blockv.pushBack(block);
			}
		}
	}

	fin.close();

	Hero = Mario::m_create(Vec2(100, 16 * 3), GameLayer, &blockv,&firev);
	GameLayer->addChild(Hero);

	Castle1 = Sprite::create("castle1.png");
	Castle1->setPosition(Vec2(3280,32));
	Castle1->setAnchorPoint(Vec2(1, 0));
	GameLayer->addChild(Castle1, -1);

	Castle2 = Sprite::create("castle2.png");
	Castle2->setPosition(Vec2(3280, 32));
	Castle2->setAnchorPoint(Vec2(0, 0));
	GameLayer->addChild(Castle2, -1);

	RedFlag = Sprite::create("castleflag.png");
	RedFlag->setAnchorPoint(Vec2(1, 0));
	RedFlag->setPosition(Vec2(3280, 80));
	GameLayer->addChild(RedFlag, -2);

	FlagRect = Rect(Flag->getPosition().x-8, 0, 16, 500);

	pressJumpTime = 1.0f;
	runupdate = true;

	m_pCamera = new M_Camera();
	m_pCamera->init(GameLayer, Size(16 * MAP_WIDTH, 16 * MAP_HEIGHT),
		Size(D_DESIGN_WIDTH, D_DESIGN_HEIGHT));


	listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(GameScene::onTouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(GameScene::onTouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(GameScene::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	auto keylistener = EventListenerKeyboard::create();
	keylistener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	keylistener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keylistener, this);

	schedule(schedule_selector(GameScene::Update), 0.0f);

	return true;
}

void GameScene::Update(float dt)
{
	if (runupdate == true)
	{
		m_pCamera->update(Hero->m_update(dt, GameLayer->getPosition().x), GameLayer->getPosition());
	}
	if (runupdate == true && GameClear == false)
	{
		HeroInfo = Hero->m_getInfo();
		
		if (Hero->m_getRect().intersectsRect(FlagRect) && GameClear == false)
		{
			Hero->m_ClearAnima();
			GameClear = true;
			ClearAnimation();
		}
		if (mushv.empty() == false)
		{
			for (Mush * m : mushv)
			{
				m->m_update(dt, GameLayer->getPosition().x);
				if (m->m_getInfo() == 1)
				{
					if (m->del == true)
					{
						m->removethis();
						mushv.clear();
						break;
					}
					else if (m->m_getRect().intersectsRect(Hero->m_getRect()))
					{
						runupdate = false;
						Hero->m_UPgrade();
						m->removethis();
						mushv.clear();
						auto ac = CallFunc::create(CC_CALLBACK_0(GameScene::setRunUpdate, this));
						this->runAction(Sequence::create(DelayTime::create(0.6), ac, NULL));
						break;
					}
				}
				else
				{
					if (m->del == true)
					{
						m->removethis();
						mushv.clear();
						break;
					}
					else if (m->m_getRect().intersectsRect(Hero->m_getRect()))
					{
						m->removethis();
						mushv.clear();
						break;
					}
				}
			}
		}
		if (flowerv.empty() == false)
		{
			for (Flower * m : flowerv)
			{
				m->m_update(dt, GameLayer->getPosition().x);
				if (m->m_getRect().intersectsRect(Hero->m_getRect()))
				{
					if (HeroInfo != 3)
					{
						runupdate = false;
						Hero->m_UPgrade();
						m->removethis();
						flowerv.clear();
						auto ac = CallFunc::create(CC_CALLBACK_0(GameScene::setRunUpdate, this));
						this->runAction(Sequence::create(DelayTime::create(0.6), ac, NULL));
						break;
					}
					else
					{
						m->removethis();
						flowerv.clear();
						break;
					}
				}
			}
		}
		if (starv.empty() == false)
		{
			for (Star * m : starv)
			{
				m->m_update(dt, GameLayer->getPosition().x);
				if (m->m_getRect().intersectsRect(Hero->m_getRect()))
				{
					Hero->m_star();
					m->removethis();
					flowerv.clear();
					break;
				}
				if (m->del == true)
				{
					starv.eraseObject(m);
					break;
				}
			}
		}

		
		if (firev.empty() == false)
		{
			Vector<Fire*> tempv;
			for (Fire * m : firev)
			{
				m->m_update(dt, GameLayer->getPosition().x);
				for (M_Gumba * gumba : gumbav)
				{
					if(gumba->m_getRect().intersectsRect(m->m_getRect()))
					{
						gumba->burn = true;
						gumba->del = true;
						m->del = true;
					}
				}
				if (m->del == true)
				{
					if (m->out==false)
						m->m_boom();

					tempv.pushBack(m);
					break;
				}
				    
				for (M_Tuttle * gumba : turtlev)
				{
					if (gumba->m_getRect().intersectsRect(m->m_getRect()))
					{
						gumba->burn = true;
						gumba->del = true;
						m->del = true;
					}
				}
				if (m->del == true)
				{
					if (m->out == false)
						m->m_boom();

					tempv.pushBack(m);
					break;
				}
			}
			for (Fire*m : tempv)
			{
				firev.eraseObject(m);
			}
		}
		

		if (gumbav.empty() == false)
		{
			
			for (M_Gumba * gumba : gumbav)
			{
				
				gumba->m_update(dt, GameLayer->getPosition().x);

				if (Hero->m_getRect().intersectsRect(gumba->m_getRect()) && Hero->star == true)
				{
					gumba->del = true;
					gumba->burn = true;
				}
				else if (gumba->del == false)
				{
					if (Hero->falling == true)
					{
						if (Hero->m_getFootRect().intersectsRect(gumba->m_getRect()))
						{
							Hero->m_killjump();
							gumba->del = true;
							break;
						}
						else if (Hero->m_getRect().intersectsRect(gumba->m_getRect()))
						{
							if (Hero->strong == false)
							{
								runupdate = false;
								if (HeroInfo != 1)
								{
									auto ac = CallFunc::create(CC_CALLBACK_0(GameScene::setRunUpdate, this));
									this->runAction(Sequence::create(DelayTime::create(0.6), ac, NULL));
								}
								
									Hero->m_Downgrade();
							}

							break;
						}
					}
					else if (Hero->m_getRect().intersectsRect(gumba->m_getRect()))
					{
						if (Hero->strong == false)
						{
							runupdate = false;
							if (HeroInfo != 1)
							{

								auto ac = CallFunc::create(CC_CALLBACK_0(GameScene::setRunUpdate, this));
								this->runAction(Sequence::create(DelayTime::create(0.6), ac, NULL));
							
							}
							
								Hero->m_Downgrade();
						}
						
						break;
					}
				}


				if (gumba->del == true)
				{
					gumba->m_dead();
					gumbav.eraseObject(gumba);
					break;
				}
			}
		}


		if (turtlev.empty() == false)
		{

			for (M_Tuttle * gumba : turtlev)
			{

				gumba->m_update(dt, GameLayer->getPosition().x);

				if (Hero->m_getRect().intersectsRect(gumba->m_getRect()) && Hero->star == true)
				{
					gumba->del = true;
					gumba->burn = true;
				}else if (gumba->del == false)
				{
					if (Hero->falling == true)
					{
						if (Hero->m_getFootRect().intersectsRect(gumba->m_getRect()))
						{
							Hero->m_killjump();
							gumba->del = true;
							break;
						}
						else if (Hero->m_getRect().intersectsRect(gumba->m_getRect()))
						{
							if (Hero->strong == false)
							{
								runupdate = false;
								if (HeroInfo != 1)
								{
									auto ac = CallFunc::create(CC_CALLBACK_0(GameScene::setRunUpdate, this));
									this->runAction(Sequence::create(DelayTime::create(0.6), ac, NULL));
								}

								Hero->m_Downgrade();
							}

							break;
						}
					}
					else if (Hero->m_getRect().intersectsRect(gumba->m_getRect()))
					{
						if (Hero->strong == false)
						{
							runupdate = false;
							if (HeroInfo != 1)
							{

								auto ac = CallFunc::create(CC_CALLBACK_0(GameScene::setRunUpdate, this));
								this->runAction(Sequence::create(DelayTime::create(0.6), ac, NULL));

							}

							Hero->m_Downgrade();
						}

						break;
					}
				}


				if (gumba->del == true)
				{
					gumba->m_dead();
					turtlev.eraseObject(gumba);
					break;
				}
			}
		}
	}
}

void GameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	//이런 식으로 키 값에 따라 처리를 하면 된다
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		Hero->m_jump();
		break;
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		Hero->m_touchLeft();
		break;
	//case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	//	FallingBlock();
	//	break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		Hero->m_touchRight();
		break;
	case EventKeyboard::KeyCode::KEY_X:
		Hero->m_touchShoot();
		break;

	}
}

void GameScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		Hero->m_releaseJump();
		break;
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		Hero->m_releaseLeft();
		break;
		//case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		//	FallingBlock();
		//	break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		Hero->m_releaseRight();
		break;
	}
}

void GameScene::ClearAnimation()
{
	auto ac = MoveBy::create(1.0f, Vec2(0, -128));
	Flag->runAction(ac);

	auto ac1 = MoveBy::create(0.5f, Vec2(0, 32));
	auto seq = Sequence::create(DelayTime::create(3.5f),ac1,NULL);
	RedFlag->runAction(seq);
}

void GameScene::onTouchesBegan(const std::vector<Touch*>& touches, Event* unused_event)
{

	std::vector<Touch*>::const_iterator it = touches.begin();
	Touch *touch;
	Point location[2];

	for (int i = 0; i < touches.size(); i++)
	{
		touch = (Touch*)(*it);
		location[i] = touch->getLocation();
		it++;


	}


}

void GameScene::onTouchesMoved(const std::vector<Touch*>& touches, Event* unused_event)
{
	std::vector<Touch*>::const_iterator it = touches.begin();
	Touch *touch;
	Point location[2];

	for (int i = 0; i < touches.size(); i++)
	{
		touch = (Touch*)(*it);
		location[i] = touch->getLocation();
		it++;


	}

}

void GameScene::onTouchesEnded(const std::vector<Touch*>& touches, Event* unused_event)
{
	std::vector<Touch*>::const_iterator it = touches.begin();
	Touch *touch;
	Point location[2];

	for (int i = 0; i < touches.size(); i++)
	{
		touch = (Touch*)(*it);
		location[i] = touch->getLocation();
		it++;


	}

}

void GameScene::setRunUpdate()
{
	runupdate = true;
}

void GameScene::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
	return;
#endif

	Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
