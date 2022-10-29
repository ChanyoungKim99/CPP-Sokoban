#include <iostream>

const char gStageData[]{"\
########\n\
# .. p #\n\
# oo   #\n\
#      #\n\
########"
};				
					// \를 이용해서 문자열을 위와 같이 보기쉽게 구별할 수 있다
	
const int gStageWidth{ 8 };
const int gStageHeight{ 5 };
					// 상수는 전역변수로 사용해도 문제없다.

enum Object
{
	OBJ_SPACE,
	OBJ_WALL,
	OBJ_GOAL,
	OBJ_BLOCK,
	OBJ_BLOCK_ON_GOAL,
	OBJ_MAN,
	OBJ_MAN_ON_GOAL,
						// enum이므로 "상수" 이다.

	OBJ_UNKNOWN			// 마무리에 필요한 구문
};

void Initialize(Object* state, int w, int h, const char* stageData);
void Draw(const Object* state, int w, int h);
void Update(Object* state, char input, int w, int h);
bool CheckClear(const Object* state, int w, int h);

// 변하지않는 데이터는 상수 const를 사용하는 것이 좋은 프로그래밍이다.
// 선언만 있는 함수들을 모두 정의까지 만드는 방법 =
// 함수들을 드래그, 우클릭해서 빠른 작업 및 리펙터링 - 선언/정의 만들기

int main()
{
	// state = 동적배열로 할당한 1차 배열 enum
	Object* state = new Object[gStageWidth * gStageHeight];

	Initialize(state, gStageWidth, gStageHeight, gStageData);

	while (true)
	{
		Draw(state, gStageWidth, gStageHeight);

		if (CheckClear(state, gStageWidth, gStageHeight))
		{
			break;
		}

		std::cout << "a:left d:right w:up s:down" << std::endl;
		std::cout << "command : ";
		char input;
		std::cin >> input;

		Update(state, input, gStageWidth, gStageHeight);
	}

	std::cout << "Congratulations!!! You Win!!" << std::endl;

	delete[] state;
	state = nullptr;
}

void Initialize(Object* state, int w, int h, const char* stageData)
{
	// gStageData를 enum(열거형) Object로
	const char* p = stageData;
	int x{}, y{};

	while (*p != '\0')
	{
		// *p -> stageData의 포인터 변수

		Object t;		// Object type의 변수t 선언

		// p의 역참조를 하면서 stageData의 값을 임시 Object t에 옮기면서 치환,
		// 그 치환된 값을 다시 1차배열의 state에 옮김

		switch (*p)
		{
			case '#':
				t = OBJ_WALL;
				break;
			case ' ':
				t = OBJ_SPACE;
				break;
			case 'o':
				t = OBJ_BLOCK;
				break;
			case 'O':
				t = OBJ_BLOCK_ON_GOAL;
				break;
			case '.':
				t = OBJ_GOAL;
				break;
			case 'p':
				t = OBJ_MAN;
				break;
			case 'P':
				t = OBJ_MAN_ON_GOAL;
				break;
			case '\n':
				x = 0;
				++y;
				t = OBJ_UNKNOWN;
				break;
			default:
				t = OBJ_UNKNOWN;
				break;
		}
		p++;

		// OBJ_UNKNOWN이 실제로 들어가면 안되므로
		// OBJ_UNKNOWN이 들어가는 상황을 제외하고 조건을 짜자.

		if (t != OBJ_UNKNOWN)			// OBJ_UNKNOWN은 안쓰는 정보니까
		{
			state[y * w + x] = t;
			++x;
		}
	}
}

void Draw(const Object* state, int w, int h)
{
	// Initialize 함수를 거꾸로하는 과정

	const char font[]{ ' ', '#', '.', 'o', 'O', 'p', 'P' }; 
	// enum을 통한 switch case를 쓰는 대신, 배열의 인덱스를 활용

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			Object o = state[y * w + x];
			// 1차배열 state에 저장되어있는 값들 (OBJ_뭐시기)
			// 새로 생성한 Object 타입의 변수 o에 옮기기

			// ex) state[0] = OBJ_WALL = 1
			// ex) Object o = 1
			std::cout << font[o];
		}
		std::cout << std::endl;
	}

	// switch case 구문을 사용하는 대신 간편하게 만들었다.
}

