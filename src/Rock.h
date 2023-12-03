#pragma once

#include "ofMain.h"
#include "Config.h"

class SpatialObject
{
public:
	ofVec2f m_pos;
	float m_speed;
	float m_angle;
	float m_radius;

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

	void update();
};

class Rock : public SpatialObject
{
public:
	int m_type;
	ofImage m_rockImage[5];
	ofImage m_explodedRockImage;
	void setup();
	void draw();
};

class Star : public SpatialObject
{
public:
	ofImage m_starImage;
	void setup();
	void draw();
};