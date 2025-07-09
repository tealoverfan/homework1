#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include <graphics.h>
#include<stdlib.h>
#include<time.h>
#include<conio.h>

//定义场景大小
#define WIDTH 1040
#define HEIGHT 640
//定义食物以及蛇的大小
#define SIZE  20
//定义蛇的朝向
#define UP 1
#define DOWN -1
#define LEFT 2
#define RIGHT -2
//蛇的最大长度
#define MAXLEN 1600


typedef struct {
	int x;
	int y;
}SnakeNode;

SnakeNode tmp[MAXLEN];	//用另外一个数组来存储蛇原来的位置
int speed = 150;		//蛇的速度(用在睡眠函数里面)
IMAGE wallImage;       //边界墙
int score=0;	       //分数
int difficulty_level = 0;//选择难度，默认为零（简单）
ExMessage msg;     //鼠标信息


//创建蛇的类
class Snake
{
	friend class Food;
	friend class Wall;
public:
	Snake();					//初始化
	void Move();				//移动
	void Draw();				//绘制蛇
	bool Eat(Food food);	    //吃食物
	bool Defeat(Wall wall);	//失败判定
	
private:
	int dirt;					//朝向
	int length;					//长度
	SnakeNode node[MAXLEN];		//蛇的结点
	IMAGE ima1, ima2, ima3, ima4, ima5;//蛇的图片
};

//创建食物的类
class Food
{
	friend class Snake;
	friend class Wall;
public:
	Food(Snake snake);			//食物初始化
	void Draw();				//绘制食物
private:
	int x, y;					//坐标
	IMAGE foodImage;
	int food_kind;//食物种类，1-3为减速食物，4-9为普通食物，10-12为加数食物；
};

class Wall
{
	friend class Snake;
	friend class Food;
public:
	Wall(Snake snake,Food food);
	void Draw();
private:
	
	int num;
	SnakeNode wallnode[MAXLEN];
};



void initWall()
{
	loadimage(&wallImage, "wall.png", SIZE, SIZE);
	int wall_x = WIDTH - 260;
	int wall_y = 0;
	while (wall_y <= HEIGHT)
	{
		putimage(wall_x, wall_y, &wallImage);
		wall_y += 20;
	}
}
void printMessage()
{
	char message1[] = "你当前的分数为：";
	outtextxy(WIDTH-240,20, message1);
	char s1[6];
	sprintf(s1, "%d", score);
	outtextxy(WIDTH - 120,20, s1);//打印分数

	char message2[] = "青蛇当前的速度为：";
	outtextxy(WIDTH - 240, 40, message2);
	char s2[6];
	int _speed = 300 - speed;//转化速度
	sprintf(s2, "%d", _speed);
	outtextxy(WIDTH - 120, 40, s2);//打印速度
	//打印相关说明
	char message3[] = "可选择难度（默认为简单）：";
	outtextxy(WIDTH - 240, 60, message3);
	char message4[] = "简单"; char message5[] = "困难"; char message6[] = "炼狱";
	outtextxy(WIDTH - 180, 80, message4);
	outtextxy(WIDTH - 180, 100, message5);
	outtextxy(WIDTH - 180, 120, message6);
}


 bool botton()
{


		if (msg.message == WM_LBUTTONDOWN)
		{
			if (msg.x >= WIDTH - 180 && msg.x <= WIDTH - 140 && msg.y >= 80 && msg.y <= 100) {
				difficulty_level = 0;
				return true;
			}
			if (msg.x >= WIDTH - 180 && msg.x <= WIDTH - 140 && msg.y >100 && msg.y <= 120)
			{
				difficulty_level = 2;
				return true;
			}
			if (msg.x >= WIDTH - 180 && msg.x <= WIDTH - 140 && msg.y >120 && msg.y <= 140)
			{
				difficulty_level = 4;
				return true;
			}
		
			return false;
		}
		return false;

}

