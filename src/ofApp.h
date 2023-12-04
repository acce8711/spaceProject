#pragma once

#include "ofMain.h"
#include "Config.h"
#include "Projectile.h"
#include "Rock.h"
//#include "ofxImGui.h"
#include "ofEvents.h"

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


		vector<Projectile> m_projectiles;
		vector<Star> m_stars;
		vector<Rock> m_rocks;
		int m_frames;
		bool m_crashed;
		float m_crashTime;
		float m_lastShotTime;
		ofVec2f m_crashPos;
		int m_starsCollected;
		int m_rocksHit;
		float m_pressTime;

		//images
		ofImage m_startMenu;
		ofImage m_instructionsImage;
		ofImage m_playImage;
		ofImage m_gameOverImage;
		ofImage m_spaceshipImage;
		ofImage m_starUI;
		ofImage m_rockUI;
		ofImage m_explosionImage;

		//sounds
		ofSoundPlayer m_musicPlayer;
		ofSoundPlayer m_sfxPlayer;
		std::string m_hitSFX;
		std::string m_shootSFX;
		std::string m_collectSFX;

		//Font
		ofTrueTypeFont m_slackeyFont;

		//ImGUI
		//ofxImGui::Gui m_gui;

		//game modes
		int m_gameMode;
		enum GAME_MODE {start, instructions, playing, gameover};

		void setup();
		void update();
		void draw();

		


		//ARDUINO
		static const int PIN_ANALOG_INPUT = 5;
		static const int PIN_PWM_OUTPUT = 3;
		static const int PIN_OUTPUT_VIBRATION = 4;

		static const int PIN_ANALOG_INPUT_RIGHT = 4;
		static const int PIN_PWM_OUTPUT_RIGHT = 9;
		static const int PIN_OUTPUT_VIBRATION_RIGHT = 8;
		static const int PIN_INPUT_BUTTON_RIGHT = 2;


		//Arduino stuff
		ofArduino				m_arduino;
		ofTrueTypeFont          m_font;
		int					    m_input_val;	//sensor value
		bool					m_bSetup;       //is Arduinio initialized yet
		void setupArduino(const int& _version);
		void updateArduino();
		void digitalPinChanged(const int& pinNum);
		void analogPinChanged(const int& pinNum);

		//IR sensor variables / new added
		int m_irReadingLeft;
		int m_irReadingLeftCurr;
		float m_propellerLeft;


		int m_irReadingRight;
		int m_irReadingRightCurr;
		float m_propellerRight;


		bool m_lerpLeft;
		bool m_lerpRight;


		float getIRDistance(int& val);
		void lerpIRData();
		void lerpIRDataRight();
		void IRDataToAction(int propellerValue, int pinPwm, int pinVibration);

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);


		void createRock();
		void createStars();
		void updateProjectiles();
		void updateRocks();
		void updateStars();
		void checkRockCollision();
		void checkProjectileCollision();
		void checkStarCollision();
		void resetGame();
};
