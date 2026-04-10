#include <iostream>
#include "Board.h"
#include <cstdlib> // rand() 사용
#include <ctime>
#include <string>

Board::Board() {
	init();
}

void Board::init() {
	score = 0;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++) grid[i][j] = 0;

	spawn();	// 초기 숫자 2개 생성
	spawn();
}

// 숫자 생성: 빈 칸 중 한 곳에 2(90%) 또는 4(10%)를 넣습니다.
void Board::spawn() {
	// 빈 칸을 찾아서 랜덤하게 2 또는 4를 넣는 로직 작성
	int emptyCells[16][2];	// 빈 칸들의 좌표(행, 열)를 저장할 임시 명단
	int count = 0;			// 발견된 빈 칸의 개수

	// 1. 보드를 전체 다 훑으면서 빈 칸(0)이 어디인지 찾기
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (grid[i][j] == 0) {
				emptyCells[count][0] = i; // 명단의 count번째 줄에 '행' 위치 기록
				emptyCells[count][1] = j; // 명단의 count번째 줄에 '열' 위치 기록
				count++;				  // 빈 칸을 하나 찾았으니 명단 번호 증가
			}
		}
	}

	// 2. 빈 칸이 하나라도 있다면, 그중 하나를 무작위로 골라 숫자 채우기
	if (count > 0) {	
		int r = rand() % count;			// 0부터 (빈칸개수-1) 사이의 숫자 하나를 랜덤 선택
		int row = emptyCells[r][0];		// 선택된 명단 번호에서 '행' 좌표를 가져옴
		int col = emptyCells[r][1];		// 선택된 명단 번호에서 '열' 좌표를 가져옴
		
		// 10% 확률로 4, 90% 확률로 2를 생성 (삼항 연산자)
		grid[row][col] = (rand() % 10 == 0) ? 4 : 2;

	}
}

// 게임 오버 판전
bool Board::isGameOver() {
	// 1. 빈칸이 있는지 확인
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (grid[i][j] == 0)
				return false;
		}
	}

	// 2. 가로로 인접한 칸 중 합칠 수 있는지 확인
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			if (grid[i][j] == grid[i][j + 1]) return false;
		}
	}

	// 3. 새로로 인접한 칸 중 합칠 수 있는지 확인
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			if (grid[i][j] == grid[i + 1][j]) return false;
		}
	}

	// 여기까지 왔으면 움직일 방법이 없음
	return true;

}


