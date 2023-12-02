#pragma once

#include "ofMain.h"
#include "Config.h"
#include "Projectile.h"
#include "Rock.h"
#include "ofxImGui.h"

class ofApp : public ofBaseApp{

	public:

		//spaceship vars

		//to-do
		//spaceship rotation
		//two jets affecting position, velocity, angle
		//collision detection 
		//offscreen boundary
		//projectile shooting from the spaceship

		ofVec2f m_spaceshipPos;
		float m_spaceshipRadius;
		float m_spaceshipAngle;
		float m_spaceshipSpeed;
		float m_leftJetForce;
		float m_rightJetForce;

		vector<Projectile> m_projectiles;
		vector<Rock> m_rocks;
		int m_frames;


		//images

		ofImage m_startMenu;

		//ImGUI
		ofxImGui::Gui m_gui;

		//game modes
		int m_gameMode;
		enum GAME_MODE {start, playing, gameover};

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void createRock();
		void updateProjectiles();
};
