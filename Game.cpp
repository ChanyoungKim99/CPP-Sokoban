#include <iostream>

const char gStageData[]{"\
########\n\
# .. p #\n\
# oo   #\n\
#      #\n\
########"
};				
					// \�� �̿��ؼ� ���ڿ��� ���� ���� ���⽱�� ������ �� �ִ�
	
const int gStageWidth{ 8 };
const int gStageHeight{ 5 };
					// ����� ���������� ����ص� ��������.

enum Object
{
	OBJ_SPACE,
	OBJ_WALL,
	OBJ_GOAL,
	OBJ_BLOCK,
	OBJ_BLOCK_ON_GOAL,
	OBJ_MAN,
	OBJ_MAN_ON_GOAL,
						// enum�̹Ƿ� "���" �̴�.

	OBJ_UNKNOWN			// �������� �ʿ��� ����
};

void Initialize(Object* state, int w, int h, const char* stageData);
void Draw(const Object* state, int w, int h);
void Update(Object* state, char input, int w, int h);
bool CheckClear(const Object* state, int w, int h);

// �������ʴ� �����ʹ� ��� const�� ����ϴ� ���� ���� ���α׷����̴�.
// ���� �ִ� �Լ����� ��� ���Ǳ��� ����� ��� =
// �Լ����� �巡��, ��Ŭ���ؼ� ���� �۾� �� �����͸� - ����/���� �����

int main()
{
	// state = �����迭�� �Ҵ��� 1�� �迭 enum
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
	// gStageData�� enum(������) Object��
	const char* p = stageData;
	int x{}, y{};

	while (*p != '\0')
	{
		// *p -> stageData�� ������ ����

		Object t;		// Object type�� ����t ����

		// p�� �������� �ϸ鼭 stageData�� ���� �ӽ� Object t�� �ű�鼭 ġȯ,
		// �� ġȯ�� ���� �ٽ� 1���迭�� state�� �ű�

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

		// OBJ_UNKNOWN�� ������ ���� �ȵǹǷ�
		// OBJ_UNKNOWN�� ���� ��Ȳ�� �����ϰ� ������ ¥��.

		if (t != OBJ_UNKNOWN)			// OBJ_UNKNOWN�� �Ⱦ��� �����ϱ�
		{
			state[y * w + x] = t;
			++x;
		}
	}
}

void Draw(const Object* state, int w, int h)
{
	// Initialize �Լ��� �Ųٷ��ϴ� ����

	const char font[]{ ' ', '#', '.', 'o', 'O', 'p', 'P' }; 
	// enum�� ���� switch case�� ���� ���, �迭�� �ε����� Ȱ��

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			Object o = state[y * w + x];
			// 1���迭 state�� ����Ǿ��ִ� ���� (OBJ_���ñ�)
			// ���� ������ Object Ÿ���� ���� o�� �ű��

			// ex) state[0] = OBJ_WALL = 1
			// ex) Object o = 1
			std::cout << font[o];
		}
		std::cout << std::endl;
	}

	// switch case ������ ����ϴ� ��� �����ϰ� �������.
}

void Update(Object* state, char input, int w, int h)
{
	int dx{}, dy{};			// dx = delta x,  dy = delta y  (��ǥ ��ȭ��)

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

	// 1�� �迭���� �÷��̾� ��ġ ã��
	int i{};
	for (i = 0; i < w * h; i++)
	{
		if (state[i] == OBJ_MAN || state[i] == OBJ_MAN_ON_GOAL)
		{
			break;		// �÷��̾ ���� ��� �ε����� �ִ��� Ȯ���� i�� �� break
		}
	}
	// �÷��̾��� ��ġ�� �ε��� �� i�� ���´�.

	int x{ i % w };
	int y{ i / w };				// ���� ��ǥ x,y

	int tx{ x + dx };
	int ty{ y + dy };			// �̵��� ��ǥ tx,ty


	// ȭ�� ������ �����?
	if (tx < 0 || ty < 0 || tx >= w || ty >= h)		//���� �� Ȥ�� ������ �Ʒ�
	{
		return;
	}

	// 2�� �ε����� �ٽ� 1�� �ε����� �ٲٱ�
	int p = y * w + x;
	int tp = ty * w + tx;

	if (state[tp] == OBJ_SPACE || state[tp] == OBJ_GOAL)
	{
		// �÷��̾ �� �������� �̵��ϴ� ���  (p, ����)
		// ��� ��ġ�� ����, �̵� ��ġ�� p 
		
		// �÷��̾ GOAL�� �̵��ϴ� ���   (p, .)
		// ��� ��ġ�� ����, �̵� ��ġ�� P

		// GOAL�� �ִ� �÷��̾ �� �������� �̵��ϴ� ���   (P, ����)
		// ��� ��ġ�� ., �̵� ��ġ�� p

		// GOAL�� �ִ� �÷��̾ GOAL�� �̵��ϴ� ���   (P, .)
		// ��� ��ġ�� ., �̵� ��ġ�� P

		// ��� ��ġ
		state[p] = (state[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
		// �̵� ��ġ
		state[tp] = (state[tp] == OBJ_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;

		// if�� 4���� ����ϴ� ���, 3�׿����� �ڵ� 2���� �ٿ���.
	}
	else if (state[tp] == OBJ_BLOCK || state[tp] == OBJ_BLOCK_ON_GOAL)
	{
		// ���ڵ� �о���ϰ�, �÷��̾ �̵��ؾ���
		// ���⼭ ���ڰ� �и��� �ִ� ��ġ���� �ƴ����� Ȯ������

		// �÷��̾� ��ġ x,y  ������ġ tx, ty
		// ���ڰ� �̵��� ��ġ tx2,ty2 ���� 
		int tx2 = tx + dx;
		int ty2 = ty + dy;

		// ���ڰ� �̵��� ��ġ�� ȭ�� ������ ����°�?
		if (tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h)
		{
			return;
		}

		int tp2 = ty2 * w + tx2;

		// ���� �ڿ� ���ڰ� �� ������ �̵� �ȵȴ�.
		// ���� �ڿ� ����� Ȥ�� GOAL�� ����

		if (state[tp2] == OBJ_SPACE || state[tp2] == OBJ_GOAL)
		{
			// �÷��̾� ��ġ, ���� ��ġ, �̵��� ���� ��ġ
			// 3���� ��ġ�� �ٲ���� �Ѵ�.

			// �÷��̾� ��ġ
			state[p] = (state[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
			// ���� ��ġ
			state[tp] = (state[tp] == OBJ_BLOCK_ON_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;
			// ���ڰ� �̵��� ��ġ
			state[tp2] = (state[tp2] == OBJ_GOAL) ? OBJ_BLOCK_ON_GOAL : OBJ_BLOCK;
		}
	}
}

bool CheckClear(const Object* state, int w, int h)
{
	// ���� Ŭ���� ���� = ���� �ϳ��� ���ҳ�?
	for (int i = 0; i < w * h; i++)
	{
		if (state[i] == OBJ_BLOCK)
		{
			return false;
		}
	}

	return true;
}
