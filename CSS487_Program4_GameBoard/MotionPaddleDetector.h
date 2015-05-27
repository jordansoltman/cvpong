#ifndef MOTIONPADDLEDECTOR_H
#define MOTIONPADDLEDECTOR_H
#include "PaddleDetector.h"

class MotionPaddleDector : public PaddleDetector {
	static const int THRESHOLD_SENSITIVITY = 20;
	static const int BLUR_SIZE = 10;
public:
	MotionPaddleDector(VideoCapture* vid);

	~MotionPaddleDector() {}
	virtual PaddlePositions processFrame(Mat& frame);

	int getLeftPaddleLoc();
	int getRightPaddleLoc();

private:
	void detectMotionLeft(Mat thresholdImage, Mat &left);
	void detectMotionRight(Mat thresholdImage, Mat &right);

	int m_leftPaddle[2];
	int m_rightPaddle[2];

	VideoCapture* m_vid;
};

#endif