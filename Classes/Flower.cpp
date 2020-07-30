#include "stdafx.h"

void Flower::m_init(Vec2 Pos, Layer * layer, Vector<Block*> *v, Vector<Flower*> *mv)
{
	mush = Sprite::create();
	mush->setPosition(Pos.x + 8, Pos.y);
	mush->setAnchorPoint(Vec2(0.5, 0));
	layer->addChild(mush, -1);
	mush->setVisible(false);

	Vector<SpriteFrame *>aniFrames1;
	char str1[256];
	for (int i = 0; i < 4; i++)
	{
		sprintf(str1, "flower%d.png", i);

		SpriteFrame * frame1 = SpriteFrameCache::getInstance()->getSpriteFrameByName(str1);
		aniFrames1.pushBack(frame1);
	}
	Animation * ani = Animation::createWithSpriteFrames(aniFrames1, 0.1, 1);
	Animate * animate = Animate::create(ani);
	auto * seq = Sequence::create(animate, NULL);


	mush->runAction(CCRepeatForever::create(seq));
	auto cc = CallFunc::create(CC_CALLBACK_0(Flower::m_setVisivle, this));
	auto ss = Sequence::create(DelayTime::create(0.13), cc, NULL);
	mush->runAction(ss);


	charHigh = 16;
	moving = false;
	GLayer = layer;

	Info = 1;
	del = false;

	auto ac = MoveBy::create(0.5, Vec2(0, 16));
	auto seqq = Sequence::create(ac, NULL);
	mush->runAction(seqq);

	bvv = v;
	mushv = mv;
}


void Flower::m_update(float dt, float MapPosX)
{

	if (del == true)
	{
		
		mushv->eraseObject(this);
	}
}

void Flower::removethis()
{
	mush->setVisible(false);
	del = true;
}

Rect Flower::m_getRect()
{
	return mush->boundingBox();
}

void Flower::m_setVisivle()
{
	mush->setVisible(true);
}