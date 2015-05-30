/*
 * PaddleDectector is an abstract class that requires processFrame() to be overridden
 */

#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/opencv.hpp"

using namespace cv;

class PaddleDetector
{
public:

	static const int DEFAULT_PADDLE_POSITION = 0;

	PaddleDetector() {};
	
	virtual ~PaddleDetector() {};

	/*
	 *
	 * Abstract method process frame
	 *
	 * Preconditions:	Frame will be a vaild mat object with one frame of video
	 * Postconditions:	Sets the paddle positions of the left and right paddles
	 */
	virtual void processFrame(Mat& frame) = 0;

	virtual int getLeftPaddleLoc() = 0;
	virtual int getRightPaddleLoc() = 0;
	virtual void configure() {};

protected:
	int m_leftPaddlePos;
	int m_rightPaddlePos;
};

