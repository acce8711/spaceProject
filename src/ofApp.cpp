#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowShape(Config::APP_WINDOW_WIDTH, Config::APP_WINDOW_HEIGHT);
	ofSetFrameRate(Config::APP_DESIRED_FRAMERATE);


	//init vars
	m_spaceshipPos.set(Config::APP_WINDOW_WIDTH / 2.0f, Config::APP_WINDOW_HEIGHT - 100.0f);
	m_spaceshipRadius = 50.0f;
	m_spaceshipAngle = 0.0f;
	m_leftJetForce = 0.0f;
	m_rightJetForce = 0.0f;

	//load images
	m_startMenu.load("spaceStartMenu.png");
	m_gameMode = start; //0
	
	//ImGUI setup
	m_gui.setup();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(255);
	m_startMenu.draw(0, 0);
	ofSetColor(ofColor::red);
	ofDrawCircle(m_spaceshipPos, m_spaceshipRadius - 5.0f);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 'A' || key == 'a')
	{
		m_leftJetForce += 0.5f;
	}
	if (key == 'D' || key == 'd')
	{
		m_leftJetForce += 0.5f;
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

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
