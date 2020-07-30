#include "stdafx.h"

void M_Camera::init(Layer *pTargetLayer, Size mapSize, Size screenSize)
{
	m_pTargetLayer = pTargetLayer;

	m_mapSize = mapSize;

	m_screenSize = screenSize;
}

void M_Camera::update(Vec2 objPos,Vec2 scenePos)
{
	Vec2 mapPos;

	if (objPos.x < m_screenSize.width / 2)
		mapPos.x = 0.f;
	else if (objPos.x > m_mapSize.width - m_screenSize.width / 2)
		mapPos.x = m_mapSize.width - m_screenSize.width;
	else
		mapPos.x = objPos.x - m_screenSize.width / 2;

	/*if (objPos.y < m_screenSize.height / 2)
		mapPos.y = 0.f;
	else if (objPos.y > m_mapSize.height - m_screenSize.height / 2)
		mapPos.y = m_mapSize.height - m_screenSize.height;
	else
		mapPos.y = objPos.y - m_screenSize.height / 2;
*/
	//log("m      %f", abs(scenePos.x));
	if (mapPos.x > abs(scenePos.x))
		m_pTargetLayer->setPosition(-mapPos);
}