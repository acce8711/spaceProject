#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowShape(Config::APP_WINDOW_WIDTH, Config::APP_WINDOW_HEIGHT);
	ofSetFrameRate(Config::APP_DESIRED_FRAMERATE);


	//init vars
	m_spaceshipPos = ofVec2f(Config::APP_WINDOW_WIDTH / 2.0f, Config::APP_WINDOW_HEIGHT - 400.0f);
	m_spaceshipRadius = 50.0f;
	m_spaceshipAngle = 0.0f;
	m_spaceshipSpeed = 0.0f;

	m_frames = 0;
	m_gameMode = start; //0

	m_starsCollected = 0;
	m_rocksHit = 0;

	m_lastShotTime = 0.0f;
	m_crashTime = 0.0f;

	//load images
	m_startMenu.load("startMenu.png");
	m_instructionsImage.load("instructions.png");
	m_playImage.load("Playing.png");
	m_gameOverImage.load("gameOver.png");
	m_spaceshipImage.load("spaceshipPlaceholder.png");
	m_starUI.load("starUIPlaceholder.png");
	m_rockUI.load("rockPlaceholder");
	m_explosionImage.load("explosion.png");

	//sounds
	m_hitSFX = "hit.wav";
	m_collectSFX = "star.wav";
	m_shootSFX = "shoot.mp3";

	//load font
	m_slackeyFont.load("slackey.ttf", 28);
	
	//ImGUI setup
	m_gui.setup();

}

