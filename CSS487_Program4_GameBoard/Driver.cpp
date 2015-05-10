#include <iostream>
#include "GameBoard.h"
#include "MotionPaddleDetector.h"
using namespace std;

int main() {
	VideoCapture cap(0);
	if(!cap.isOpened()) {
		return(-1);
	}

	MotionPaddleDector MPD(&cap);
	GameBoard pong;
	Mat frame;
	while(pong.gameOn()) {
		cap >> frame;
		MPD.processFrame(frame);
		pong.play(frame, MPD.getLeftPaddleLoc(), MPD.getRightPaddleLoc());
		int key = waitKey(30);
		if (key == 27) break; // If 'esc' key is pressed we'll quit
	}
	
	//GameBoard pong;
	//Mat frame;
	//while(pong.gameOn()) {
	//	cap >> frame;
	//  // needs left and right paddle locs to be passed in
	//	pong.play(frame);
	//	int key = waitKey(30);
	//	if (key == 27) break; // If 'esc' key is pressed we'll quit
	//}
	
	return(0);
};