#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<windows.h>
#include<time.h> //食物随机
#include<conio.h> //按键监控
//辅助宏定义
#define MAPHEIGHT 25
#define MAPWIDTH 60
#define SNAKESIZE 645

//数据设计/////// 

//食物的结构体

struct {
	//定位
	int x;
	int y;
}food;

//蛇的结构体
struct {
	//记录每一节蛇的坐标
	int x[SNAKESIZE];
	int y[SNAKESIZE];
	long int len;
	int speed;
}snake;

//全局变量////////
int key; //移动方向
int changeFlag = 0;//蛇的变化标记
int mark = 0;//积分
void drawMap();
void gotoxy(int x, int y);
void keyDown();
int snakeStatus();
void createFood();
void showScore();
void newGame();
int main();

//1.画地图
void drawMap() {
	//O蛇身  e食物
	srand((unsigned int)time(NULL));
	//1.圈墙圈地
	//1.1 左右边框
	for (int i = 0; i < MAPHEIGHT; i++) {
		gotoxy(0, i);
		printf("##");
		gotoxy(MAPWIDTH - 3, i);
		printf("##");

	}
	//1.2 上下边框
	for (int i = 0; i < MAPWIDTH - 1; i++) {
		gotoxy(i, 0);
		printf("#");
		gotoxy(i, MAPHEIGHT - 1);
		printf("#");

	}
	//2.画蛇s
	//2.1确定蛇的属性
	snake.len = 3;
	snake.speed = 240;
	//开头蛇在屏幕的中间
	snake.x[0] = MAPWIDTH / 2;
	snake.y[0] = MAPHEIGHT / 2;
	//2.2 画蛇
	//画蛇头
	gotoxy(snake.x[0], snake.y[0]);
	printf("@");
	//画剩下身体
	for (int k = 1; k < snake.len; k++) {
		snake.x[k] = snake.x[k - 1] + 2;
		snake.y[k] = snake.y[k - 1];
		gotoxy(snake.x[k], snake.y[k]);
		printf("@");
	}
	//3.画食物
		//3.1确定坐标
	while (1) {
		food.x = rand() % (MAPWIDTH - 4) + 2;
		food.y = rand() % (MAPHEIGHT - 2) + 1;
		if (food.x % 2 == 0) {
			break;
		}
	}
	//3.2画出来就可以了
	gotoxy(food.x, food.y);
	printf("$");

}
//2.食物的产生
void createFood() {
	//蛇把食物吃了
	if (snake.x[0] == food.x && snake.y[0] == food.y) {
		++mark;
		//产生的食物不能再蛇的身上，并且坐标是偶数
		srand((unsigned int)time(NULL));
		while (1) {
			int flag = 1;
			food.x = rand() % (MAPWIDTH - 4) + 2;
			food.y = rand() % (MAPHEIGHT - 2) + 1;
			//产生的食物不能再蛇的身上
			for (int k = 0; k < snake.len; k++) {
				if (snake.x[k] == food.x && snake.y[k] == food.y) {
					flag = 0;
					break;
				}
			}
			if (flag && food.x % 2 == 0) {
				break;
			}
		}
		gotoxy(food.x, food.y);
		printf("$");
		snake.len++;
		changeFlag = 1;//蛇的标记是1	
	}
}

//3.按键操作
void keyDown()
{
	//无按键的处理
	if (_kbhit()) {
		fflush(stdin);
		key = _getch();
	}
	//擦除最后的位置
	if (!changeFlag) {
		gotoxy(snake.x[snake.len - 1], snake.y[snake.len - 1]);
		printf(" ");
		//后面的蛇身
	}
	for (int i = snake.len - 1; i > 0; i--) {
		snake.x[i] = snake.x[i - 1];
		snake.y[i] = snake.y[i - 1];
	}
	//移动方向的处理
	switch (key) {
	case 'w':
	case 'W':
		snake.y[0]--;
		break;
	case 's':
	case 'S':
		snake.y[0]++;
		break;
	case 'A':
	case 'a':
		snake.x[0] -= 2;
		break;
	case 'd':
	case 'D':
		snake.x[0] += 2;
		break;
	}
	gotoxy(snake.x[0], snake.y[0]);
	printf("@");
	changeFlag = 0;
	gotoxy(MAPHEIGHT + 2, 0);  //移动不能一直看着光标

}

//4.蛇的状态:判读是否结束游戏
int snakeStatus() {
	if (snake.x[0] == 0 || snake.x[0] == MAPWIDTH - 2 || snake.y[0] == 0 || snake.y[0] == MAPHEIGHT - 1)
		return 0;
	for (int k = 1; k < snake.len; k++) {
		if (snake.x[0] == snake.x[k] && snake.y[k] == snake.y[0])
			return 0;
	}
	return 1;
}

//5.结束游戏分数显示
void showScore() {
	//擦除蛇身
	for (int i = 0; i < snake.len; i++)
	{
		gotoxy(snake.x[i], snake.y[i]);
		printf(" ");
	}
	//擦除食物
	gotoxy(food.x, food.y);
	printf(" ");
	//中央打印分数
	gotoxy((MAPWIDTH / 2) - 9, (MAPHEIGHT / 2) - 1);
	printf("****************");
	gotoxy((MAPWIDTH / 2) - 9, MAPHEIGHT / 2);
	printf("  your score:%d ", mark);
	gotoxy((MAPWIDTH / 2) - 9, (MAPHEIGHT / 2) + 1);
	printf("****************");
}

//6.重新开始游戏询问
void newGame() {
	
	char bo = 'n';
	gotoxy(0, MAPHEIGHT + 1);
	printf("是否重新开始游戏(y/n):");
	bo = getchar();
	getchar();
	if (bo == 'y') {
		gotoxy((MAPWIDTH / 2) - 9, (MAPHEIGHT / 2) - 1);
		printf("                ");
		gotoxy((MAPWIDTH / 2) - 9, MAPHEIGHT / 2);
		printf("                         ", mark);
		gotoxy((MAPWIDTH / 2) - 9, (MAPHEIGHT / 2) + 1);
		printf("                ");
		gotoxy(0, MAPHEIGHT + 1);
		printf("                           ");
		main();
	}
	else {
		return;
	}
}

//7.辅助函数：光标移动
void gotoxy(int x, int y) {
	//调用win32 API 去设置控制台的光标位置
	//1.找到控制台的这个窗口
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	//2.光标的结构体
	COORD coord;
	//3.设置坐标
	coord.X = x;
	coord.Y = y;
	//4.同步到控制台
	SetConsoleCursorPosition(handle, coord);
}

int main() {
	mark = 0;//初始化分数
	key = 'a';//初始化移动方向
	gotoxy(0, MAPHEIGHT);
	printf("W:Up  S:Down  A:Left  D:Right");
	drawMap();
	while (1) {
		createFood();
		Sleep(snake.speed);
		keyDown();
		if (!snakeStatus()) {
			break;
		}
	}
	showScore();
	newGame();
	return 0;
}