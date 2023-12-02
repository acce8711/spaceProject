#pragma once

#include "ofMain.h"
#include "Config.h"

class Rock
{
public:
	ofVec2f m_rockPos;
	float m_rockSpeed;
	float m_rockAngle;
	int m_rockType;
	ofImage m_rockImage[5];
	ofImage m_explodedRockImage;

	//rocks will be spawned from the top of the screen at intervals
	//rocks will rotate and move down
	//there will be a variety of rocks
	//the rock will be drawn as an exploded rock when colliding with a projectile or rocket

	//rock update function
	//move the rock down
	//rotate the rock
	//check for collision with any projectiles or the rocket

	//rock draw function
	//draw the rock at the position and rotation

	void setup();
	void update();
	void draw();
};