void Update(Object* state, char input, int w, int h)
{
	int dx{}, dy{};			// dx = delta x,  dy = delta y  (좌표 변화량)

	switch (input)
	{
		case 'a':
			dx = -1;
			break;

		case 'd':
			dx = 1;
			break;

		case 'w':
			dy = -1;
			break;

		case 's':
			dy = 1;
			break;
	}

	// 1차 배열에서 플레이어 위치 찾기
	int i{};
	for (i = 0; i < w * h; i++)
	{
		if (state[i] == OBJ_MAN || state[i] == OBJ_MAN_ON_GOAL)
		{
			break;		// 플레이어가 현재 몇번 인덱스에 있는지 확인후 i의 값 break
		}
	}
	// 플레이어의 위치의 인덱스 값 i가 나온다.

	int x{ i % w };
	int y{ i / w };				// 원래 좌표 x,y

	int tx{ x + dx };
	int ty{ y + dy };			// 이동한 좌표 tx,ty


	// 화면 밖으로 벗어났나?
	if (tx < 0 || ty < 0 || tx >= w || ty >= h)		//왼쪽 위 혹은 오른쪽 아래
	{
		return;
	}

	// 2차 인덱스를 다시 1차 인덱스로 바꾸기
	int p = y * w + x;
	int tp = ty * w + tx;

	if (state[tp] == OBJ_SPACE || state[tp] == OBJ_GOAL)
	{
		// 플레이어가 빈 공간으로 이동하는 경우  (p, 공백)
		// 출발 위치가 공백, 이동 위치가 p 
		
		// 플레이어가 GOAL로 이동하는 경우   (p, .)
		// 출발 위치가 공백, 이동 위치가 P

		// GOAL에 있는 플레이어가 빈 공간으로 이동하는 경우   (P, 공백)
		// 출발 위치가 ., 이동 위치가 p

		// GOAL에 있는 플레이어가 GOAL로 이동하는 경우   (P, .)
		// 출발 위치가 ., 이동 위치가 P

		// 출발 위치
		state[p] = (state[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
		// 이동 위치
		state[tp] = (state[tp] == OBJ_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;

		// if문 4개를 사용하는 대신, 3항연산자 코드 2개로 줄였다.
	}
	else if (state[tp] == OBJ_BLOCK || state[tp] == OBJ_BLOCK_ON_GOAL)
	{
		// 상자도 밀어야하고, 플레이어도 이동해야함
		// 여기서 상자가 밀릴수 있는 위치인지 아닌지도 확인하자

		// 플레이어 위치 x,y  상자위치 tx, ty
		// 상자가 이동한 위치 tx2,ty2 생성 
		int tx2 = tx + dx;
		int ty2 = ty + dy;

		// 상자가 이동한 위치가 화면 밖으로 벗어났는가?
		if (tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h)
		{
			return;
		}

		int tp2 = ty2 * w + tx2;

		// 상자 뒤에 상자가 또 있으면 이동 안된다.
		// 상자 뒤에 빈공간 혹은 GOAL만 가능

		if (state[tp2] == OBJ_SPACE || state[tp2] == OBJ_GOAL)
		{
			// 플레이어 위치, 상자 위치, 이동한 상자 위치
			// 3개의 위치를 바꿔줘야 한다.

			// 플레이어 위치
			state[p] = (state[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
			// 상자 위치
			state[tp] = (state[tp] == OBJ_BLOCK_ON_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;
			// 상자가 이동한 위치
			state[tp2] = (state[tp2] == OBJ_GOAL) ? OBJ_BLOCK_ON_GOAL : OBJ_BLOCK;
		}
	}
}

bool CheckClear(const Object* state, int w, int h)
{
	// 게임 클리어 여부 = 블럭이 하나라도 남았나?
	for (int i = 0; i < w * h; i++)
	{
		if (state[i] == OBJ_BLOCK)
		{
			return false;
		}
	}

	return true;
}
