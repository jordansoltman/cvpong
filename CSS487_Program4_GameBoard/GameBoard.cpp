#ifndef GAMEBAORD_CPP
#define GAMEBOARD_CPP
#include "GameBoard.h"

GameBoard::GameBall::GameBall() {
	m_Xpos = static_cast<int>((DEFAULT_X / 2) - (BALL_SIZE / 2));
	m_Ypos = static_cast<int>((DEFAULT_Y / 2) - (BALL_SIZE / 2));
	m_Xmov = MOVE_RIGHT;
	m_Ymov = MOVE_HORIZ;
}

void GameBoard::GameBall::move(double x, double y) {
	m_Xmov = static_cast<int>(x);
	m_Ymov = static_cast<int>(y);
}

GameBoard::GameBoard() {
	m_gameOn = true;
	m_score[0] = 0;
	m_score[1] = 0;
	m_speedFactor = 0;
	m_board = cv::Mat::zeros(DEFAULT_Y, DEFAULT_X, CV_8UC3);
	initPaddles();
}

bool GameBoard::gameOn() {
	return(m_gameOn);
}

void GameBoard::play(const Mat& background, int leftPaddlePos, int rightPaddleLoc) {
	// random number for paddle location, used for testing
	//srand(static_cast<int>(time(NULL)));
	//int y1 = rand() % DEFAULT_Y;
	//srand((y1 + 9971232) % 17);
	//int y2 = rand() % DEFAULT_Y;
	
	m_board = background;
	setLeftPaddle(leftPaddlePos);
	setRightPaddle(rightPaddleLoc);
	setScore();
	setBall();
	namedWindow("Board");
	imshow("Board", m_board);
	waitKey(1);
}

void GameBoard::setBall() {	
	checkCollisions();
	m_ball.m_Ypos += m_ball.m_Ymov;
	m_ball.m_Xpos += m_ball.m_Xmov;

	for(int i = m_ball.m_Ypos; i < m_ball.m_Ypos + BALL_SIZE; i++) {
		for(int j = m_ball.m_Xpos; j < m_ball.m_Xpos + BALL_SIZE; j++) {
			m_board.at<Vec3b>(i, j)[0] = BALL_COLOR[0];
			m_board.at<Vec3b>(i, j)[1] = BALL_COLOR[1];
			m_board.at<Vec3b>(i, j)[2] = BALL_COLOR[2];
		}
	}
}

void GameBoard::checkCollisions() {
	// check for y-boundary collision
	if(m_ball.m_Ypos + m_ball.m_Ymov + BALL_SIZE >= m_board.rows - 1 ||
	   m_ball.m_Ypos + m_ball.m_Ymov <= 0) {
		m_ball.move(m_ball.m_Xmov, -m_ball.m_Ymov);
	}

	// check for x-boundary collision right-side
	// left side scores
	if(m_ball.m_Xpos + m_ball.m_Xmov + BALL_SIZE >= m_board.cols - 1) {
		m_ball.m_Xpos = static_cast<int>((DEFAULT_X / 2) - (BALL_SIZE / 2));
		m_ball.m_Ypos = static_cast<int>((DEFAULT_Y / 2) - (BALL_SIZE / 2));
		m_ball.move(MOVE_LEFT - m_speedFactor, MOVE_HORIZ);
		m_score[0]++;
		m_speedFactor += SPEED_INCREMENT;
	}

	// check for x-boundary collision left-side
	// right side scores
	if(m_ball.m_Xpos + m_ball.m_Xmov <= 0) {
		m_ball.m_Xpos = static_cast<int>((DEFAULT_X / 2) - (BALL_SIZE / 2));
		m_ball.m_Ypos = static_cast<int>((DEFAULT_Y / 2) - (BALL_SIZE / 2));
		m_ball.move(MOVE_RIGHT + m_speedFactor, MOVE_HORIZ);
		m_score[1]++;
		m_speedFactor += SPEED_INCREMENT;
	}

	// check for right paddle collision
	if(m_ball.m_Xpos + m_ball.m_Xmov + BALL_SIZE >= m_rightPaddle.m_Xpos) {
		// check for bottom, middle, top
		if(m_ball.m_Ypos + m_ball.m_Ymov + BALL_SIZE >= m_rightPaddle.m_Ypos + PADDLE_MOD * 2 &&
		   m_ball.m_Ypos + m_ball.m_Ymov <= m_rightPaddle.m_Ypos + PADDLE_Y) {
			m_ball.move(MOVE_LEFT - m_speedFactor, MOVE_DOWN + m_speedFactor);
		} else if(m_ball.m_Ypos + m_ball.m_Ymov + BALL_SIZE >= m_rightPaddle.m_Ypos + PADDLE_MOD &&
				  m_ball.m_Ypos + m_ball.m_Ymov <= m_rightPaddle.m_Ypos + PADDLE_Y - PADDLE_MOD) {
			m_ball.move(MOVE_LEFT - m_speedFactor, MOVE_HORIZ);
		} else if(m_ball.m_Ypos + m_ball.m_Ymov + BALL_SIZE >= m_rightPaddle.m_Ypos &&
				  m_ball.m_Ypos + m_ball.m_Ymov <= m_rightPaddle.m_Ypos + PADDLE_Y - PADDLE_MOD * 2) {
			m_ball.move(MOVE_LEFT - m_speedFactor, MOVE_UP - m_speedFactor);
		}
	}

	// check for left paddle collision
	if(m_ball.m_Xpos + m_ball.m_Xmov <= m_leftPaddle.m_Xpos + PADDLE_X) {
		// check for bottom, middle, top
		if(m_ball.m_Ypos + m_ball.m_Ymov + BALL_SIZE >= m_leftPaddle.m_Ypos + PADDLE_MOD * 2 &&
		   m_ball.m_Ypos + m_ball.m_Ymov <= m_leftPaddle.m_Ypos + PADDLE_Y) {
			m_ball.move(MOVE_RIGHT + m_speedFactor, MOVE_DOWN + m_speedFactor);
		} else if(m_ball.m_Ypos + m_ball.m_Ymov + BALL_SIZE >= m_leftPaddle.m_Ypos + PADDLE_MOD &&
		   m_ball.m_Ypos + m_ball.m_Ymov <= m_leftPaddle.m_Ypos + PADDLE_Y - PADDLE_MOD) {
			m_ball.move(MOVE_RIGHT + m_speedFactor, MOVE_HORIZ);
		} else if(m_ball.m_Ypos + m_ball.m_Ymov + BALL_SIZE >= m_leftPaddle.m_Ypos &&
		   m_ball.m_Ypos + m_ball.m_Ymov <= m_leftPaddle.m_Ypos + PADDLE_Y - PADDLE_MOD * 2) {
			m_ball.move(MOVE_RIGHT + m_speedFactor, MOVE_UP - m_speedFactor);
		}
	}
}

