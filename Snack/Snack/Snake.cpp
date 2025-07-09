#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include <graphics.h>
#include<stdlib.h>
#include<time.h>
#include<conio.h>

//���峡����С
#define WIDTH 1040
#define HEIGHT 640
//����ʳ���Լ��ߵĴ�С
#define SIZE  20
//�����ߵĳ���
#define UP 1
#define DOWN -1
#define LEFT 2
#define RIGHT -2
//�ߵ���󳤶�
#define MAXLEN 1600


typedef struct {
	int x;
	int y;
}SnakeNode;

SnakeNode tmp[MAXLEN];	//������һ���������洢��ԭ����λ��
int speed = 150;		//�ߵ��ٶ�(����˯�ߺ�������)
IMAGE wallImage;       //�߽�ǽ
int score=0;	       //����
int difficulty_level = 0;//ѡ���Ѷȣ�Ĭ��Ϊ�㣨�򵥣�
ExMessage msg;     //�����Ϣ


//�����ߵ���
class Snake
{
	friend class Food;
	friend class Wall;
public:
	Snake();					//��ʼ��
	void Move();				//�ƶ�
	void Draw();				//������
	bool Eat(Food food);	    //��ʳ��
	bool Defeat(Wall wall);	//ʧ���ж�
	
private:
	int dirt;					//����
	int length;					//����
	SnakeNode node[MAXLEN];		//�ߵĽ��
	IMAGE ima1, ima2, ima3, ima4, ima5;//�ߵ�ͼƬ
};

//����ʳ�����
class Food
{
	friend class Snake;
	friend class Wall;
public:
	Food(Snake snake);			//ʳ���ʼ��
	void Draw();				//����ʳ��
private:
	int x, y;					//����
	IMAGE foodImage;
	int food_kind;//ʳ�����࣬1-3Ϊ����ʳ�4-9Ϊ��ͨʳ�10-12Ϊ����ʳ�
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
	char message1[] = "�㵱ǰ�ķ���Ϊ��";
	outtextxy(WIDTH-240,20, message1);
	char s1[6];
	sprintf(s1, "%d", score);
	outtextxy(WIDTH - 120,20, s1);//��ӡ����

	char message2[] = "���ߵ�ǰ���ٶ�Ϊ��";
	outtextxy(WIDTH - 240, 40, message2);
	char s2[6];
	int _speed = 300 - speed;//ת���ٶ�
	sprintf(s2, "%d", _speed);
	outtextxy(WIDTH - 120, 40, s2);//��ӡ�ٶ�
	//��ӡ���˵��
	char message3[] = "��ѡ���Ѷȣ�Ĭ��Ϊ�򵥣���";
	outtextxy(WIDTH - 240, 60, message3);
	char message4[] = "��"; char message5[] = "����"; char message6[] = "����";
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
		while (peekmessage(&msg, EX_MOUSE))//��ȡ�����Ϣ
		{
			if (botton()) goto table2;//�ж���������Ƿ��ǰ���
		}
		snake.Draw();
		food.Draw();  
		initWall();   
	    wall.Draw();   
		printMessage(); 
		FlushBatchDraw();
		EndBatchDraw();//˫���壬��ֹ��Ļһ��һ����
		if (snake.Eat(food)) {
			goto table1;
		}
		if (snake.Defeat(wall)) {
			break;
		}
		snake.Move(); 
	}
	//��ʾʧ����Ϣ
	HWND window = GetHWnd();
	SetWindowText(window, _T("��ʾ"));
	MessageBox(window, _T("��Ϸʧ��"), _T("��ʾ"), MB_OKCANCEL);
	return 0;
}

//�ߵĳ�ʼ��
Snake::Snake()
{
	this->dirt = RIGHT;
	this->length = 3;
	//�±���0��λ��Ϊ�ߵ�ͷ��
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

//�ƶ�
void Snake::Move() {
	//��ԭ�����߽�㿽��һ��
	for (int i = 0; i < this->length; i++) {
		tmp[i].x = this->node[i].x;
		tmp[i].y = this->node[i].y;
	}
	int status = 0;//�����ж��Ƿ�����ת�򰴼�
	if (this->dirt == RIGHT) {
		//�ж��Ƿ�ת��
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
		//�ж��Ƿ�ת��
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
		//�ж��Ƿ�ת��
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
		//�ж��Ƿ�ת��
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
	//�ƶ�
	for (int i = 1; i < this->length; i++) {
		this->node[i].x = tmp[i - 1].x;
		this->node[i].y = tmp[i - 1].y;
	}
	Sleep(speed);
}

//������
void Snake::Draw() {
	cleardevice();//���ԭ�ȵĻ�ͼ
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



//��ʳ��
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

//ʧ���ж�
bool Snake::Defeat(Wall wall) {
	//1.�����߽�
	if (this->node[0].x < 0 || this->node[0].x >= WIDTH-280 || this->node[0].y < 0 || this->node[0].y >= HEIGHT) {
		return true;
	}
	//2.�����Լ�������
	for (int i = 1; i < this->length; i++) {
		if (this->node[0].x == this->node[i].x && this->node[0].y == this->node[i].y) {
			return true;
		}
	}
	//3.�����ϰ���
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

//ʳ��ĳ�ʼ��
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
	this->food_kind = rand() % 12 + 1;//���ɲ�ͬ�����ʳ��
}

//����ʳ��
void Food::Draw() {
	putimage(this->x, this->y, &foodImage);
}

//��ʼ���ϰ���
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
		}//����������ߵ�λ��
		if (this->wallnode[i].x == food.x && this->wallnode[i].y == food.y) goto table3;//���������ʳ���λ��

		for (int p = 0; p < i; p++)
		{
			if (this->wallnode[p].x == this->wallnode[i].x && this->wallnode[p].y == this->wallnode[i].y) {
				goto table3;
			}
		}//���������֮ǰǽ��λ��
	}
	
	
}

//�����ϰ���
void Wall::Draw()
{
	for (int i = 0; i < num-1; i++)
	{
		putimage(this->wallnode[i].x, this->wallnode[i].y, &wallImage);
	}
}