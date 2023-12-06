#pragma once

#include "ofMain.h"

class Projectile
{
public:
	ofVec2f m_projectilePos;
	float m_projectileAngle;
	float m_projectileSpeed;
	float m_xSpeed;
	float m_ySpeed;
	float m_radius;
	bool m_isVisible; 
	ofImage* m_projectileImage;
	
	//projectiles will be shot out from the spaceship
	//the projectile angle will be the same as the spaceship at the time of emission

	void setup(ofVec2f pos, float angle, float speed);
	void update();
	void draw();
};