int main() {
	initgraph(WIDTH, HEIGHT);
	Snake snake;
	
table1:
	Food food(snake);
table2:
	Wall wall(snake, food);
	while (1) {
		BeginBatchDraw();
		FlushBatchDraw();
		while (peekmessage(&msg, EX_MOUSE))//获取鼠标信息
		{
			if (botton()) goto table2;//判断鼠标点击处是否是按键
		}
		snake.Draw();
		food.Draw();  
		initWall();   
	    wall.Draw();   
		printMessage(); 
		FlushBatchDraw();
		EndBatchDraw();//双缓冲，防止屏幕一闪一闪的
		if (snake.Eat(food)) {
			goto table1;
		}
		if (snake.Defeat(wall)) {
			break;
		}
		snake.Move(); 
	}
	//提示失败信息
	HWND window = GetHWnd();
	SetWindowText(window, _T("提示"));
	MessageBox(window, _T("游戏失败"), _T("提示"), MB_OKCANCEL);
	return 0;
}

//蛇的初始化
Snake::Snake()
{
	this->dirt = RIGHT;
	this->length = 3;
	//下标是0的位置为蛇的头部
	for (int i = 0; i < length; i++) {
		this->node[i].x = 60 - ((i + 1) * SIZE);
		this->node[i].y = 0;
	}
	loadimage(&ima1, "snakeHeadUp.png", SIZE, SIZE);
	loadimage(&ima2, "snakeHeadDown.png", SIZE, SIZE);
	loadimage(&ima3, "snakeHeadLeft.png", SIZE, SIZE);
	loadimage(&ima4, "snakeHeadRight.png", SIZE, SIZE);
	loadimage(&ima5, "snakeTail.png", SIZE, SIZE);
}

//移动
void Snake::Move() {
	//将原来的蛇结点拷贝一份
	for (int i = 0; i < this->length; i++) {
		tmp[i].x = this->node[i].x;
		tmp[i].y = this->node[i].y;
	}
	int status = 0;//用来判断是否点击了转向按键
	if (this->dirt == RIGHT) {
		//判断是否转向
		if (GetAsyncKeyState('W') && status == 0) {
			//this->node[0].y -= SIZE;
			this->dirt = UP;
			status = 1;
		}
		else if (GetAsyncKeyState('S') && status == 0) {
			this->dirt = DOWN;
			status = 1;
		}
		else {
			this->node[0].x += SIZE;
		}
	}
	if (this->dirt == DOWN) {
		//判断是否转向
		if (GetAsyncKeyState('A') && status == 0) {
			//this->node[0].x -= SIZE;
			this->dirt = LEFT;
			status = 1;
		}
		else if (GetAsyncKeyState('D') && status == 0) {
			this->node[0].x += SIZE;
			this->dirt = RIGHT;
			status = 1;
		}
		else {
			this->node[0].y += SIZE;
		}
	}
	if (this->dirt == LEFT) {
		//判断是否转向
		if (GetAsyncKeyState('W') && status == 0) {
			//this->node[0].y -= SIZE;
			this->dirt = UP;
			status = 1;
		}
		else if (GetAsyncKeyState('S') && status == 0) {
			this->node[0].y += SIZE;
			this->dirt = DOWN;
			status = 1;
		}
		else {
			this->node[0].x -= SIZE;
		}
	}
	if (this->dirt == UP) {
		//判断是否转向
		if (GetAsyncKeyState('A') && status == 0) {
			this->node[0].x -= SIZE;
			this->dirt = LEFT;
			status = 1;
		}
		else if (GetAsyncKeyState('D') && status == 0) {
			this->node[0].x += SIZE;
			this->dirt = RIGHT;
			status = 1;
		}
		else {
			this->node[0].y -= SIZE;
		}
	}
	//移动
	for (int i = 1; i < this->length; i++) {
		this->node[i].x = tmp[i - 1].x;
		this->node[i].y = tmp[i - 1].y;
	}
	Sleep(speed);
}

//绘制蛇
void Snake::Draw() {
	cleardevice();//清空原先的绘图
	for (int i = 0; i < this->length; i++) {
		if (i == 0) {
			if (this->dirt == UP) { putimage(this->node[i].x, this->node[i].y, &ima1); }
			else if (this->dirt == DOWN) { putimage(this->node[i].x, this->node[i].y, &ima2); }
			else if (this->dirt == LEFT) { putimage(this->node[i].x, this->node[i].y, &ima3); }
			else  { putimage(this->node[i].x, this->node[i].y, &ima4); }
	}
		else {
			putimage(this->node[i].x, this->node[i].y, &ima5);
		}
	}
	
}



