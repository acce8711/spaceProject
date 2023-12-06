#include "Rock.h"


void SpatialObject::setup()
{
	m_isVisible = true;
}

void SpatialObject::update()
{
	m_pos.y += m_speed;
	m_angle += 4.0f;
}



void Rock::setup()
{
	m_rockImage[0].load("rock1.png");
	m_rockImage[1].load("rock2.png");
	m_rockImage[2].load("rock3.png");
	m_type = ofRandom(0, 3);

	m_angle = 0.0f;
	m_speed = 2.0f;
	m_radius = m_rockImage[0].getWidth() * 0.95f;
	switch (m_type)
	{
	case 0:
		m_radius = 40.0f;
		break;
	case 1:
		m_radius = 52.0f;
		break;
	case 2: 
		m_radius = 30.0f;
		break;
	}
	m_pos = ofVec2f(ofRandom(0, Config::APP_WINDOW_WIDTH), 0.0f);
}

void Rock::draw()
{
	if (m_isVisible)
	{
		ofPushMatrix();
		{
			ofSetColor(255);
			ofSetRectMode(OF_RECTMODE_CENTER);
			ofTranslate(m_pos);
			ofRotateDeg(m_angle);
			switch (m_type)
			{
			case 0:
				m_rockImage[0].draw(0, 0);
				break;
			case 1:
				m_rockImage[1].draw(0, 0);
				break;
			case 2:
				m_rockImage[2].draw(0, 0);
				break;
			}
			ofSetRectMode(OF_RECTMODE_CORNER);
		}
		ofPopMatrix();
	}
}

void Star::setup()
{
	m_starImage.load("starPlaceholder.png");
	m_angle = 0.0f;
	m_speed = 2.0f;
	m_radius = 40.0f;
	m_pos = ofVec2f(ofRandom(0, Config::APP_WINDOW_WIDTH), 0.0f);
}

void Star::draw()
{
	ofPushMatrix();
	{
		ofSetColor(255);
		ofSetRectMode(OF_RECTMODE_CENTER);
		ofTranslate(m_pos);
		m_starImage.draw(0,0);
		ofSetRectMode(OF_RECTMODE_CORNER);
	}
	ofPopMatrix();
}
