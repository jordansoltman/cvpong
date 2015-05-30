#ifndef GAMEBAORD_H
#define GAMEBOARD_H
#include <time.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;

const int DEFAULT_X = 640;
const int DEFAULT_Y = 480;
const int BOARDER_WIDTH = 3;
const int BALL_SIZE = 33;
const int PADDLE_X = 18;
const int PADDLE_Y = 120;
const int PADDLE_MOD = PADDLE_Y / 3;
const int WINNING_SCORE = 13;
const double SPEED_INCREMENT = 1.5;

const int MOVE_LEFT = -11;
const int MOVE_RIGHT = 11;
const int MOVE_UP = -11;
const int MOVE_DOWN = 11;
const int MOVE_HORIZ = 0;

const int BOARDER_COLOR[3] = {0, 153, 0};
const int BALL_COLOR[3] = {0, 204, 0};
const int L_PADDLE_COLOR[3] = {0 , 0, 255}; /* red paddle */
const int R_PADDLE_COLOR[3] = {255, 0, 0}; /* blue paddle */


class GameBoard {

public:
	GameBoard();
	bool gameOn();
	void play(const Mat& background, int leftPaddlePos, int rightPaddleLoc);

private:
	void displayBall();
	void setBall();
	void checkCollisions();
	void setRightPaddle(int y);
	void setLeftPaddle(int y);
	void setScore();
	void initPaddles();

	struct GameBall {
		GameBall();
		void move(double x, double y);
		int m_Xpos;
		int m_Ypos;
		int m_Xmov;
		int m_Ymov;
	};

	struct Paddle {
		int m_Xpos;
		int m_Ypos;
	};

	GameBall m_ball;
	Paddle m_leftPaddle;
	Paddle m_rightPaddle;
	bool m_gameOn;
	int m_score[2];
	double m_speedFactor;
	Mat m_board;
};
#endif