#include <iostream>
#include <windows.h>
#include <thread>
#include "GameBoard.h"
using namespace std;

void playGame(/*GameBoard& board*/);
void captureVideo(/*VideoCapture& cap*/);

int main() {
	// multithreaded pong code with video capture
	thread t1(playGame);
	thread t2(captureVideo);
	t1.join();
	t2.join();

	// just pong code
	//GameBoard pong;
	//while(pong.gameOn()) {
	//	pong.play();
	//}
	//waitKey(0);
	return(0);
};

void playGame(/*GameBoard& board*/) {
	GameBoard pong;
	while(pong.gameOn()) {
	pong.play();
	}
	waitKey(0);
}

void captureVideo(/*VideoCapture& cap*/) {
	VideoCapture cap(0); // open the default camera
	if(cap.isOpened()) { // check if we succeeded
		namedWindow("edges", 1);
		for(;;) {
			Mat frame;
			cap >> frame; // get a new frame from camera
			imshow("edges", frame);
			if(waitKey(30) >= 0) break;
		}
	}
}


//GameBoard pong;
//VideoCapture cap(0); // open the default camera
//if(!cap.isOpened()) { // check if we succeeded
//	return(-1);
//}

//Mat edges;
//namedWindow("edges", 1);
//while(pong.gameOn()) {
//	Mat frame;
//	cap >> frame; // get a new frame from camera
//	imshow("edges", frame);
//	pong.play();
//}



//Mat test1 = imread("test1.jpg");
//Mat test2 = imread("test2.jpg");
//for(int i = 0; i < 25; i++) {
//	if(i % 2 == 0) {
//		namedWindow("Board");
//		imshow("Board", test1);
//		waitKey(1000);
//	} else {
//		namedWindow("Board");
//		imshow("Board", test2);
//		waitKey(1000);
//	}
//}
//waitKey(1000);