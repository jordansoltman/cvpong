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
	int getLeftPaddleLoc() {return(m_leftPaddlePos);}

	/*
	* Abstract get right paddle location
	*
	* Preconditions:	none
	* Postconditions:	returns the location of the right paddle
	*/
	int getRightPaddleLoc() {return(m_rightPaddlePos);}

	/*
	* Abstract configure
	*
	* Preconditions:	none
	* Postconditions:	configures the tracking setting
	*/
	virtual void configure() {};

protected:
	/*
	* m_leftPaddlePos
	* contains the y-value of the object tracked in the left half of the frame being processed
	*/
	int m_leftPaddlePos;

	/*
	* m_rightPaddlePos
	* contains the y-value of the object tracked in the right half of the frame being processed
	*/
	int m_rightPaddlePos;
};

