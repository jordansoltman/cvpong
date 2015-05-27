#ifndef MOTIONPADDLEDECTOR_CPP
#define MOTIONPADDLEDECTOR_CPP
#include "MotionPaddleDetector.h"

MotionPaddleDector::MotionPaddleDector(VideoCapture* vid) : PaddleDetector() {
	m_leftPaddle[0] = 0;
	m_leftPaddle[1] = 0;
	m_rightPaddle[0] = 0;
	m_rightPaddle[1] = 0;
	m_vid = vid;
}

PaddleDetector::PaddlePositions MotionPaddleDector::processFrame(Mat& frame) {
	PaddlePositions pd(3, 3);
	Mat frame2, gray, gray2, thres, diff;

	// capture sequential images as frame1 and frame2 for movement comparison

	// read in frame and convert to grayscale
	m_vid->read(frame);
	cvtColor(frame, gray, COLOR_BGR2GRAY);

	// read in frame2 and convert to grayscale
	m_vid->read(frame2);
	cvtColor(frame2, gray2, COLOR_BGR2GRAY);

	// create difference image of frame1 and frame2 after being converted to
	// grayscale images
	absdiff(gray, gray2, diff);

	// threshold difference image at a given sensitivity value
	threshold(diff, thres, THRESHOLD_SENSITIVITY, 255, THRESH_BINARY);

	// blur the image to get rid of the noise. output will be an intensity image
	blur(thres, thres, cv::Size(BLUR_SIZE, BLUR_SIZE));

	// threshold a second time to get a binary image (after blurring)
	threshold(thres, thres, THRESHOLD_SENSITIVITY, 255, THRESH_BINARY);

	// split frame and threshold into left and right halves
	Mat left(frame, Rect(0, 0, 320, 480));
	Mat thresholdLeft(thres, Rect(0, 0, 320, 480));
	Mat right(frame, Rect(320, 0, 320, 480));
	Mat thresholdRight(thres, Rect(320, 0, 320, 480));

	// detect motion in each 
	detectMotionLeft(thresholdLeft, left);
	detectMotionRight(thresholdRight, right);

	// recombine the left and right frames into the frame being processed
	frame.rowRange(0, 480).colRange(0, 319) = left;
	frame.rowRange(0, 480).colRange(320, 639) = right;

	return(pd);
}

int MotionPaddleDector::getLeftPaddleLoc() {
	return(m_leftPaddle[1]);
}

int MotionPaddleDector::getRightPaddleLoc() {
	return(m_rightPaddle[1]);
}

void MotionPaddleDector::detectMotionLeft(Mat thresholdImage, Mat &left) {
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
		Rect objBoundingRect = boundingRect(largestContourVec.at(0));
		int xpos = objBoundingRect.x + objBoundingRect.width / 2;
		int ypos = objBoundingRect.y + objBoundingRect.height / 2;

		//update left paddle's positions
		m_leftPaddle[0] = xpos, m_leftPaddle[1] = ypos;
	}

	int x = m_leftPaddle[0];
	int y = m_leftPaddle[1];

	//draw crosshairs around the object
	circle(left, Point(x, y), 10, Scalar(0, 0, 255), 2);
	line(left, Point(x, y), Point(x, y - 15), Scalar(0, 0, 255), 2);
	line(left, Point(x, y), Point(x, y + 15), Scalar(0, 0, 255), 2);
	line(left, Point(x, y), Point(x - 15, y), Scalar(0, 0, 255), 2);
	line(left, Point(x, y), Point(x + 15, y), Scalar(0, 0, 255), 2);
}

void MotionPaddleDector::detectMotionRight(Mat thresholdImage, Mat &right) {
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
		Rect objBoundingRect = boundingRect(largestContourVec.at(0));
		int xpos = objBoundingRect.x + objBoundingRect.width / 2;
		int ypos = objBoundingRect.y + objBoundingRect.height / 2;

		//update left paddle's positions
		m_rightPaddle[0] = xpos, m_rightPaddle[1] = ypos;
	}

	int x = m_rightPaddle[0];
	int y = m_rightPaddle[1];

	//draw crosshairs around the object
	circle(right, Point(x, y), 10, Scalar(255, 0, 0), 2);
	line(right, Point(x, y), Point(x, y - 15), Scalar(255, 0, 0), 2);
	line(right, Point(x, y), Point(x, y + 15), Scalar(255, 0, 0), 2);
	line(right, Point(x, y), Point(x - 15, y), Scalar(255, 0, 0), 2);
	line(right, Point(x, y), Point(x + 15, y), Scalar(255, 0, 0), 2);
}

#endif