//吃食物
bool Snake::Eat(Food food) {
	if (food.x == this->node[0].x && food.y == this->node[0].y) {
		if (food.food_kind >=9 ) { 
			if(speed>20) speed -= 20; 
			score += 30; 
		}
		else if (food.food_kind <=2) { speed += 20; score += 10; }
		else { score += 20; }

		if (this->node[length - 1].x - this->node[length - 2].x == 0 && this->node[length - 1].y - this->node[length - 2].y == -20) {
			this->length++;
			this->node[length - 1].x = this->node[length - 2].x;
			this->node[length - 1].y = this->node[length - 2].y - SIZE;
		}
		else if (this->node[length - 1].x - this->node[length - 2].x == 0 && this->node[length - 1].y - this->node[length - 2].y == 20) {
			this->length++;
			this->node[length - 1].x = this->node[length - 2].x;
			this->node[length - 1].y = this->node[length - 2].y + SIZE;
		}
		else if (this->node[length - 1].x - this->node[length - 2].x == 20 && this->node[length - 1].y - this->node[length - 2].y == 0) {
			this->length++;
			this->node[length - 1].x = this->node[length - 2].x + SIZE;
			this->node[length - 1].y = this->node[length - 2].y;
		}
		else /*(this->node[length - 1].x - this->node[length - 2].x == -20 && this->node[length - 1].y - this->node[length - 2].y == 0)*/ {
			this->length++;
			this->node[length - 1].x = this->node[length - 2].x - SIZE;
			this->node[length - 1].y = this->node[length - 2].y;
		}

		return true;
	}
	return false;
}

//失败判定
bool Snake::Defeat(Wall wall) {
	//1.碰到边界
	if (this->node[0].x < 0 || this->node[0].x >= WIDTH-280 || this->node[0].y < 0 || this->node[0].y >= HEIGHT) {
		return true;
	}
	//2.碰到自己的身体
	for (int i = 1; i < this->length; i++) {
		if (this->node[0].x == this->node[i].x && this->node[0].y == this->node[i].y) {
			return true;
		}
	}
	//3.碰到障碍物
	if (difficulty_level)
	{
		for (int i = 0; i < wall.num-1; i++) {
			if (this->node[0].x == wall.wallnode[i].x && this->node[0].y == wall.wallnode[i].y) {
				return true;
			}
		}
	}
	return false;
}

//食物的初始化
Food::Food(Snake snake)
{
	loadimage(&foodImage, "china.png", SIZE, SIZE);
table:
	srand((unsigned)time(NULL));
	this->x = (rand() % ((WIDTH-280) / SIZE)) * SIZE;
	this->y = (rand() % (HEIGHT / SIZE)) * SIZE;
	
	for (int i = 0; i < snake.length; i++) {
		if (snake.node[i].x == this->x && snake.node[i].y == this->y) {
			goto table;
		}
	}
	this->food_kind = rand() % 12 + 1;//生成不同种类的食物
}

//绘制食物
void Food::Draw() {
	putimage(this->x, this->y, &foodImage);
}

//初始化障碍物
Wall::Wall(Snake snake,Food food)
{
	this->num = difficulty_level * 10 + 1;

	for (int i = 0; i < num-1; i++)
	{
	table3:
		this->wallnode[i].x = (rand() % ((WIDTH - 280) / SIZE)) * SIZE;
		this->wallnode[i].y = (rand() % (HEIGHT / SIZE)) * SIZE;
		
		for (int j = 0; j < snake.length; j++) {
			if (snake.node[j].x == this->wallnode[i].x && snake.node[j].y == this->wallnode[i].y) {
				goto table3;
			}
		}//避免出现在蛇的位置
		if (this->wallnode[i].x == food.x && this->wallnode[i].y == food.y) goto table3;//避免出现在食物的位置

		for (int p = 0; p < i; p++)
		{
			if (this->wallnode[p].x == this->wallnode[i].x && this->wallnode[p].y == this->wallnode[i].y) {
				goto table3;
			}
		}//避免出现在之前墙的位置
	}
	
	
}

//绘制障碍物
void Wall::Draw()
{
	for (int i = 0; i < num-1; i++)
	{
		putimage(this->wallnode[i].x, this->wallnode[i].y, &wallImage);
	}
}