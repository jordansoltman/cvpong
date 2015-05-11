/*
 * PaddleDectector is an abstract class that requires processFrame() to be overridden
 */

#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

class PaddleDetector
{
public:

	static const int UNKNOWN_PADDLE_POSITION = -1;

	struct PaddlePositions {
		int leftPaddlePosition = UNKNOWN_PADDLE_POSITION;
		int rightPaddlePosition = UNKNOWN_PADDLE_POSITION;
		PaddlePositions(int l, int r) : leftPaddlePosition(l), rightPaddlePosition(r){};
	};

	PaddleDetector() {};
	virtual ~PaddleDetector() {};

	/*
	 *
	 * Abstract method process frame
	 *
	 * Preconditions:	Frame will be a vaild mat object with one frame of video
	 * Postconditions:	Should return a PaddlePositions struct that contains the two
	 * 					positions of the left and right paddles
	 */
	virtual PaddlePositions processFrame(Mat& frame) = 0;

	virtual int getLeftPaddleLoc() = 0;
	virtual int getRightPaddleLoc() = 0;
	virtual void configure() {};
};

