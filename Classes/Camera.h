#pragma once

#include "stdafx.h"

class M_Camera
{
private:
	Layer *m_pTargetLayer;
	Size	m_mapSize;

	Size	m_screenSize;

public:
	void init(Layer *pTargetLayer, Size mapSize, Size screenSize);

	void update(Vec2 objPos, Vec2 scenePos);
};