// 이동 로직: 왼쪽(Left) 예시
bool Board::move(int dir) {
	bool moved = false;

	// 위로 이동 (dir == 0 라고 가정할 떄 로직)
	if (dir == 0) { // 위쪽(up)으로 밀기
		for (int i = 0; i < 4; i++) { // 각 행별로 검사
			int target = 0; // 숫자가 도착하거나 합쳐질 수 있는 '기준점'

			for (int j = 1; j < 4; j++) { // 2번째 행(index 1)부터 아래로 내려가며 검사
				if (grid[j][i] == 0) continue; // 위쪽으로 옮길 숫자가 없으면 패스

				int current = j; // 현재 움직일 숫자의 위치

				// 1. 앞에 빈 공간이 있다면 쭉 밀기
				while (current > target && grid[current - 1][i] == 0) {
					grid[current - 1][i] = grid[current][i]; // 위칸으로 복사
					grid[current][i] = 0;					 // 현재 칸은 비움
					current--;								 // 위치를 한 칸 앞으로 수정
					moved = true;							 // 움직임 발생 기록
				}

				// 2. 밀고 난 후, 앞 칸의 숫자와 내 숫자가 같다면 합치기
				if (current > target && grid[current - 1][i] == grid[current][i]) {
					grid[current - 1][i] *= 2;				 // 숫자를 2배로 (합치기)
					score += grid[current - 1][i];			 // 점수 추가
					grid[current][i] = 0;					 // 합쳐졌으니 내 자리는 비움
					target = current;						 // 중요: 방금 합쳐진 칸은 이번 턴에 또 합칠 수 없음
					moved = true;							 // 움직임 발생 기록
				}
				else {
					// 합쳐지지 않았다면, 이 숫자가 다음 숫자의 벽(target)이 됨
					target = current;
				}

			}
		}
	}

	// 2.아래로 이동 (dir == 1 라고 가정할 떄 로직)
	else if (dir == 1) { // 아래쪽(down)으로 밀기
		for (int i = 0; i < 4; i++) { // 각 행별로 검사
			int target = 3; // 숫자가 도착하거나 합쳐질 수 있는 '기준점'

			for (int j = 2; j >= 0; j--) { // 2번째 행(index 1)부터 아래로 내려가며 검사
				if (grid[j][i] == 0) continue; // 위쪽으로 옮길 숫자가 없으면 패스

				int current = j; // 현재 움직일 숫자의 위치

				// 1. 앞에 빈 공간이 있다면 쭉 밀기
				while (current < target && grid[current + 1][i] == 0) {
					grid[current + 1][i] = grid[current][i]; // 위칸으로 복사
					grid[current][i] = 0;					 // 현재 칸은 비움
					current++;								 // 위치를 한 칸 앞으로 수정
					moved = true;							 // 움직임 발생 기록
				}

				// 2. 밀고 난 후, 앞 칸의 숫자와 내 숫자가 같다면 합치기
				if (current < target && grid[current + 1][i] == grid[current][i]) {
					grid[current + 1][i] *= 2;				 // 숫자를 2배로 (합치기)
					score += grid[current + 1][i];			 // 점수 추가
					grid[current][i] = 0;					 // 합쳐졌으니 내 자리는 비움
					target = current;						 // 중요: 방금 합쳐진 칸은 이번 턴에 또 합칠 수 없음
					moved = true;							 // 움직임 발생 기록
				}
				else {
					// 합쳐지지 않았다면, 이 숫자가 다음 숫자의 벽(target)이 됨
					target = current;
				}

			}
		}
	}

	// 3. 왼쪽(left)으로 밀기
	else if (dir == 2) { 
		for (int i = 0; i < 4; i++) { // 각 행별로 검사
			int target = 0; // 숫자가 도착하거나 합쳐질 수 있는 '기준점'

			for (int j = 1; j < 4; j++) { // 2번째 행(index 1)부터 아래로 내려가며 검사
				if (grid[i][j] == 0) continue; // 위쪽으로 옮길 숫자가 없으면 패스

				int current = j; // 현재 움직일 숫자의 위치

				// 1. 앞에 빈 공간이 있다면 쭉 밀기
				while (current > target && grid[i][current - 1] == 0) {
					grid[i][current - 1] = grid[i][current]; // 위칸으로 복사
					grid[i][current] = 0;					 // 현재 칸은 비움
					current--;								 // 위치를 한 칸 앞으로 수정
					moved = true;							 // 움직임 발생 기록
				}

				// 2. 밀고 난 후, 앞 칸의 숫자와 내 숫자가 같다면 합치기
				if (current > target && grid[i][current - 1] == grid[i][current]) {
					grid[i][current - 1] *= 2;				 // 숫자를 2배로 (합치기)
					score += grid[i][current - 1];			 // 점수 추가
					grid[i][current] = 0;					 // 합쳐졌으니 내 자리는 비움
					target = current;						 // 중요: 방금 합쳐진 칸은 이번 턴에 또 합칠 수 없음
					moved = true;							 // 움직임 발생 기록
				}
				else {
					// 합쳐지지 않았다면, 이 숫자가 다음 숫자의 벽(target)이 됨
					target = current;
				}

			}
		}
	}

	// 4. 오른쪽(right)으로 밀기
	else if (dir == 3) { 
		for (int i = 0; i < 4; i++) { // 각 행별로 검사
			int target = 3; // 숫자가 도착하거나 합쳐질 수 있는 '기준점'

			for (int j = 2; j >= 0; j--) { // 2번째 행(index 1)부터 아래로 내려가며 검사
				if (grid[i][j] == 0) continue; // 위쪽으로 옮길 숫자가 없으면 패스

				int current = j; // 현재 움직일 숫자의 위치

				// 1. 앞에 빈 공간이 있다면 쭉 밀기
				while (current < target && grid[i][current + 1] == 0) {
					grid[i][current + 1] = grid[i][current]; // 위칸으로 복사
					grid[i][current] = 0;					 // 현재 칸은 비움
					current++;								 // 위치를 한 칸 앞으로 수정
					moved = true;							 // 움직임 발생 기록
				}

				// 2. 밀고 난 후, 앞 칸의 숫자와 내 숫자가 같다면 합치기
				if (current > target && grid[i][current + 1] == grid[i][current]) {
					grid[i][current + 1] *= 2;				 // 숫자를 2배로 (합치기)
					score += grid[i][current + 1];			 // 점수 추가
					grid[i][current] = 0;					 // 합쳐졌으니 내 자리는 비움
					target = current;						 // 중요: 방금 합쳐진 칸은 이번 턴에 또 합칠 수 없음
					moved = true;							 // 움직임 발생 기록
				}
				else {
					// 합쳐지지 않았다면, 이 숫자가 다음 숫자의 벽(target)이 됨
					target = current;
				}

			}
		}
	}

	if (moved) spawn(); // 보드에 변화가 한 번이라도 있었다면 새 숫자 하나 생성
	return moved;
}

void Board::saveToDB() {
	// 1. 비밀번호 설정 (DBeaver와 동일하게)
	std::string db_pw = "my123456";

	// 2. 쿼리문 생성 (공백과 세미콜론 위치를 아주 정밀하게 맞췄습니다)
	// "INSERT INTO ..." 문장 끝에 ; 가 들어있는지 꼭 확인하세요.
	std::string sql_query = "INSERT INTO game_results (score) VALUES (" + std::to_string(score) + ");";

	// 3. 전체 실행 명령어 조합
	std::string full_command = "mysql -u root -p" + db_pw + " game_db -e \"" + sql_query + "\"";

	std::cout << "\n[시스템] DB에 점수(" << score << ") 저장 시도 중..." << std::endl;

	// 4. 실행 및 결과 확인
	int result = system(full_command.c_str());

	if (result == 0) {
		std::cout << "[성공] 데이터가 MySQL에 저장되었습니다!" << std::endl;
	}
	else {
		std::cout << "[오류] 실행 실패 (코드: " << result << ")" << std::endl;
		std::cout << "[팁] CMD에서 직접 다음을 실행해보세요: " << full_command << std::endl;
	}
}