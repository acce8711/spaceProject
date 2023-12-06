#include "Projectile.h"

void Projectile::setup(ofVec2f pos, float angle, float speed)
{
	m_projectileSpeed = 6.0f;
	m_projectilePos = pos;
	m_projectileAngle = ofDegToRad(angle - 90.0f);
	m_xSpeed = m_projectileSpeed * cos(m_projectileAngle);
	m_ySpeed = m_projectileSpeed * sin(m_projectileAngle);
 	m_radius = 10.0f;
	m_isVisible = true;
}

void Projectile::update()
{
	m_projectilePos.y += m_ySpeed;
	m_projectilePos.x += m_xSpeed;
}

void Projectile::draw()
{
	if (m_isVisible)
	{
		ofSetColor(255);
		ofPushMatrix();
		{
			ofTranslate(m_projectilePos);
			ofSetRectMode(OF_RECTMODE_CENTER);
			ofDrawCircle(0, 0, m_radius);
			ofSetRectMode(OF_RECTMODE_CORNER);
		}
		ofPopMatrix();
	}

}
