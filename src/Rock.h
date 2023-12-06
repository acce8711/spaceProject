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
	bool m_isVisible;

	void setup();
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