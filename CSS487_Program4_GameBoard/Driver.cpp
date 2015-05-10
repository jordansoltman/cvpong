#include <iostream>
#include "GameBoard.h"
using namespace std;

int main() {
	VideoCapture cap(0);
	if(!cap.isOpened()) {
		return(-1);
	}
	GameBoard pong;
	Mat frame;
	while(pong.gameOn()) {
		cap >> frame;
		pong.play(frame);
		int key = waitKey(30);
		if (key == 27) break; // If 'esc' key is pressed we'll quit
	}
	
	return(0);
};