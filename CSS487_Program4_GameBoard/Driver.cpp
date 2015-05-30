#include <iostream>
#include <string>
#include "GameBoard.h"
#include "MotionPaddleDetector.h"
#include "ColorPaddleDetector.h"
using namespace std;

const string MPD_FLAG = "motion";
const string CPD_FLAG = "color";

int main(int argc, char *argv[]) {
	PaddleDetector* sherlock;
	GameBoard pong;
	Mat frame;
	string gametype;

	if(argc < 2) {
		// no command line args, prompt for game type
		cout << "Pick your gametype. Enter \"motion\" or \"color\" to play." << endl;
		cout << "Gametype: ";
		cin >> gametype;

		if(gametype != CPD_FLAG) {
			gametype = MPD_FLAG;
		}
	} else {
		sscanf_s(argv[1], "%s", &gametype);
	}
	cout << "Gametype = " << gametype;
	cout << " ... initializing game ..." << endl;


	// Get videofeed from computer's default camera and set the camer's FPS
	VideoCapture cap(0);
	cap.set(CV_CAP_PROP_FPS, 15);

	// If camera is not on we will exit; cant play without video tracking
	if(!cap.isOpened()) {
		cout << "No camera has been detected, please connect one to play." << endl;
		return(-1);
	}

	if(gametype == CPD_FLAG) {
		sherlock = new ColorPaddleDetector(&cap);
	} else {
		sherlock = new MotionPaddleDetector(&cap);
	}

	while(pong.gameOn()) {
		cap >> frame;
		sherlock->processFrame(frame);
		pong.play(frame, sherlock->getLeftPaddleLoc(), sherlock->getRightPaddleLoc());
		int key = waitKey(30);
		if(key == 27) {break;} // If 'esc' key is pressed we'll quit
	}

	cap.release();
	return(0);
};