void GameBoard::setLeftPaddle(int y) {
	// check if new location is in bounds
	if(y <= 1) {
		m_leftPaddle.m_Ypos = 1;
	} else if(y + PADDLE_Y >= m_board.rows - 1) {
		m_leftPaddle.m_Ypos = m_board.rows - PADDLE_Y - 1;
	} else {
		m_leftPaddle.m_Ypos = y;
	}

	// set paddle at new location
	for(int i = m_leftPaddle.m_Ypos; i <m_leftPaddle.m_Ypos + PADDLE_Y; i++) {
		for(int j = m_leftPaddle.m_Xpos; j < m_leftPaddle.m_Xpos + PADDLE_X; j++) {
			m_board.at<Vec3b>(i, j)[0] = L_PADDLE_COLOR[0];
			m_board.at<Vec3b>(i, j)[1] = L_PADDLE_COLOR[1];
			m_board.at<Vec3b>(i, j)[2] = L_PADDLE_COLOR[2];
		}
	}
}

void GameBoard::setRightPaddle(int y) {
	// check if new location is in bounds
	if(y <= 1) {
		m_rightPaddle.m_Ypos = 1;
	} else if(y + PADDLE_Y >= m_board.rows - 1) {
		m_rightPaddle.m_Ypos = m_board.rows - PADDLE_Y - 1;
	} else {
		m_rightPaddle.m_Ypos = y;
	}

	// set paddle at new location
	for(int i = m_rightPaddle.m_Ypos; i <m_rightPaddle.m_Ypos + PADDLE_Y; i++) {
		for(int j = m_rightPaddle.m_Xpos; j < m_rightPaddle.m_Xpos + PADDLE_X; j++) {
			m_board.at<Vec3b>(i, j)[0] = R_PADDLE_COLOR[0];
			m_board.at<Vec3b>(i, j)[1] = R_PADDLE_COLOR[1];
			m_board.at<Vec3b>(i, j)[2] = R_PADDLE_COLOR[2];
		}
	}
}

void GameBoard::initPaddles() {
	m_leftPaddle.m_Xpos = BOARDER_WIDTH * 2;
	m_leftPaddle.m_Ypos = static_cast<int>((DEFAULT_Y / 2) - (PADDLE_Y / 2));

	m_rightPaddle.m_Xpos = DEFAULT_X - PADDLE_X - (BOARDER_WIDTH * 2) - 1;
	m_rightPaddle.m_Ypos = static_cast<int>((DEFAULT_Y / 2) - (PADDLE_Y / 2));
}

void GameBoard::setScore() {
	std::string score = "";
	if(m_score[0] >= WINNING_SCORE) {
		score = "PLAYER 1 WINS!";
		putText(m_board, score, cvPoint((DEFAULT_X / 2) - 143, BOARDER_WIDTH * 15), FONT_HERSHEY_COMPLEX_SMALL, 1.5, cvScalar(255, 0, 255), 1, CV_AA);
		m_gameOn = false;
	} else if(m_score[1] >= WINNING_SCORE) {
		score = "PLAYER 2 WINS!";
		putText(m_board, score, cvPoint((DEFAULT_X / 2) - 143, BOARDER_WIDTH * 15), FONT_HERSHEY_COMPLEX_SMALL, 1.5, cvScalar(255, 0, 255), 1, CV_AA);
		m_gameOn = false;
	} else {
		score = to_string(m_score[0]);
		score += " | " + to_string(m_score[1]);
		putText(m_board, score, cvPoint((DEFAULT_X / 2) - 41, BOARDER_WIDTH * 15), FONT_HERSHEY_COMPLEX_SMALL, 1.5, cvScalar(255, 0, 255), 1, CV_AA);
	}
}

#endif