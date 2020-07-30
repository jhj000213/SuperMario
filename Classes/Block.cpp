#include "stdafx.h"



void Block::m_init(int x, int y, char info, Layer * layer, void  pfunc(int num, Vec2 Pos))
{

	Solid = false;
	Crash = false;
	upping = false;
	if (info == 'G')
	{
		pfunc(10,Vec2(x*16,y*16));
	}
	else if (info == 'T')
	{
		pfunc(11, Vec2(x * 16, y * 16));
	}
	else if (info == '4')
	{
		hitnum = 0;
		Action = true;
		block = Sprite::create("block6.png");//동전든블록
		block->setPosition(Vec2(x * 16, y * 16));
		block->setAnchorPoint(Vec2(0, 0));
		layer->addChild(block);

		Skin = Sprite::create("block3.png");
		Skin->setAnchorPoint(Vec2(0, 0));
		block->addChild(Skin);

		
	}
	else if (info == '5')//스타
	{
		Action = true;
		block = Sprite::create("block6.png");
		block->setPosition(Vec2(x * 16, y * 16));
		block->setAnchorPoint(Vec2(0, 0));
		layer->addChild(block);

		Skin = Sprite::create("block3.png");
		Skin->setAnchorPoint(Vec2(0, 0));
		block->addChild(Skin);

		
	}
	else if (info == '7')
	{
		Action = true;
		block = Sprite::create("block6.png");//버섯
		block->setPosition(Vec2(x * 16, y * 16));
		block->setAnchorPoint(Vec2(0, 0));
		layer->addChild(block);

		Skin = Sprite::create();
		Skin->setAnchorPoint(Vec2(0, 0));
		block->addChild(Skin);

		Vector<SpriteFrame *>aniFrames1;
		char str1[256];
		for (int i = 0; i < 4; i++)
		{
			sprintf(str1, "itembox%d.png", i);

			SpriteFrame * frame1 = SpriteFrameCache::getInstance()->getSpriteFrameByName(str1);
			aniFrames1.pushBack(frame1);
		}
		Animation * ani = Animation::createWithSpriteFrames(aniFrames1, 0.2f, 1);
		Animate * animate = Animate::create(ani);
		auto * seq = Sequence::create(animate, NULL);
		Skin->runAction(CCRepeatForever::create(seq));

		
	}
	else if (info == '8')
	{
		Action = true;
		block = Sprite::create("block6.png");//히든
		block->setVisible(false);
		block->setPosition(Vec2(x * 16, y * 16));
		block->setAnchorPoint(Vec2(0, 0));
		layer->addChild(block);

	}
	else if (info == 'E')
	{
		char str[256];
		sprintf(str, "block%c.png", info);
		block = Sprite::create(str);
		block->setPosition(Vec2(x * 16 +8, y * 16));
		block->setAnchorPoint(Vec2(0.5, 0));
		layer->addChild(block);

		pfunc(15, Vec2(x * 16 +8, y * 16));
	}
	else if (info != '0')
	{
		Action = false;
		char str[256];
		sprintf(str, "block%c.png", info);
		block = Sprite::create(str);
		block->setPosition(Vec2(x * 16, y * 16));
		block->setAnchorPoint(Vec2(0, 0));
		layer->addChild(block);
		if (info == 'F')
		{
			block->setAnchorPoint(Vec2(0.5, 0));
			block->setPosition(Vec2(x * 16+8, y * 16));
		}

		if (info == '6')
		{
			Action = true;
			Skin = Sprite::create();
			Skin->setAnchorPoint(Vec2(0, 0));
			block->addChild(Skin);

			Vector<SpriteFrame *>aniFrames1;
			char str1[256];
			for (int i = 0; i < 4; i++)
			{
				sprintf(str1, "itembox%d.png", i);

				SpriteFrame * frame1 = SpriteFrameCache::getInstance()->getSpriteFrameByName(str1);
				aniFrames1.pushBack(frame1);
			}
			Animation * ani = Animation::createWithSpriteFrames(aniFrames1, 0.2f, 1);
			Animate * animate = Animate::create(ani);
			auto * seq = Sequence::create(animate, NULL);
			Skin->runAction(CCRepeatForever::create(seq));
		}
	}
	X = x;
	Y = y;
	GLayer = layer;
	Info = info;
	makeobj = pfunc;

}

Rect Block::m_getRect()
{
	return block->boundingBox();
}

Vec2 Block::m_getPosition()
{
	return block->getPosition();
}

bool Block::returninfo()
{
	if (Info != '0' && Info != 'G'&&Info != 'T')
		return true;
	else
		return false;
}

