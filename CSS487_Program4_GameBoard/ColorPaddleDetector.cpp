#include "ColorPaddleDetector.h"

ColorPaddleDetector::ColorPaddleDetector(VideoCapture *vid)
{
	m_leftPaddlePos = DEFAULT_PADDLE_POSITION;
	m_rightPaddlePos = DEFAULT_PADDLE_POSITION;
	this->vid = vid;
	configure();
}


ColorPaddleDetector::~ColorPaddleDetector() {}

void ColorPaddleDetector::configure()
{

	Mat frame, thresholded;

	namedWindow("Settings", CV_WINDOW_AUTOSIZE); //create a window called "Control"
	//Create trackbars in "Control" window
	cvCreateTrackbar("Low Hue", "Settings", &lowHue, 179); //Hue (0 - 179)
	cvCreateTrackbar("High Hue", "Settings", &highHue, 179);

	cvCreateTrackbar("Low Saturation", "Settings", &lowSaturation, 255); //Saturation (0 - 255)
	cvCreateTrackbar("High Saturation", "Settings", &highSaturation, 255);

	cvCreateTrackbar("Low Value", "Settings", &lowValue, 255); //Value (0 - 255)
	cvCreateTrackbar("High Value", "Settings", &highValue, 255);

	while (true)
	{
		*vid >> frame;
		flip(frame, frame, 1);
		thresholdImage(frame, thresholded);
		
		imshow("Configure", thresholded);
		int key = waitKey(30);
		if (key != -1) break;
	}

	cvDestroyAllWindows();
}

void ColorPaddleDetector::configureSettings(int e, int x, int y, int flags, void *userData)
{

}

void ColorPaddleDetector::thresholdImage(Mat &f, Mat &destination)
{
	Mat HSV;
	Mat frame = f.clone();

	// Create a HSV version of the grame
	cvtColor(frame, HSV, COLOR_BGR2HSV);

	// Threshold image
	inRange(HSV, Scalar(lowHue, lowSaturation, lowValue), Scalar(highHue, highSaturation, highValue), destination); //Threshold the image

	// remove small objects from the image foreground 
	erode(destination, destination, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	dilate(destination, destination, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	// fill the removed holes in the image foreground 
	dilate(destination, destination, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	erode(destination, destination, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
}


void ColorPaddleDetector::processFrame(Mat &frame)
{
	// need to test this here to make sure it is working properly
	flip(frame, frame, 1);

	Mat thresholded;
	thresholdImage(frame, thresholded);

	// create left and right threshold images for seperate color detection in
	// left and right sides of the frame
	Mat thresholdLeft(thresholded, Rect(0, 0, 320, 480));
	Mat thresholdRight(thresholded, Rect(320, 0, 320, 480));

	// calculate moments in left and right frames
	Moments LeftMoments = moments(thresholdLeft);
	Moments RightMoments = moments(thresholdRight);

	double left_m01 = LeftMoments.m01;
	double left_m10 = LeftMoments.m10;
	double left_area = LeftMoments.m00;

	double right_m01 = RightMoments.m01;
	double right_m10 = RightMoments.m10;
	double right_area = RightMoments.m00;

	if(left_area > 10000)
	{
		// calculate the position of the color being tracke in the left frame
		int x = left_m10 / left_area;
		int y = left_m01 / left_area;

		m_leftPaddlePos = y;

		// draw crosshairs through the point being tracked in the left frame
		circle(frame, Point(x, y), 10, Scalar(0, 0, 255), 2);
		line(frame, Point(x, y), Point(x, y - 15), Scalar(0, 0, 255), 2);
		line(frame, Point(x, y), Point(x, y + 15), Scalar(0, 0, 255), 2);
		line(frame, Point(x, y), Point(x - 15, y), Scalar(0, 0, 255), 2);
		line(frame, Point(x, y), Point(x + 15, y), Scalar(0, 0, 255), 2);
	}

	if(right_area > 10000)
	{
		// calculate the position of the color being tracke in the right frame
		int x = right_m10 / right_area;
		int y = right_m01 / right_area;

		m_rightPaddlePos = y;

		// position the right object appears in the entire frame
		x = (frame.cols / 2) + x;

		// draw crosshairs through the point being tracked in the right frame
		circle(frame, Point(x, y), 10, Scalar(255, 0, 0), 2);
		line(frame, Point(x, y), Point(x, y - 15), Scalar(255, 0, 0), 2);
		line(frame, Point(x, y), Point(x, y + 15), Scalar(255, 0, 0), 2);
		line(frame, Point(x, y), Point(x - 15, y), Scalar(255, 0, 0), 2);
		line(frame, Point(x, y), Point(x + 15, y), Scalar(255, 0, 0), 2);
	}
}

int ColorPaddleDetector::getLeftPaddleLoc()
{
	return m_leftPaddlePos;
}
int ColorPaddleDetector::getRightPaddleLoc()
{
	return m_rightPaddlePos;
}