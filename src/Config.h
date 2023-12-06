#pragma once

namespace Config {
	static const int APP_WINDOW_WIDTH = 650;
	static const int APP_WINDOW_HEIGHT = 900;
	static const int APP_DESIRED_FRAMERATE = 60;

	static const std::string ARDUINO_DEVICE_NAME = "COM3";		//WIN
	//static const std::string ARDUINO_DEVICE_NAME = "/dev/cu.usbmodem144101";	//MAC

	//IR sensors - new
	const int IR_READING_THRESHOLD = 20;
	const int IR_MIN_READING = 200;
	const int IR_MAX_READING = 500;

	const int IR_MIN_READING_RIGHT = 240;
	const int IR_MAX_READING_RIGHT = 480;

	//spaceship varaibles - new
	const int PROPELLER_INTESITY_MIN = 0;
	const int PROPELLER_INTESITY_MAX = 10;
	const string PROPELLER_LEFT_IMG = "fuelLeft.png";
	const string PROPELLER_RIGHT_IMG = "fuelRight.png";

	//audio
	const string BACKGROUND_MUSIC = "songBg.mp3";

}