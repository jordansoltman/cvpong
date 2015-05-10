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
	void detectMotion(Mat thresholdImage, Mat &cameraFeed);

	Rect m_objBoundingRect;
	int m_leftPaddle[2];
	int m_rightPaddle[2];
	Mat m_frame1;
	Mat m_frame2;
	Mat m_gray1;
	Mat m_gray2;
	Mat m_differenceImg;
	Mat m_thresholdImg;

	VideoCapture* m_vid;
};

#endif