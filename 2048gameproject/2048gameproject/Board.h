#ifndef BOARD_H
#define BOARD_H

#include <vector>

class Board {
private:
	int grid[4][4];	// 배열 
	int score;		// 변수


public:
	Board();
	void init();		// 게임초기화 (2개 숫자 생성
	void spawn();		// 빈공간에 랜덤하게 2또는 4 생성
	void saveToDB();
	bool isGameOver();  // 게임 종료 여부 확인
	bool move(int dir);	// 0:상, 1:하, ::2:좌 3:우
	
	//bool canMove();		// 게임 오버 판전
	int getScore() const { return score;  }
	int getCell(int r, int c) const { return grid[r][c]; }
};

#endif
