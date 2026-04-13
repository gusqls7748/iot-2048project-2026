#ifndef BOARD_H
#define BOARD_H

#include <vector>

class Board {
private:
    int grid[4][4];    // 4x4 게임판 데이터
    int score;         // 현재 점수

public:
    Board();           // 객체 생성 시 초기화
    void init();       // 게임 시작 (숫자 2개 생성)
    void reset();      // 게임 재시작 (판 비우기)

    // 핵심 로직
    void spawn();      // 빈 칸에 2 또는 4 랜덤 생성
    bool move(int dir);// 타일 이동 및 합치기 (성공 시 true)
    bool isGameOver(); // 더 이상 움직일 수 없는지 확인

    // 데이터베이스 연동 (MySQL)
    void saveToDB();                       // 점수 저장
    int getBestScoreFromDB();              // 역대 최고 점수 가져오기
    std::vector<int> getTopScoresFromDB(); // 랭킹 TOP 5 가져오기

    // UI 출력용 데이터 접근 (Getter)
    int getScore() const { return score; } 
    int getCell(int r, int c) const { return grid[r][c]; }
};

#endif