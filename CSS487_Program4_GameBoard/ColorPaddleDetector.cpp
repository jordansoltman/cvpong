/*
* ColorPaddleDetector class
*
* a class which detects motion in a video frame using color. The color
* used for tracking is configured by the user before tracking beings. Motion
* is tracked seperately in the left and right halves of the video frame.
*
*/
#include "ColorPaddleDetector.h"

/*
* ColorPaddleDetector VideoCapture constructor
*
* preconditions:	vid must be a valid VideoCapture object point not equal to
*					nullptr
* postconditions:	sets the left and right paddles equal to the default position,
*					sets this->vid equal to vid, and initilizes the tracking color
*					configurations
*/
ColorPaddleDetector::ColorPaddleDetector(VideoCapture *vid)
{
	m_leftPaddlePos = DEFAULT_PADDLE_POSITION;
	m_rightPaddlePos = DEFAULT_PADDLE_POSITION;
	this->vid = vid;
	configure();
}

/*
* ColorPaddleDetector destructor
*
* preconditions:	none
* postconditions:	deletes this
*/
ColorPaddleDetector::~ColorPaddleDetector() {}

/*
* configure
*
* launches a configurations windows to configure the color to be tracked
*
* preconditions:	none
* postconditions:	sets the values of lowHue, highHue, lowSat, highSat, lowVal, highVal
*					according to the input from the scroll bars in the configuration window
*/
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

/*
* thresholdImage
*
* creates a threshold image from frame, removes small objects from the foreground
* of the thresholded image and then fills the removed holes. The thresholded 
* image is returned in destination.
*
* preconditions:	frame must be the frame of video currently being processed. 
* postconditions:	creates a thresholded image from frame and returns it in destination
*/
void ColorPaddleDetector::thresholdImage(Mat frame, Mat &destination)
{
	Mat HSV;

	// Create a HSV version of the frame
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

/*
* processFrame
*
* creates a threshold image of frame, splits the thresholded image into left and
* right halves and then tracks for the configured color in each half, setting the
* left and right paddle positions accordingly
*
* preconditions:	frame must be a valid Mat object representing a single frame from
*					from a VideoCapture object
* postconditions:	sets left and right paddles according to color detected in the
*					left and right halves of the frame, respectively
*/
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

	// detect motion in the left and right frames 
	detectMotion(thresholdLeft, frame, IS_RED);
	detectMotion(thresholdRight, frame, IS_BLUE);
}

/*
* detectMotion
*
* detects motion in a thresholded image
*
* preconditions:	thres must be one half (left or right) of the threshold image of the
*					difference image from the sequential frames. frame must be the video
*					frame being processed. isRight should be set true if we are detecting
*					motion in the right frame, otherwise it should be false as we are
*					tracking motion in the left frame.
* postconditions:	sets the paddle position of the paddle indicated by isRight and draws
*					a crosshair around the object being tracked
*/
void ColorPaddleDetector::detectMotion(Mat &thres, Mat &frame, bool isRight) {
	
	Moments Moms = moments(thres);

	double m01 = Moms.m01;
	double m10 = Moms.m10;
	double area = Moms.m00;

	if(area > 10000)
	{
		// calculate the position of the color being tracke in the left frame
		int x = m10 / area;
		int y = m01 / area;

		Scalar color;
		if(isRight) {
			m_rightPaddlePos = y;
			// position the right object appears in the entire frame
			x = (frame.cols / 2) + x;
			color = BLUE;
		} else {
			m_leftPaddlePos = y;
			color = RED;
		}

		// draw crosshairs through the point being tracked in the left frame
		circle(frame, Point(x, y), 10, color, 2);
		line(frame, Point(x, y), Point(x, y - 15), color, 2);
		line(frame, Point(x, y), Point(x, y + 15), color, 2);
		line(frame, Point(x, y), Point(x - 15, y), color, 2);
		line(frame, Point(x, y), Point(x + 15, y), color, 2);
	}
}


/*

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



// draw crosshairs through the point being tracked in the right frame
circle(frame, Point(x, y), 10, Scalar(255, 0, 0), 2);
line(frame, Point(x, y), Point(x, y - 15), Scalar(255, 0, 0), 2);
line(frame, Point(x, y), Point(x, y + 15), Scalar(255, 0, 0), 2);
line(frame, Point(x, y), Point(x - 15, y), Scalar(255, 0, 0), 2);
line(frame, Point(x, y), Point(x + 15, y), Scalar(255, 0, 0), 2);
}

*/