﻿#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#define HEIGHT 48
#define WIDTH 64
#define INTERVAL 15

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

int SnakeDirection = NONE; //蛇的方向
SnakeStruct Snake;  //单向链表来表示蛇
Food food = Food{ 3, 0 }; //食物位置

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
			DrawPosition(S->X, S->Y, RED);
			S = S->Next;
		}
	}
}

//获取键盘输入的方向
int ChangeDirection()
{
	if (_getch())
	{
		switch (_getch())
		{
		case 72:
			return UP;
			break;
		case 80:
			return DOWN;
			break;
		case 75:
			return LEFT;
			break;
		case 77:
			return RIGHT;
			break;
		default:
			return NONE;
			break;
		}
	}
}

//蛇移动
void SnakeMove(int direction)
{
	//判断是否是相反的方向
	int isOppositeDirection = (direction == LEFT && SnakeDirection == RIGHT) ||
		(direction == RIGHT && SnakeDirection == LEFT) ||
		(direction == UP && SnakeDirection == DOWN) ||
		(direction == DOWN && SnakeDirection == UP) ||
		(direction == NONE && SnakeDirection == NONE);

	if (!isOppositeDirection)
	{
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

		//如果前方没有食物
		int hasFood = Snake->X == food.X && Snake->Y == food.Y;
		if (!hasFood)
		{
			if (Snake)
			{
				SnakeStruct S = Snake;
				while (S->Next)
				{
					S->Next->X = S->X;
					S->Next->Y = S->Y;
					S = S->Next;
				}
				Snake->X += x;
				Snake->Y += y;
			}
		}
		else
		{
			SnakeStruct NewHead = (SnakeStruct)malloc(sizeof(struct _Snake));
			if (NewHead)
			{
				NewHead->X = Snake->X + x;
				NewHead->Y = Snake->Y + y;
				NewHead->Next = Snake;
				Snake = NewHead;
			}
		}
		SnakeDirection = direction;
	}
}

//画食物
void DrawFood()
{
	DrawPosition(food.X, food.Y, GREEN);
}

//重新生成食物
void NewFood()
{

}

int main()
{
	initgraph(WIDTH * INTERVAL, HEIGHT * INTERVAL, SHOWCONSOLE);
	setbkcolor(WHITE);
	cleardevice();
	Snake = (SnakeStruct)malloc(sizeof(struct _Snake));
	if (Snake)
	{
		Snake->X = 1;
		Snake->Y = 0;
		SnakeStruct Snake1 = (SnakeStruct)malloc(sizeof(struct _Snake));
		if (Snake1)
		{
			Snake1->X = 0;
			Snake1->Y = 0;
			Snake1->Next = NULL;
			Snake->Next = Snake1;
		}

	}
	DrawSnake();
	DrawFood();
	Sleep(1000);
	SnakeMove(RIGHT);
	DrawSnake();
	DrawFood();
	Sleep(1000);
	SnakeMove(RIGHT);
	DrawSnake();
	DrawFood();
	Sleep(1000);
	SnakeMove(RIGHT);
	DrawSnake();
	DrawFood();
	Sleep(1000);
	SnakeMove(RIGHT);
	DrawSnake();
	DrawFood();
	Sleep(1000);
	SnakeMove(RIGHT);
	DrawSnake();
	DrawFood();
	_getch();
	closegraph();
}
