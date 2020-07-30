#pragma once

#include "stdafx.h"

class GameScene : public LayerColor
{
private:
	EventListenerTouchAllAtOnce *listener;
	
	Sprite * Castle1;
	Sprite * Castle2;
	Sprite * RedFlag;
	Rect FlagRect;
	//Layer * GameLayer;
	M_Camera   *m_pCamera;
	Mario * Hero;
	float pressJumpTime;
	
	int makingNum;

	bool runupdate;
	bool GameClear;
public:



	static Scene* createScene();
	virtual bool init();
	void menuCloseCallback(Ref* pSender);

	void Update(float dt);
	void setRunUpdate();

	void ClearAnimation();

	void MakeMap();

	CREATE_FUNC(GameScene);

	virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event* unused_event);
	virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event* unused_event);
	virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event* unused_event);

	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

};