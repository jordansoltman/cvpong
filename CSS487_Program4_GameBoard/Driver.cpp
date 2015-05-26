#include <iostream>
#include <thread>
#include "GameBoard.h"
#include "MotionPaddleDetector.h"
#include "ColorPaddleDetector.h"
using namespace std;

int main() {

	// Object Tracking: Color
	//
	//VideoCapture cap(0);
	//if(!cap.isOpened()) {
	//	return(-1);
	//}

	//ColorPaddleDetector CPD(&cap);
	//CPD.configure();
	//GameBoard pong;
	//Mat frame, flipped;
	//while(pong.gameOn()) {
	//	cap >> frame;
	//	flip(frame, frame, 1);
	//	CPD.processFrame(frame);
	//	pong.play(frame, CPD.getLeftPaddleLoc(), CPD.getRightPaddleLoc());
	//	int key = waitKey(30);
	//	if (key == 27) break; // If 'esc' key is pressed we'll quit
	//}
	//

	// Object Tracking: Motion
	//
	VideoCapture cap(0);
	if(!cap.isOpened()) {
		return(-1);
	}

	MotionPaddleDector MPD(&cap);
	GameBoard pong;
	Mat frame;

	while(pong.gameOn()) {
		cap >> frame;
		flip(frame, frame, 1);
		MPD.processFrame(frame);
		pong.play(frame, MPD.getLeftPaddleLoc(), MPD.getRightPaddleLoc());
		int key = waitKey(30);
		if(key == 27) {break;} // If 'esc' key is pressed we'll quit
	}
	
	return(0);
};
