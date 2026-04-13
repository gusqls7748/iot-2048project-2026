#define _CRT_SECURE_NO_WARNINGS // 1순위로 작성
#include "raylib.h"
#include <vector>
#include <string>
#include <iostream>
#include <conio.h> // Windows에서 키 입력을 받기 위해 사용 (getch)
#include "Board.h"

using namespace std;
char input;

// 폰트 변수 선언
Font nanumFont;

// 2048 스타일 색상 정의
Color bg_color = { 251, 248, 239, 255 };      // 전체 배경 (연한 베이지)
Color board_color = { 187, 173, 160, 255 };   // 보드 판 배경 (갈색)
Color tile_empty = { 205, 193, 180, 255 };    // 빈 타일 색상

Color GetTileColor(int value) {
    switch (value) {
    case 2:    return { 238, 228, 218, 255 };
    case 4:    return { 237, 224, 200, 255 };
    case 8:    return { 242, 177, 121, 255 };
    case 16:   return { 245, 149, 99, 255 };
    case 32:   return { 246, 124, 95, 255 };
    case 64:   return { 246, 94, 59, 255 };
    case 128:  return { 237, 207, 114, 255 };
    case 256:  return { 237, 204, 97, 255 };
    case 512:  return { 237, 200, 80, 255 };
    case 1024: return { 237, 197, 63, 255 };
    case 2048: return { 237, 194, 46, 255 };
    default:   return tile_empty;
    }
}

int main() {
    // 1. 창 초기화
    InitWindow(500, 700, "My 2048 Project");
    SetTargetFPS(60);

    // 2. 한글 폰트 로드
    int codepointsCount = 0;
    // 1. 게임에서 사용할 모든 글자를 한 번에 등록 (영문, 숫자, 한글, 특수문자 포함)
    const char* textToLoad = "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "위:점다시하기종료조작법방향키로타일을밀어서합쳐보세요게임최종점수실시간랭킹! |";
    int* codepoints = LoadCodepoints(textToLoad, &codepointsCount);
    // 2. 폰트 로드 (폰트 크기를 넉넉히 40~50 정도로 잡으면 더 선명합니다)
    nanumFont = LoadFontEx("C:/Windows/Fonts/NANUMGOTHICCODING.TTF", 50, codepoints, codepointsCount);
    // 3. 반드시 메모리 해제
    UnloadCodepoints(codepoints);

    Board game;
    bool isSaved = false;
    int bestScore = game.getBestScoreFromDB();

    // 3. [메인 루프] 게임 실행 중 무한 반복
    while (!WindowShouldClose()) {

        // --- [입력 처리] ---
        // Q 키는 게임 상태와 상관없이 언제든 눌리면 저장 후 종료되어야 함
        if (IsKeyPressed(KEY_Q)) {
            game.saveToDB();
            break; // 루프 즉시 탈출
        }

        // 게임 중일 때만 방향키 이동 허용
        if (!game.isGameOver()) {
            if (IsKeyPressed(KEY_UP))    game.move(0);
            if (IsKeyPressed(KEY_DOWN))  game.move(1);
            if (IsKeyPressed(KEY_LEFT))  game.move(2);
            if (IsKeyPressed(KEY_RIGHT)) game.move(3);
        }
        else {
            // 게임 오버 상태에서 R 키를 누르면 재시작
            if (IsKeyPressed(KEY_R)) {
                game.reset();
                bestScore = game.getBestScoreFromDB();
                isSaved = false;
                // static 변수는 직접 지울 수 없으므로 로직상 초기화 확인
            }
        }

        // 실시간 Best 점수 갱신 (현재 점수가 더 높을 때)
        if (game.getScore() > bestScore) bestScore = game.getScore();

        // --- [그리기 시작] ---
        BeginDrawing();
        // [UI] 타일 숫자에 따른 배경색 반환 함수
        ClearBackground(bg_color); // 겹침 방지를 위해 매 프레임 초기화

        // 상단 UI: 제목, 현재 점수, 최고 점수 표시
        DrawText("2048", 30, 40, 80, { 119, 110, 101, 255 });
        DrawRectangleRounded({ 240, 40, 110, 60 }, 0.2f, 10, board_color);
        DrawText("SCORE", 270, 45, 12, { 238, 228, 218, 255 });
        DrawText(TextFormat("%d", game.getScore()), 260, 65, 20, WHITE);

        DrawRectangleRounded({ 360, 40, 110, 60 }, 0.2f, 10, board_color);
        DrawText("BEST", 395, 45, 12, { 238, 228, 218, 255 });
        DrawText(TextFormat("%d", bestScore), 380, 65, 20, WHITE);

        // 2. 화면 분기 (진행 vs 종료)
        if (!game.isGameOver()) {
            // [진행 화면] 4x4 보드와 타일 렌더링
            DrawTextEx(nanumFont, "R: 다시하기  |  Q: 종료", { 30, 135 }, 20, 2, { 119, 110, 101, 255 });
            DrawRectangleRounded({ 20, 180, 460, 460 }, 0.05f, 10, board_color);

            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    int val = game.getCell(i, j);
                    DrawRectangleRounded({ 35 + j * 110.0f, 195 + i * 110.0f, 95, 95 }, 0.15f, 10, GetTileColor(val));
                    if (val > 0) {
                        int fontSize = (val < 100) ? 40 : (val < 1000) ? 30 : 20;
                        int textX = 35 + j * 110.0f + (val < 10 ? 35 : val < 100 ? 25 : 15);
                        int textY = 195 + i * 110.0f + 30;
                        DrawText(TextFormat("%d", val), textX, textY, fontSize, (val <= 4) ? DARKGRAY : WHITE);
                    }
                }
            }
            DrawTextEx(nanumFont, "조작법: 방향키로 타일을 밀어서 합쳐보세요!", { 30, 655 }, 18, 2, DARKGRAY);
        }
        else {
            // [종료 화면] 결과 출력 및 실시간 랭킹 표시
            DrawRectangle(0, 130, 500, 570, bg_color); // 보드판 영역 덮어씌우기

            DrawTextEx(nanumFont, "GAME OVER!", { 135, 180 }, 50, 2, { 119, 110, 101, 255 });
            DrawTextEx(nanumFont, TextFormat("최종 점수: %d", game.getScore()), { 175, 240 }, 25, 2, DARKGRAY);
            DrawTextEx(nanumFont, "[ 실시간 랭킹 TOP 5 ]", { 145, 310 }, 22, 2, { 119, 110, 101, 255 });

            static std::vector<int> topScores;
            if (!isSaved) { // 게임 종료 직후 1회만 DB 저장 및 랭킹 로드
                game.saveToDB();
                topScores = game.getTopScoresFromDB();
                isSaved = true;
            }

            for (int i = 0; i < (int)topScores.size(); i++) {
                Color rankColor = (i == 0) ? GOLD : (i == 1) ? GRAY : (i == 2) ? BROWN : DARKGRAY;
                char rankText[50];
                sprintf(rankText, "%d위: %d 점", i + 1, topScores[i]);
                DrawTextEx(nanumFont, rankText, { 180, (float)(360 + i * 45) }, 25, 2, rankColor);
            }

            DrawTextEx(nanumFont, "REPLAY: PRESS 'R'!", { 130, 620 }, 30, 2, { 246, 94, 59, 255 });
        }

        EndDrawing();
    } // while 루프 끝

    // 자원 해제
    UnloadFont(nanumFont);
    CloseWindow();
    return 0;
}

