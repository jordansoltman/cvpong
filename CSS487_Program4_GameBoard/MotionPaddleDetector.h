#ifndef MOTIONPADDLEDETECTOR_H
#define MOTIONPADDLEDETECTOR_H
#include "PaddleDetector.h"

class MotionPaddleDetector : public PaddleDetector {
	static const int THRESHOLD_SENSITIVITY = 20;
	static const int BLUR_SIZE = 10;
public:
	MotionPaddleDetector(VideoCapture* vid);

	~MotionPaddleDetector() {}
	virtual void processFrame(Mat& frame);

	int getLeftPaddleLoc();
	int getRightPaddleLoc();

private:
	void detectMotionLeft(Mat& thresholdImage, Mat& left);
	void detectMotionRight(Mat& thresholdImage, Mat& right);

	VideoCapture* m_vid;
};

#endif