//--------------------------------------------------------------
void ofApp::update(){
	cout << m_gameMode << endl;
	if (m_gameMode == playing)
	{
		m_frames++;

		//projectiles update
		updateProjectiles();
		//creating new rocks
		createRock();
		createStars();
		updateRocks();
		updateStars();
		//collision checking for rocks with spaceship
		checkRockCollision();
		//collision checking for projectiles and rocks
		checkProjectileCollision();
		//collision checking for stars
		checkStarCollision();

		//spaceship boundary check
		if (m_spaceshipPos.x < 0.0f)
		{
			m_spaceshipPos.x = 0.0f;
		}
		else if (m_spaceshipPos.x > Config::APP_WINDOW_WIDTH)
		{
			m_spaceshipPos.x = Config::APP_WINDOW_WIDTH;
		}
		if (m_spaceshipPos.y < 0.0f)
		{
			m_spaceshipPos.y = 0.0f;
		}
		else if (m_spaceshipPos.y > Config::APP_WINDOW_HEIGHT)
		{
			m_spaceshipPos.y = Config::APP_WINDOW_HEIGHT;
		}

		//spaceship movement
		if (!m_crashed)
		{
			if (abs(m_spaceshipSpeed) > 5.0f)
			{
				m_spaceshipSpeed = -5.5f;
			}
			if (m_spaceshipSpeed < 0.0f)
				m_spaceshipSpeed -= 0.02f;
			else if (m_spaceshipSpeed > 0.0f)
				m_spaceshipSpeed = 0.0f;
			float xspeed = m_spaceshipSpeed * (sin(ofDegToRad(m_spaceshipAngle)));
			float yspeed = m_spaceshipSpeed * abs(sin(ofDegToRad(m_spaceshipAngle)));
			float degrees = ofRadToDeg(ofDegToRad(m_spaceshipAngle));

			m_spaceshipPos.y += yspeed;
			m_spaceshipPos.x += -xspeed;
			m_spaceshipPos.y += 1.0f;	
		}

		//crashing for 1.3 seconds then go to game over
		if (m_crashed && ((ofGetElapsedTimef() - m_crashTime) > 1.3f))
		{
			m_gameMode = gameover;
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

	if (m_gameMode == start)
	{
		m_startMenu.draw(0,0);
	}
	if (m_gameMode == instructions)
	{
		m_instructionsImage.draw(0, 0);
	}
	if (m_gameMode == playing)
	{
		ofSetColor(255);
		m_playImage.draw(0, 0);
		ofSetColor(ofColor::red);
	
		//spaceship matrix
		if (!m_crashed)
		{
			ofPushMatrix();
			{
				ofTranslate(m_spaceshipPos);
				ofRotateDeg(m_spaceshipAngle);
				ofSetRectMode(OF_RECTMODE_CENTER);
				m_spaceshipImage.draw(0, 0);
				ofSetRectMode(OF_RECTMODE_CORNER);
			}
			ofPopMatrix();
		}


		//bullets
		for (int i = 0; i < m_projectiles.size(); i++)
		{
			m_projectiles[i].draw();
		}
		//rocks
		for (int i = 0; i < m_rocks.size(); i++)
		{
			m_rocks[i].draw();
		}
		//stars
		for (int i = 0; i < m_stars.size(); i++)
		{
			m_stars[i].draw();
		}

		ofSetColor(255);
		//UI
		m_slackeyFont.drawString(ofToString(m_starsCollected), 575.0f, 860.0f);

		if (m_crashed)
		{
			ofSetRectMode(OF_RECTMODE_CENTER);
			m_explosionImage.draw(m_crashPos);
			ofSetRectMode(OF_RECTMODE_CORNER);
		}

	}
	if (m_gameMode == gameover)
	{
		ofSetColor(255);
		m_gameOverImage.draw(0, 0);
		m_slackeyFont.drawString(ofToString(m_starsCollected) + " stars collected", Config::APP_WINDOW_WIDTH / 2.0f - 190.0f, 560.0f);
	}

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	if (m_gameMode == playing)
	{
		if (key == 'A' || key == 'a')
		{
			m_spaceshipSpeed -= 0.2f;
			m_spaceshipAngle += 1.0f;
		}
		if (key == 'D' || key == 'd')
		{
			m_spaceshipSpeed -= 0.2f;
			m_spaceshipAngle -= 1.0f;
			cout << m_spaceshipAngle << endl;
		}
		if (key == ' ')
		{
			if(ofGetElapsedTimef() - m_lastShotTime > 0.5f)
			{
				Projectile p;
				cout << m_spaceshipAngle << endl;
				p.setup(m_spaceshipPos, m_spaceshipAngle, m_spaceshipSpeed);
				m_projectiles.push_back(p);
				m_lastShotTime = ofGetElapsedTimef();
				m_sfxPlayer.load(m_shootSFX);
				m_sfxPlayer.play();
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (m_gameMode == start)
	{
		if (key == ' ')
		{
			m_gameMode = instructions;
			m_pressTime = ofGetElapsedTimef();
		}
	}
	if (m_gameMode == instructions)
	{
		if (ofGetElapsedTimef() - m_pressTime > 0.5f)
		{
			if (key == ' ')
			{
				m_gameMode = playing;
			}
			m_pressTime = ofGetElapsedTimef();
		}

	}
	if (m_gameMode == gameover)
	{
		if (key == ' ')
		{
			cout << "hello" << endl;
			resetGame();
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::createRock()
{
	if (m_frames % 100 == 0)
	{
		Rock r; 
		r.setup();
		m_rocks.push_back(r);
	}
}
//--------------------------------------------------------------
void ofApp::createStars()
{
	if (m_frames % 200 == 0)
	{
		Star s;
		s.setup();
		m_stars.push_back(s);
	}
}
//--------------------------------------------------------------
void ofApp::updateProjectiles()
{
	for (int i = 0; i < m_projectiles.size(); i++)
	{
		m_projectiles[i].update();

		if (m_projectiles[i].m_projectilePos.y < 0.0f ||
			m_projectiles[i].m_projectilePos.y > Config::APP_WINDOW_HEIGHT ||
			m_projectiles[i].m_projectilePos.x < 0.0f ||
			m_projectiles[i].m_projectilePos.x > Config::APP_WINDOW_WIDTH
			)
		{
			m_rocksHit++;
			m_projectiles.erase(m_projectiles.begin() + i);
		}
	}
}
//--------------------------------------------------------------
void ofApp::updateRocks()
{
	//rocks update
	for (int i = 0; i < m_rocks.size(); i++)
	{
		m_rocks[i].update();
		if (m_rocks[i].m_pos.y < 0.0f ||
			m_rocks[i].m_pos.y > Config::APP_WINDOW_HEIGHT ||
			m_rocks[i].m_pos.x < 0.0f ||
			m_rocks[i].m_pos.x > Config::APP_WINDOW_WIDTH
			)
		{
			m_rocks.erase(m_rocks.begin() + i);
		}
	}
}
//--------------------------------------------------------------
void ofApp::updateStars()
{
	for (int i = 0; i < m_stars.size(); i++)
	{
		m_stars[i].update();
		if (m_stars[i].m_pos.y < 0.0f ||
			m_stars[i].m_pos.y > Config::APP_WINDOW_HEIGHT ||
			m_stars[i].m_pos.x < 0.0f ||
			m_stars[i].m_pos.x > Config::APP_WINDOW_WIDTH
			)
		{
			m_stars.erase(m_stars.begin() + i);
		}
	}
}

void ofApp::checkRockCollision()
{
	for (int i = 0; i < m_rocks.size(); i++)
	{
		if (ofDist(m_rocks[i].m_pos.x, m_rocks[i].m_pos.y, m_spaceshipPos.x, m_spaceshipPos.y) < (m_rocks[i].m_radius + m_spaceshipRadius))
		{
			if (m_crashed != true)
			{
				m_crashed = true;
				m_crashTime = ofGetElapsedTimef();
				m_sfxPlayer.load(m_hitSFX);
				m_sfxPlayer.play();
			}
			m_crashPos = ofVec2f((m_rocks[i].m_pos + m_spaceshipPos) / 2.0f);
			m_rocks.erase(m_rocks.begin() + i);
		}
	}
}

void ofApp::checkProjectileCollision()
{
	for (int i = 0; i < m_rocks.size(); i++)
	{
		for (int j = 0; j < m_projectiles.size(); j++)
		{
			if (ofDist(m_rocks[i].m_pos.x, m_rocks[i].m_pos.y, m_projectiles[j].m_projectilePos.x, m_projectiles[j].m_projectilePos.y) < (m_rocks[i].m_radius + m_projectiles[j].m_radius))
			{
				ofVec2f collisionPos = ofVec2f(m_rocks[i].m_pos - m_projectiles[j].m_projectilePos);
				m_rocks.erase(m_rocks.begin() + i);
				m_projectiles.erase(m_projectiles.begin() + j);
				m_sfxPlayer.load(m_hitSFX);
				m_sfxPlayer.play();
			}
		}

	}
}

void ofApp::checkStarCollision()
{
	if (!m_crashed)
	{
		for (int i = 0; i < m_stars.size(); i++)
		{
			if (ofDist(m_stars[i].m_pos.x, m_stars[i].m_pos.y, m_spaceshipPos.x, m_spaceshipPos.y) < (m_stars[i].m_radius + m_spaceshipRadius))
			{
				ofVec2f collisionPos = ofVec2f(m_stars[i].m_pos - m_spaceshipPos);
				m_stars.erase(m_stars.begin() + i);
				m_starsCollected++;
				m_sfxPlayer.load(m_collectSFX);
				m_sfxPlayer.play();
			}
		}
	}

}

void ofApp::resetGame()
{
	//reset spaceship
	m_spaceshipPos = ofVec2f(Config::APP_WINDOW_WIDTH / 2.0f, Config::APP_WINDOW_HEIGHT - 400.0f);
	m_spaceshipRadius = 50.0f;
	m_spaceshipSpeed = 0.0f;
	m_spaceshipAngle = 0.0f;

	//reset frame count for star and rock generation
	m_frames = 0;

	//reset points collected
	m_starsCollected = 0;
	m_rocksHit = 0;

	//reset states and object vector arrays
	m_crashed = false;
	m_crashTime = 0.0f;
	m_lastShotTime = 0.0f;
	m_stars.clear();
	m_rocks.clear();
	m_projectiles.clear();


	//change game state
	m_gameMode = playing;
}

