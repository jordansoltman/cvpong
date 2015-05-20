#ifndef MOTIONPADDLEDECTOR_CPP
#define MOTIONPADDLEDECTOR_CPP
#include "MotionPaddleDetector.h"

MotionPaddleDector::MotionPaddleDector(VideoCapture* vid) : PaddleDetector() {
	m_objBoundingRect = Rect(0, 0, 0, 0);
	m_leftPaddle[0] = 0;
	m_leftPaddle[1] = 0;
	m_rightPaddle[0] = 0;
	m_rightPaddle[1] = 0;
	m_vid = vid;
}

PaddleDetector::PaddlePositions MotionPaddleDector::processFrame(Mat& frame) {
	PaddlePositions pd(3, 3);
	// capture sequential images as frame1 and frame2 for movement
	// comparision

	// read in frame1 and convert to grayscale
	m_vid->read(m_frame1);
	cvtColor(m_frame1, m_gray1, COLOR_BGR2GRAY);

	// read in frame2 and convert to grayscale
	m_vid->read(m_frame2);
	cvtColor(m_frame2, m_gray2, COLOR_BGR2GRAY);

	// create difference image of frame1 and frame2 after being converted to
	// grayscale images
	absdiff(m_gray1, m_gray2, m_differenceImg);

	// threshold difference image at a given sensitivity value
	threshold(m_differenceImg, m_thresholdImg, THRESHOLD_SENSITIVITY, 255, THRESH_BINARY);

	// blur the image to get rid of the noise. This will output an intensity image
	blur(m_thresholdImg, m_thresholdImg, cv::Size(BLUR_SIZE, BLUR_SIZE));

	// threshold again to obtain binary image from blur output
	threshold(m_thresholdImg, m_thresholdImg, THRESHOLD_SENSITIVITY, 255, THRESH_BINARY);

	// detect motion
	detectMotion(m_thresholdImg, m_frame1);

	frame = m_frame1;

	return(pd);
}

int MotionPaddleDector::getLeftPaddleLoc() {
	return(m_leftPaddle[1]);
}

int MotionPaddleDector::getRightPaddleLoc() {
	return(m_rightPaddle[1]);
}

void MotionPaddleDector::detectMotion(Mat thresholdImage, Mat& cameraFeed) {
	bool objectDetected = false;
	Mat temp;
	thresholdImage.copyTo(temp);

	// these two vectors needed for output of findContours
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	// find contours of filtered image using openCV findContours function
	// findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );// retrieves all contours
	findContours(temp, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);// retrieves external contours

	// if contours vector is not empty, we have found some objects
	objectDetected = contours.size() > 0 ? true : false;

	if(objectDetected){
		// the largest contour is found at the end of the contours vector
		// we will simply assume that the biggest contour is the object we are looking for.
		vector<vector<Point>> largestContourVec;
		largestContourVec.push_back(contours.at(contours.size() - 1));

		// make a bounding rectangle around the largest contour then find its centroid
		// this will be the object's final estimated position.
		m_objBoundingRect = boundingRect(largestContourVec.at(0));
		int xpos = m_objBoundingRect.x + m_objBoundingRect.width / 2;
		int ypos = m_objBoundingRect.y + m_objBoundingRect.height / 2;

		//update left paddle's positions
		m_leftPaddle[0] = xpos, m_leftPaddle[1] = ypos;

		//update right paddle's positions
		m_rightPaddle[0] = xpos, m_rightPaddle[1] = ypos;
	}

	int x = m_leftPaddle[0];
	int y = m_leftPaddle[1];

	//draw crosshairs around the object
	circle(cameraFeed, Point(x, y), 20, Scalar(0, 255, 0), 2);
	line(cameraFeed, Point(x, y), Point(x, y - 25), Scalar(0, 0, 255), 2);
	line(cameraFeed, Point(x, y), Point(x, y + 25), Scalar(0, 0, 255), 2);
	line(cameraFeed, Point(x, y), Point(x - 25, y), Scalar(0, 0, 255), 2);
	line(cameraFeed, Point(x, y), Point(x + 25, y), Scalar(0, 0, 255), 2);

	//write the position of the object to the screen
	//putText(cameraFeed, "Tracking object at (" + std::to_string(x) + "," + std::to_string(y) + ")", Point(x, y), 1, 1, Scalar(255, 0, 0), 2);
}

#endif