#pragma once
#include "PaddleDetector.h"
class ColorPaddleDetector :
	public PaddleDetector
{
private:

	int lowHue = 17;
	int highHue = 75;

	int lowSaturation = 15;
	int highSaturation = 244;

	int lowValue = 129;
	int highValue = 255;

	int leftPaddlePos = 0;
	int rightPaddlePos = 0;

	VideoCapture *vid;

	void thresholdImage(Mat &frame, Mat &destination);

public:
	ColorPaddleDetector() {};
	ColorPaddleDetector(VideoCapture *vid);
	~ColorPaddleDetector();
	PaddleDetector::PaddlePositions ColorPaddleDetector::processFrame(Mat &frame);
	int getLeftPaddleLoc();
	int getRightPaddleLoc();
	void configure();
};

