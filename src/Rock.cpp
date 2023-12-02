#include "Rock.h"

void Rock::setup()
{
	m_rockImage[0].load("rock1.png");
	m_rockImage[1].load("rock2.png");
	m_rockImage[2].load("rock3.png");
	m_rockImage[3].load("rock4.png");
	m_rockImage[4].load("rock5.png");
	m_rockType = ofRandom(0, 6);
	m_rockAngle = 0.0f;
	m_rockSpeed = 2.0f;
	m_rockPos = ofVec2f(ofRandom(0, Config::APP_WINDOW_WIDTH), 0.0f);
}

void Rock::update()
{
	m_rockPos.y += m_rockSpeed;
}

void Rock::draw()
{
	ofSetColor(255);
	switch (m_rockType)
	{
	case 1:
		m_rockImage[0].draw(m_rockPos);
		break;
	case 2:
		m_rockImage[1].draw(m_rockPos);
		break;
	case 3:
		m_rockImage[2].draw(m_rockPos);
		break;
	case 4:
		m_rockImage[3].draw(m_rockPos);
		break;
	case 5:
		m_rockImage[4].draw(m_rockPos);
		break;
	}
}
