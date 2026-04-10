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

#include "raylib.h"
#include <vector>
#include <string>
#include <iostream>
#include <conio.h> // Windows에서 키 입력을 받기 위해 사용 (getch)
#include "Board.h"

using namespace std;
char input;

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
    // 1. 창 초기화 (이미지 비율에 맞게 500x700 정도로 설정)
    InitWindow(500, 700, "My 2048 Project");
    SetTargetFPS(60);

    Board game; // 게임 객체 생성
    bool isSaved = false; // DB 저장 중복 방지용 변수 추가!

    // 콘솔창에 안내 문구 출력
    cout << "========================================" << endl;
    cout << " 게임이 시작되었습니다!" << endl;
    cout << " - 화살표 키: 타일 이동" << endl;
    cout << " - Q 키: 게임 종료 및 현재 점수 저장" << endl;
    cout << "========================================" << endl;

    while (!WindowShouldClose()) {
        // 1. 입력 처리 (Raylib 방식)
        if (!game.isGameOver()) {
            if (IsKeyPressed(KEY_UP))    game.move(0);
            if (IsKeyPressed(KEY_DOWN))  game.move(1);
            if (IsKeyPressed(KEY_LEFT))  game.move(2);
            if (IsKeyPressed(KEY_RIGHT)) game.move(3);
        }

        // Q 키를 눌렀을 때 종료 로직
        if (IsKeyPressed(KEY_Q)) {
            cout << "\n[시스템] Q를 눌러 게임을 중단합니다. 점수를 저장합니다..." << endl;
            game.saveToDB();
            break; // 루프 탈출
        }
        
        BeginDrawing();
        ClearBackground(bg_color); // 전체 배경색 칠하기

        // 상단 텍스트 및 점수 업데이트
        DrawText("2048", 30, 40, 80, { 119, 110, 101, 255 });
        DrawRectangleRounded({ 320, 40, 150, 60 }, 0.2f, 10, board_color);
        DrawText("SCORE", 360, 45, 15, { 238, 228, 218, 255 });
        DrawText(TextFormat("%d", game.getScore()), 380, 65, 25, WHITE); // 실시간 점수

        // 보드 판 그리기
        DrawRectangleRounded({ 20, 180, 460, 460 }, 0.05f, 10, board_color);

        // 4. 타일 16개 그려보기 (일단 빈 칸으로)
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                int val = game.getCell(i, j); // 로직에서 값 가져오기
                DrawRectangleRounded({ 35 + j * 110.0f, 195 + i * 110.0f, 95, 95 }, 0.15f, 10, GetTileColor(val));

                if (val > 0) {
                    int fontSize = (val < 100) ? 40 : (val < 1000) ? 30 : 20;
                    // 숫자 중앙 정렬을 위한 오프셋 (수치 조정 가능)
                    int textX = 35 + j * 110.0f + (val < 10 ? 35 : val < 100 ? 25 : 15);
                    int textY = 195 + i * 110.0f + 30;
                    DrawText(TextFormat("%d", val), textX, textY, fontSize, (val <= 4) ? DARKGRAY : WHITE);
                }
            }
        }
        
        // 2. 게임 오버 처리 (중복 저장 방지 로직 포함)
        if (game.isGameOver()) {
            DrawRectangle(0, 0, 500, 700, { 255, 255, 255, 180 }); // 화면을 흐릿하게 가림
            DrawText("GAME OVER!", 100, 320, 50, { 119, 110, 101, 255 });
            DrawText(TextFormat("Final Score: %d", game.getScore()), 150, 380, 25, DARKGRAY);
           // DrawText("Press 'R' to Restart", 160, 450, 20, GRAY);

            // 딱 한 번만 DB에 저장!
            if (!isSaved) {
                game.saveToDB();
                isSaved = true; // 저장 완료 표시
            }

            // R키 누르면 다시 시작 (선택 사항)
            if (IsKeyPressed(KEY_R)) {
                // game.reset(); // 만약 Reset 함수가 있다면 호출
                // isSaved = false;
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}