#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#define ROW 48
#define COL 64
#define WIDTH 20

struct _Snake;
typedef struct _Snake* SnakeStruct;

SnakeStruct Snake;
char SnakeDirectory;

struct _Snake
{
	int X;
	int Y;
	SnakeStruct Next;
};

void DrawPosition(int x, int y, COLORREF color)
{
	setfillcolor(color);
	fillrectangle(y * WIDTH, x * WIDTH, (y + 1) * WIDTH, (x + 1) * WIDTH);
}

void DrawSnake()
{
	if (Snake != NULL)
	{
		SnakeStruct s = Snake;
		do {
			DrawPosition(s->X, s->Y, RED);
			s = s->Next;
		} while (s != NULL);
	}
}

int main()
{
	initgraph(COL * WIDTH, ROW * WIDTH);
	setbkcolor(WHITE);
	cleardevice();

	Snake = (SnakeStruct)malloc(sizeof(struct _Snake));
	if (Snake)
	{
		Snake->X = 0;
		Snake->Y = 0;
		Snake->Next = NULL;
	}
	DrawSnake();

	_getch();
	closegraph();
}
