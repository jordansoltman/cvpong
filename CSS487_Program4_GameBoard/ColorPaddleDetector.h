#pragma once
#include "PaddleDetector.h"
class ColorPaddleDetector :
	public PaddleDetector
{
private:

	int lowHue = 0;
	int highHue = 0;

	int lowSaturation = 0;
	int highSaturation = 0;

	int lowValue = 0;
	int highValue = 0;

	VideoCapture *vid;

	void thresholdImage(Mat &frame, Mat &destination);

	void configureSettings(int e, int x, int y, int flags, void *userData);

public:
	ColorPaddleDetector() {};
	ColorPaddleDetector(VideoCapture *vid);
	~ColorPaddleDetector();
	void ColorPaddleDetector::processFrame(Mat &frame);
	int getLeftPaddleLoc();
	int getRightPaddleLoc();
	void configure();
};

