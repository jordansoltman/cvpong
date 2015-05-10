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
	}
	waitKey(0);
	return(0);
};