#include "ColorPaddleDetector.h"


ColorPaddleDetector::ColorPaddleDetector(VideoCapture *vid)
{
	this->vid = vid;
}


ColorPaddleDetector::~ColorPaddleDetector()
{
}

void ColorPaddleDetector::configure()
{
	namedWindow("Configure", CV_WINDOW_AUTOSIZE); //create a window called "Control"
	//Create trackbars in "Control" window
	cvCreateTrackbar("Low Hue", "Configure", &lowHue, 179); //Hue (0 - 179)
	cvCreateTrackbar("High Hue", "Configure", &highHue, 179);

	cvCreateTrackbar("Low Saturation", "Configure", &lowSaturation, 255); //Saturation (0 - 255)
	cvCreateTrackbar("High Saturation", "Configure", &highSaturation, 255);

	cvCreateTrackbar("Low Value", "Configure", &lowValue, 255); //Value (0 - 255)
	cvCreateTrackbar("High Value", "Configure", &highValue, 255);
	namedWindow("Output", CV_WINDOW_AUTOSIZE);
	Mat frame, thresholded;

	while (true)
	{
		*vid >> frame;
		flip(frame, frame, 1);
		thresholdImage(frame, thresholded);
		
		imshow("Output", thresholded);
		int key = waitKey(30);
		if (key != -1) break;
	}

	cvDestroyAllWindows();
}

void ColorPaddleDetector::thresholdImage(Mat &f, Mat &destination)
{
	Mat HSV, frame = f.clone();

	// Create a HSV version of the grame
	cvtColor(frame, HSV, COLOR_BGR2HSV);

	// Threshold image
	inRange(HSV, Scalar(lowHue, lowSaturation, lowValue), Scalar(highHue, highSaturation, highValue), destination); //Threshold the image

	// TODO: look into this more V

	//morphological opening (remove small objects from the foreground)
	erode(destination, destination, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	dilate(destination, destination, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	//morphological closing (fill small holes in the foreground)
	dilate(destination, destination, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	erode(destination, destination, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
}

PaddleDetector::PaddlePositions ColorPaddleDetector::processFrame(Mat &frame)
{
	
	Mat thresholded;

	thresholdImage(frame, thresholded);

	// Calculate moments
	Moments oMoments = moments(thresholded);

	double dM01 = oMoments.m01;
	double dM10 = oMoments.m10;
	double dArea = oMoments.m00;

	if (dArea > 10000)
	{
		//calculate the position of the ball
		int posX = dM10 / dArea;
		int posY = dM01 / dArea;

		if (posX > frame.cols / 2)
			rightPaddlePos = posY;
		else
			leftPaddlePos = posY;
	}

	return PaddlePositions(leftPaddlePos, rightPaddlePos);

}

int ColorPaddleDetector::getLeftPaddleLoc()
{
	return leftPaddlePos;
}
int ColorPaddleDetector::getRightPaddleLoc()
{
	return rightPaddlePos;
}