//#include <iostream>
//#include <conio.h> // Windows에서 키 입력을 받기 위해 사용 (getch)
//#include "Board.h"
//#include "raylib.h"
//
//using namespace std;
//
//void display(const Board& game) {
//	system("cls"); // 화면 지우기
//	std::cout << "2048 Game - Score: " << game.getScore() << std::endl;
//	for (int i = 0; i < 4; i++) {
//		for (int j = 0; j < 4; j++) {
//			if (game.getCell(i, j) == 0) std::cout << "[  ] ";
//			else printf("[%4d]", game.getCell(i, j));
//		}
//		std::cout << std::endl;
//	}
//	std::cout << "방향키(w,a,s,d)를 누르세요. (종료: q)" << std::endl;
//}
//
//int main() {
//	Board game; // 게임 객체 생성
//	char input;
//	
//
//	while (true) {
//		display(game);	  // 1. 현재 상태 출력
//		input = _getch(); // 2. 사용자 입력 대기 (엔터 없이 바로 입력됨)
//
//		if (input == 'q' || input == 'Q') {
//			// 그냥 나가지말고, 나가기전에 현재 점수를 DB에 저장!
//			cout << "\n 게임을 중단합니다. 현재까지의 점수를 저장합니다..." << endl;
//			game.saveToDB();
//			break; // 종료 조건
//		}
//		// a: 왼쪽(2), d: 오른쪽(3), w: 위(0), s: 아래(1) 등으로 약속
//		int dir = -1; // 입력된 방향을 저장할 변수.
//		if (input == 'w' || input == 'W') dir = 0; // 위쪽
//		else if (input == 's' || input == 'S')dir = 1; // 아래
//		else if (input == 'a' || input == 'A')dir = 2; // 왼쪽
//		else if (input == 'd' || input == 'D')dir = 3; // 오른쪽
//		
//		// 방향 키를 제대로 놓쳤다면
//		if (dir != -1) {
//			// 일단 움직여보고, 움직임 결과(bool)발표
//			bool moved = game.move(dir);
//
//			// 만약 움직이지 못했다면 (꽉 찼거나 벽에 막혔을 떄)
//			if (!moved){
//				// 진짜로 게임 오버인지 확인
//				if(game.isGameOver()){
//				display(game); // 마지막 상태 한 번 더 보여주기
//				cout << "\n============================" << endl;
//				cout << "       GAME OVER...!        " << endl;
//				cout << "============================" << endl;
//
//				// 여기서 MySQL 저장 함수를 호출하면 딱 좋겠네요!
//				game.saveToDB(); 
//				break;
//				}
//			}
//		}
//	}
//
//		cout << " 게임이 종료되었습니다. 최종 점수: " << game.getScore() << endl;
//	return 0;
//}
