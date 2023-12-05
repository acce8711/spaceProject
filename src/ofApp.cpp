#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowShape(Config::APP_WINDOW_WIDTH, Config::APP_WINDOW_HEIGHT);
	ofSetFrameRate(Config::APP_DESIRED_FRAMERATE);


	//setup Arduino
	// replace the string below with the serial port for your Arduino board
	// you can get this from the Arduino application (Tools Menu -> Port) 
	//m_arduino.connect("COM4", 57600);
	m_arduino.connect(Config::ARDUINO_DEVICE_NAME, 57600);

	m_input_val = Config::IR_MIN_READING;
	m_irReadingLeft = Config::IR_MIN_READING;
	m_irReadingLeftCurr = Config::IR_MIN_READING;
	m_propellerLeft = 0;
	m_lerpLeft = false;
	m_lerpRight = false;

	// Listen for EInitialized notification. this indicates that
	// the arduino is ready to receive commands and it is safe to
	// call setupArduino()
	ofAddListener(m_arduino.EInitialized, this, &ofApp::setupArduino);

	m_bSetup = false;

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
	//m_gui.setup();

	

}

//--------------------------------------------------------------
void ofApp::update(){
	//cout << m_gameMode << endl;

	updateArduino();
	if (m_lerpLeft)
	{
		lerpIRData();
		IRDataToAction(m_propellerLeft, PIN_PWM_OUTPUT, PIN_OUTPUT_VIBRATION);
	}
	if (m_lerpRight)
	{
		lerpIRDataRight();
		IRDataToAction(m_propellerRight, PIN_PWM_OUTPUT_RIGHT, PIN_OUTPUT_VIBRATION_RIGHT);
	}

	if (m_gameMode == playing)
	{
		/**/
		m_spaceshipSpeed = - 0.5f *m_propellerLeft - 0.5f *m_propellerRight;
		//int ySpeed = m_propellerLeft + m_propellerRight;
		m_spaceshipAngle = ofLerp(m_spaceshipAngle,m_propellerRight*7 - m_propellerLeft*7, 0.05f);
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
			/*
			if (abs(m_spaceshipSpeed) > 5.0f)
			{
				m_spaceshipSpeed = -5.5f;
			}
			if (m_spaceshipSpeed < 0.0f)
				m_spaceshipSpeed -= 0.02f;
			else if (m_spaceshipSpeed > 0.0f)
				m_spaceshipSpeed = 0.0f;*/
			float xspeed = m_spaceshipSpeed * (sin(ofDegToRad(m_spaceshipAngle)));
			//float yspeed = m_spaceshipSpeed * abs(sin(ofDegToRad(m_spaceshipAngle)));
			float yspeed = m_spaceshipSpeed;
			float degrees = ofRadToDeg(ofDegToRad(m_spaceshipAngle));

			
			//cout <<yspeed << endl;
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
				ofSetColor(255);
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
			//cout << m_spaceshipAngle << endl;
		}
		if (key == ' ')
		{
			if(ofGetElapsedTimef() - m_lastShotTime > 0.5f)
			{
				Projectile p;
				//cout << m_spaceshipAngle << endl;
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

void ofApp::setupArduino(const int& _version)
{
	/**
	 Look under examples/communication/firmata for more examples ..
	 **/

	m_bSetup = true;

	// remove listener because we don't need it anymore
	ofRemoveListener(m_arduino.EInitialized, this, &ofApp::setupArduino);

	// print firmware name and version to the console
	ofLogNotice() << m_arduino.getFirmwareName();
	ofLogNotice() << "firmata v" << m_arduino.getMajorFirmwareVersion() << "." << m_arduino.getMinorFirmwareVersion();

	//analog input
	m_arduino.sendAnalogPinReporting(PIN_ANALOG_INPUT, ARD_ANALOG);
	m_arduino.sendAnalogPinReporting(PIN_ANALOG_INPUT_RIGHT, ARD_ANALOG);

	//PMW/digital output
	m_arduino.sendDigitalPinMode(PIN_PWM_OUTPUT, ARD_PWM);
	m_arduino.sendDigitalPinMode(PIN_OUTPUT_VIBRATION, ARD_OUTPUT);

	m_arduino.sendDigitalPinMode(PIN_PWM_OUTPUT_RIGHT, ARD_PWM);
	m_arduino.sendDigitalPinMode(PIN_OUTPUT_VIBRATION_RIGHT, ARD_OUTPUT);
	m_arduino.sendDigitalPinMode(PIN_INPUT_BUTTON_RIGHT, ARD_INPUT);

	ofAddListener(m_arduino.EDigitalPinChanged, this, &ofApp::digitalPinChanged);
	ofAddListener(m_arduino.EAnalogPinChanged, this, &ofApp::analogPinChanged);
}

//--------------------------------------------------------------
void ofApp::updateArduino() {

	// update the arduino, get any data or messages.
	// the call to m_arduino.update() is required

	m_arduino.update();
}

void ofApp::digitalPinChanged(const int& pinNum) {
	//cout << m_arduino.getDigital(pinNum) << endl;
	if (pinNum == PIN_INPUT_BUTTON_RIGHT && m_arduino.getDigital(pinNum) ==1)
	{
		std::cout << "digital pin: " + ofToString(pinNum) + " : " + ofToString(m_arduino.getDigital(pinNum)) << std::endl;
		Projectile p;
		//cout << m_spaceshipAngle << endl;
		p.setup(m_spaceshipPos, m_spaceshipAngle, m_spaceshipSpeed);
		m_projectiles.push_back(p);
		m_lastShotTime = ofGetElapsedTimef();
		m_sfxPlayer.load(m_shootSFX);
		m_sfxPlayer.play();
	}
	
}

void ofApp::analogPinChanged(const int& pinNum) {
	//std::cout  << "analog pin: " + ofToString(pinNum) + " : " + ofToString(m_arduino.getAnalog(pinNum)) << std::endl;
	if (pinNum == PIN_ANALOG_INPUT) {
		m_irReadingLeftCurr = ofLerp(m_irReadingLeftCurr, m_arduino.getAnalog(pinNum), 0.5f);

		cout << m_irReadingLeftCurr << endl;
		//making sure the reading is within the range
		//making it equal to the max if it goes above the accepted max value
		if (m_irReadingLeftCurr > Config::IR_MAX_READING)
			m_irReadingLeftCurr = Config::IR_MAX_READING;
		//not storing the sensor reading if an outlier reading is detected

		if (abs(m_irReadingLeftCurr - m_irReadingLeft) < 50 && m_irReadingLeftCurr > Config::IR_MIN_READING && m_irReadingLeftCurr < Config::IR_MAX_READING - 20)
		{
			//m_irReadingLeft = m_irReadingLeftCurr;
			m_lerpLeft = false;
		}
		else
		{
			if (m_irReadingLeftCurr > Config::IR_MAX_READING - 20)
				m_irReadingLeftCurr = Config::IR_MAX_READING + 20;
			if (m_irReadingLeftCurr < Config::IR_MIN_READING)
				m_irReadingLeftCurr = Config::IR_MIN_READING;
			m_lerpLeft = true;
		}
		
		//m_irReadingLeft = ofLerp(m_irReadingLeft, currReading, 0.05f);





		 //send out pmw value



	}

	//detecting right controller movement
	if (pinNum == PIN_ANALOG_INPUT_RIGHT) {

		//get analog value
		m_irReadingRightCurr = ofLerp(m_irReadingRightCurr, m_arduino.getAnalog(pinNum), 0.5f);
		m_irReadingRightCurr = m_arduino.getAnalog(pinNum);
		//cout << m_irReadingRightCurr << endl;
	   //making sure the reading is within the range
	   //making it equal to the max if it goes above the accepted max value
		if (m_irReadingRightCurr > Config::IR_MAX_READING_RIGHT)
			m_irReadingRightCurr = Config::IR_MAX_READING_RIGHT;
		//not storing the sensor reading if an outlier reading is detected
		if (abs(m_irReadingRightCurr - m_irReadingLeft) < 30 && m_irReadingRightCurr > Config::IR_MIN_READING_RIGHT && m_irReadingRightCurr < Config::IR_MAX_READING_RIGHT - 20)
		{
			//m_irReadingLeft = m_irReadingLeftCurr;
			m_lerpRight = false;
		}
		else
		{
			if (m_irReadingRightCurr > Config::IR_MAX_READING_RIGHT - 20)
				m_irReadingRightCurr = Config::IR_MAX_READING_RIGHT + 20;
			if (m_irReadingRightCurr < Config::IR_MIN_READING_RIGHT)
				m_irReadingRightCurr = Config::IR_MIN_READING_RIGHT;
			m_lerpRight = true;
		}

		//m_irReadingLeft = ofLerp(m_irReadingLeft, currReading, 0.05f);


		 //send out pmw value



	}


}



void ofApp::lerpIRData()
{
	m_irReadingLeft = ofLerp(m_irReadingLeft, m_irReadingLeftCurr, 0.04f);
	m_propellerLeft = (int)ofMap(m_irReadingLeft, Config::IR_MIN_READING, Config::IR_MAX_READING - 20, Config::PROPELLER_INTESITY_MIN, Config::PROPELLER_INTESITY_MAX, true);
	
	//m_spaceshipSpeed -=m_propellerLeft;

	//m_spaceshipAngle = 7 * m_propellerLeft;
	
}


void ofApp::lerpIRDataRight()
{
	m_irReadingRight = ofLerp(m_irReadingRight, m_irReadingRightCurr, 0.04f);
	m_propellerRight = (int)ofMap(m_irReadingRight, Config::IR_MIN_READING_RIGHT, Config::IR_MAX_READING_RIGHT - 20, Config::PROPELLER_INTESITY_MIN, Config::PROPELLER_INTESITY_MAX, true);
	
}

void ofApp::IRDataToAction(int propellerValue, int pinPwm, int pinVibration)
{
	if (playing)
	{
		m_arduino.sendPwm(pinPwm, ofMap(propellerValue, 0, 10, 0, 255, true));
		if (propellerValue != 0)
			m_arduino.sendDigital(pinVibration, ARD_HIGH);
		else
			m_arduino.sendDigital(pinVibration, ARD_LOW);
	}
	
}
