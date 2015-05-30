/*
 * PaddleDectector is an abstract class that requires processFrame() to be overridden
 */

#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
const Scalar RED(0, 0, 255);
const Scalar BLUE(255, 0, 0);

/*
* Abstract class PaddleDetector
*
* 
*/
class PaddleDetector
{
	
public:

	static const int DEFAULT_PADDLE_POSITION = 0;

	PaddleDetector() {};
	
	virtual ~PaddleDetector() {};

	/*
	 * Abstract method process frame
	 *
	 * Preconditions:	Frame will be a vaild mat object with one frame of video
	 * Postconditions:	Sets the paddle positions of the left and right paddles
	 */
	virtual void processFrame(Mat& frame) = 0;

	/*
	* Abstract get left paddle location
	*
	* Preconditions:	none
	* Postconditions:	returns the location of the left paddle
	*/
	virtual int getLeftPaddleLoc() = 0;

	/*
	* Abstract get right paddle location
	*
	* Preconditions:	none
	* Postconditions:	returns the location of the right paddle
	*/
	virtual int getRightPaddleLoc() = 0;

	/*
	* Abstract configure
	*
	* Preconditions:	none
	* Postconditions:	configures the tracking setting
	*/
	virtual void configure() {};

protected:
	int m_leftPaddlePos;
	int m_rightPaddlePos;
};

