#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowShape(Config::APP_WINDOW_WIDTH, Config::APP_WINDOW_HEIGHT);
	ofSetFrameRate(Config::APP_DESIRED_FRAMERATE);


	//init vars
	m_spaceshipPos = ofVec2f(Config::APP_WINDOW_WIDTH / 2.0f, Config::APP_WINDOW_HEIGHT - 200.0f);
	m_spaceshipRadius = 50.0f;
	m_spaceshipAngle = 0.0f;
	m_spaceshipSpeed = 0.0f;
	m_leftJetForce = 0.0f;
	m_rightJetForce = 0.0f;
	m_frames = 0;

	//load images
	m_startMenu.load("spaceStartMenu.png");
	m_gameMode = start; //0

	
	//ImGUI setup
	m_gui.setup();

	cout << m_spaceshipPos << endl;
}

//--------------------------------------------------------------
void ofApp::update(){
	//cout << "Spaceship speed: " << m_spaceshipSpeed << endl;
	//cout << "Spaceship angle: " << m_spaceshipAngle << endl;
	m_frames++;

	updateProjectiles();
	createRock();

	for (int i = 0; i < m_rocks.size(); i++)
	{
		m_rocks[i].update();
		if (m_rocks[i].m_rockPos.y < 0.0f ||
			m_rocks[i].m_rockPos.y > Config::APP_WINDOW_HEIGHT ||
			m_rocks[i].m_rockPos.x < 0.0f ||
			m_rocks[i].m_rockPos.x > Config::APP_WINDOW_WIDTH
			)
		{
			m_rocks.erase(m_rocks.begin() + i);
		}
	}


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
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(255);
	m_startMenu.draw(0, 0);
	ofSetColor(ofColor::red);
	
	//spaceship matrix
	ofPushMatrix();
	{
		ofTranslate(m_spaceshipPos);
		ofRotateDeg(m_spaceshipAngle);
		ofSetRectMode(OF_RECTMODE_CENTER);
		ofDrawRectangle(0,0, 50.0f, 50.0f);
		ofSetRectMode(OF_RECTMODE_CORNER);
	}
	ofPopMatrix();

	//bullet matrix
	for (int i = 0; i < m_projectiles.size(); i++)
	{
		m_projectiles[i].draw();
	}

	for (int i = 0; i < m_rocks.size(); i++)
	{
		m_rocks[i].draw();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 'A' || key == 'a')
	{
		m_leftJetForce += 0.1f;
		m_spaceshipAngle += 2.0f;
	}
	if (key == 'D' || key == 'd')
	{
		m_rightJetForce += 0.1f;
		m_spaceshipAngle -= 2.0f;
	}
	if (key == ' ')
	{
		if(ofGetElapsedTimef() > 0.5f)
		{
			Projectile p;
			cout << m_spaceshipAngle << endl;
			p.setup(m_spaceshipPos, m_spaceshipAngle, m_spaceshipSpeed);
			m_projectiles.push_back(p);
			ofResetElapsedTimeCounter();
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (key == 'A' || key == 'a')
	{
		m_leftJetForce = 0.0f;
	}
	if (key == 'D' || key == 'd')
	{
		m_rightJetForce = 0.0f;
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
void ofApp::gotMessage(ofMessage msg){

}

void ofApp::createRock()
{
	if (m_frames > 100)
	{
		Rock r; 
		r.setup();
		m_rocks.push_back(r);
		m_frames = 0;
	}
}

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
			m_projectiles.erase(m_projectiles.begin() + i);
		}
	}
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
