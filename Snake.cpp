#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <atlstr.h>

#define HEIGHT 24
#define WIDTH 32
#define INTERVAL 20
#define DEFAULT_TIME_INTERVAL 500

/******************************结构体和枚举*****************************/

struct _Snake;
struct Food;
typedef struct _Snake* SnakeStruct;

struct _Snake
{
	int X;
	int Y;
	SnakeStruct Next;
};

struct Food
{
	int X;
	int Y;
};

enum Direction
{
	LEFT, RIGHT, UP, DOWN, NONE
};

/******************************全局变量*****************************/

HWND hwnd;
int SnakeDirection = RIGHT; //蛇的方向
int direction; //输入方向
SnakeStruct Snake;  //单向链表来表示蛇
Food food; //食物位置
long t1, t2; //时间控制
DWORD TimeInterval = DEFAULT_TIME_INTERVAL;
int Score;

/******************************方法定义*****************************/

void DrawPosition(int x, int y, COLORREF color);
void DrawSnake();
int ChangeDirection();
int IsFoodOk();
void NewFood();
void SnakeMove(int direction);
void DrawFood();
void Init();

/******************************具体方法实现*****************************/

//画一个点
void DrawPosition(int x, int y, COLORREF color)
{
	setfillcolor(color);
	fillrectangle(x * INTERVAL, y * INTERVAL, (x + 1) * INTERVAL, (y + 1) * INTERVAL);
}

//画蛇
void DrawSnake()
{
	if (Snake != NULL)
	{
		cleardevice();
		SnakeStruct S = Snake;
		while (S != NULL)
		{
			if (S == Snake)
			{
				DrawPosition(S->X, S->Y, RED);
			} 
			else
			{
				DrawPosition(S->X, S->Y, LIGHTRED);
			}
			S = S->Next;
		}
	}
}

//获取键盘输入的方向
int ChangeDirection()
{
	int c = _getch();
	if (c == 224)
	{
		c = _getch();
	}
	if (c)
	{
		switch (c)
		{
		case 'W':
		case 'w':
		case 72:
			return UP;
			break;
		case 'S':
		case 's':
		case 80:
			return DOWN;
			break;
		case 'A':
		case 'a':
		case 75:
			return LEFT;
			break;
		case 'D':
		case 'd':
		case 77:
			return RIGHT;
			break;
		default:
			return SnakeDirection;
			break;
		}
	}
	else
	{
		return SnakeDirection;
	}
}

//新生成的食物是否合法
int IsFoodOk()
{
	int isOk = 1;
	SnakeStruct S = Snake;
	while (S)
	{
		if (S->X == food.X && S->Y == food.Y)
		{
			isOk = 0;
		}
		S = S->Next;
	}
	return isOk;
}

//重新生成食物
void NewFood()
{
	while (!IsFoodOk())
	{
		srand((unsigned)time(NULL));
		int x = rand() % WIDTH, y = rand() % HEIGHT;
		food = Food{ x,y };
	}
}

//蛇移动
void SnakeMove(int direction)
{
	//判断是否是相反的方向
	int isOppositeDirection = (direction == LEFT && SnakeDirection == RIGHT) ||
		(direction == RIGHT && SnakeDirection == LEFT) ||
		(direction == UP && SnakeDirection == DOWN) ||
		(direction == DOWN && SnakeDirection == UP);

	if (isOppositeDirection)
	{
		direction = SnakeDirection;
	}

	int x = 0;
	int y = 0;
	switch (direction)
	{
	case LEFT:
		x = -1;
		break;
	case RIGHT:
		x = 1;
		break;
	case UP:
		y = -1;
		break;
	case DOWN:
		y = 1;
		break;
	default:
		break;
	}

	//如果前方有墙壁则游戏结束
	int crash = 0;
	crash = Snake->X + x < 0 || Snake->X + x >= WIDTH || Snake->Y + y < 0 || Snake->Y + y >= HEIGHT;
	if (!crash)
	{
		//如果碰撞到自己的身体，则游戏结束
		SnakeStruct S = Snake;
		while (S)
		{
			if (S->X == Snake->X + x && S->Y == Snake->Y + y)
			{
				crash = 1;
			}
			S = S->Next;
		}
	}

	if (!crash && Snake)
	{
		//如果前方没有食物
		int hasFood = Snake->X + x == food.X && Snake->Y + y == food.Y;
		if (!hasFood)
		{
			SnakeStruct S = Snake;
			int currentX = S->X; //当前节点的X坐标
			int currentY = S->Y; //当前节点的Y坐标
			int nextX; //下一个节点的X坐标
			int nextY; //下一个节点的Y坐标
			while (S->Next)
			{
				nextX = S->Next->X;
				nextY = S->Next->Y;
				S->Next->X = currentX;
				S->Next->Y = currentY;
				S = S->Next;
				currentX = nextX;
				currentY = nextY;
			}
			Snake->X += x;
			Snake->Y += y;

		}
		else //前方有食物
		{
			SnakeStruct NewHead = (SnakeStruct)malloc(sizeof(struct _Snake));
			if (NewHead)
			{
				NewHead->X = Snake->X + x;
				NewHead->Y = Snake->Y + y;
				NewHead->Next = Snake;
				Snake = NewHead;
			}
			Score += 1;
			if (Score == WIDTH * HEIGHT)
			{
				MessageBox(hwnd, _T("You Win"), _T("Game Over"), MB_OK);
				Init();
			}
			TimeInterval = TimeInterval - Score <= 100 ? 100 : TimeInterval - Score;
			NewFood();
		}
		SnakeDirection = direction;
	}
	else
	{
		CString cstr;
		cstr.Format(_T("Game Over, Score: %d"), Score);
		MessageBox(hwnd, cstr, _T("Game Over"), MB_OK);
		Init();
	}
}

//画食物
void DrawFood()
{
	DrawPosition(food.X, food.Y, GREEN);
}

void Init()
{
	hwnd = initgraph(WIDTH * INTERVAL, HEIGHT * INTERVAL, NULL);
	setbkcolor(WHITE);
	cleardevice();
	if (Snake)
	{
		SnakeStruct S;
		while (Snake)
		{
			S = Snake;
			Snake = Snake->Next;
			free(S);
		}
	}
	Snake = (SnakeStruct)malloc(sizeof(struct _Snake));
	if (Snake)
	{
		Snake->X = 2;
		Snake->Y = 0;
		SnakeStruct Snake1 = (SnakeStruct)malloc(sizeof(struct _Snake));
		if (Snake1)
		{
			Snake1->X = 1;
			Snake1->Y = 0;
			Snake1->Next = NULL;
			Snake->Next = Snake1;
			SnakeStruct Snake2 = (SnakeStruct)malloc(sizeof(struct _Snake));
			if (Snake2)
			{
				Snake2->X = 0;
				Snake2->Y = 0;
				Snake2->Next = NULL;
				Snake1->Next = Snake2;
			}
		}
	}
	Score = 0;
	SnakeDirection = RIGHT;
	direction = SnakeDirection;
	TimeInterval = DEFAULT_TIME_INTERVAL;
	DrawSnake();
	NewFood();
	DrawFood();
}

int main()
{
	Init();
	t1 = GetTickCount();
	while (1)
	{
		if (_kbhit())
		{
			direction = ChangeDirection();
		}
		t2 = GetTickCount();
		if (t2 - t1 >= TimeInterval)
		{
			SnakeMove(direction);
			DrawSnake();
			DrawFood();
			t1 = t2;
		}
		//避免不必要的循环
		Sleep(TimeInterval / 2);
	}
	_getch();
	closegraph();
}