void Block::CrashBlock(int charinfo)
{
	if (Info == '3' && charinfo != 1)
	{
		Crash = true;
		block->setVisible(false);

		Sprite * bb1 = Sprite::create("bb1.png");
		bb1->setPosition(Vec2(block->getPosition().x-4,block->getPosition().y+ 4));
		GLayer->addChild(bb1);
		Sprite * bb2 = Sprite::create("bb2.png");
		bb2->setPosition(Vec2(block->getPosition().x - 4, block->getPosition().y - 4));
		GLayer->addChild(bb2);
		Sprite * bb3 = Sprite::create("bb3.png");
		bb3->setPosition(Vec2(block->getPosition().x + 4, block->getPosition().y + 4));
		GLayer->addChild(bb3);
		Sprite * bb4 = Sprite::create("bb4.png");
		bb4->setPosition(Vec2(block->getPosition().x + 4, block->getPosition().y - 4));
		GLayer->addChild(bb4);

		auto ac1 = JumpBy::create(0.75, Vec2(-80, -216), 130, 1);
		auto ac2 = JumpBy::create(0.75, Vec2(-80, -216), 50, 1);
		auto ac3 = JumpBy::create(0.75, Vec2(80, -216), 130, 1);
		auto ac4 = JumpBy::create(0.75, Vec2(80, -216), 50, 1);

		bb1->runAction(Sequence::create(ac1, RemoveSelf::create(), NULL));
		bb2->runAction(Sequence::create(ac2, RemoveSelf::create(), NULL));
		bb3->runAction(Sequence::create(ac3, RemoveSelf::create(), NULL));
		bb4->runAction(Sequence::create(ac4, RemoveSelf::create(), NULL));
	}
}

void Block::ActionBlock()
{
	if (Solid == false && Crash == false)
	{
		upping = true;
		Vec2 Pos = block->getPosition();
		auto ac = MoveBy::create(0.1f, Vec2(0, 8));
		auto ac1 = MoveBy::create(0.1f, Vec2(0, -8));
		auto call = CallFunc::create(CC_CALLBACK_0(Block::m_setUpping, this));
		block->runAction(Sequence::create(ac, ac1,call, NULL));
	}
		
	
	if (Action == true)
	{
		//
		if (Info == '4')
		{
			hitnum++;
			if (hitnum >= 10)
			{
				block->removeChild(Skin, true);
				Solid = true;
				Action = false;
			}
			Sprite * coin = Sprite::create();
			coin->setPosition(8, 8);
			block->addChild(coin,10);

			auto ac = MoveBy::create(0.2, Vec2(0, 64));
			auto ac1 = MoveBy::create(0.1, Vec2(0,-32));
			auto ss = Sequence::create(ac,ac1, RemoveSelf::create(), NULL);

			Vector<SpriteFrame *>aniFrames1;
			char str1[256];
			for (int i = 0; i < 4; i++)
			{
				sprintf(str1, "coin%d.png", i);

				SpriteFrame * frame1 = SpriteFrameCache::getInstance()->getSpriteFrameByName(str1);
				aniFrames1.pushBack(frame1);
			}
			Animation * ani = Animation::createWithSpriteFrames(aniFrames1, 0.1, 1);
			Animate * animate = Animate::create(ani);
			auto * seq = Sequence::create(animate, NULL);
			coin->runAction(CCRepeatForever::create(seq));
			coin->runAction(ss);
		}
		else if (Info == '5')
		{
			block->removeChild(Skin, true);
			Solid = true;
			Action = false;
			//star
			makeobj(3, Vec2(X* 16, Y * 16));
		}
		else if (Info == '6')
		{
			block->removeChild(Skin,true);
			Solid = true;
			Action = false;
			Sprite * coin = Sprite::create();
			coin->setPosition(8, 8);
			block->addChild(coin, 10);

			auto ac = MoveBy::create(0.2, Vec2(0, 64));
			auto ac1 = MoveBy::create(0.1, Vec2(0, -32));
			auto ss = Sequence::create(ac, ac1, RemoveSelf::create(), NULL);

			Vector<SpriteFrame *>aniFrames1;
			char str1[256];
			for (int i = 0; i < 4; i++)
			{
				sprintf(str1, "coin%d.png", i);

				SpriteFrame * frame1 = SpriteFrameCache::getInstance()->getSpriteFrameByName(str1);
				aniFrames1.pushBack(frame1);
			}
			Animation * ani = Animation::createWithSpriteFrames(aniFrames1, 0.1, 1);
			Animate * animate = Animate::create(ani);
			auto * seq = Sequence::create(animate, NULL);
			coin->runAction(CCRepeatForever::create(seq));
			coin->runAction(ss);
		}
		else if (Info == '7')
		{
			block->removeChild(Skin, true);
			Solid = true;
			Action = false;

			makeobj(1, Vec2(X * 16, Y * 16));
		}
		else if (Info == '8')
		{
			Solid = true;
			Action = false;
			block->setVisible(true);
			//1업
			makeobj(2, Vec2(X * 16, Y * 16));
		}
		
	}
}

bool Block::m_getCrash()
{
	return Crash;
}

void Block::m_setUpping()
{
	upping = false;
}

bool Block::m_getUpping()
{
	return upping;
}