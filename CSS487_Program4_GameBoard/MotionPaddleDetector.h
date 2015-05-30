#ifndef MotionPaddleDetector_H
#define MotionPaddleDetector_H
#include "PaddleDetector.h"

class MotionPaddleDetector : public PaddleDetector {
	static const int THRESHOLD_SENSITIVITY = 20;
	static const int BLUR_SIZE = 10;
public:
	MotionPaddleDetector(VideoCapture* vid);

	~MotionPaddleDetector() {}
	virtual PaddlePositions processFrame(Mat& frame);

	int getLeftPaddleLoc();
	int getRightPaddleLoc();

private:
	void detectMotionLeft(Mat& thresholdImage, Mat& left);
	void detectMotionRight(Mat& thresholdImage, Mat& right);

	vector<vector<Point>> m_prevContour;
	int m_leftPaddlePos;
	int m_rightPaddlePos;
	VideoCapture* m_vid;
};

#endif