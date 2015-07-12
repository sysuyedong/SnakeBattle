#include "Snake.h"
#include "World.h"
#include <cmath>
#include "cocos2d.h"
using namespace cocos2d;
Snake::Snake(int type){
	//������
	if(type == 1){
		//��ʼ����ͷ�����������
		for(int i=0 ; i<5 ; ++i){
			snakeX[i] = i+1;
			snakeY[i] = 1;
		}
		snakeLength = 5;
		snakeHead = 4;
		snakeTail = 0;
	
		isLive = false;
		direction = 1;		//��ʼ������
		counter = 0;
	}
	//������
	if(type == 2){
		//��ʼ����ͷ�����������
		for(int i=0 ; i<5 ; ++i){
			snakeX[i] = i+1;
			snakeY[i] = 10;
		}
		snakeLength = 5;
		snakeHead = 4;
		snakeTail = 0;

		isLive = false;
		direction = 1;		//��ʼ������
		counter = 0;
	}
}

bool Snake::isAlive(){
	return isLive;
}

int Snake::move(int dy, int dx){
	counter = ++counter % MAX_LENGTH;
	//get tail position and update tail
	int x = snakeX[snakeTail];
	int y = snakeY[snakeTail];
	snakeTail = ++snakeTail % MAX_LENGTH;
	//leave blank on snake tail
	World::map[15 - y][x] = BLANK_CELL;

	//get old-head posstion
	int hx = snakeX[snakeHead];
	int hy = snakeY[snakeHead];
	//change old-head to snake body
	World::map[15 - hy][hx] = SNAKE_BODY;
	// update head with new head
	snakeHead = ++snakeHead % MAX_LENGTH;
	hx = hx + dx;
	hy = hy + dy;
	snakeX[snakeHead] = hx;
	snakeY[snakeHead] = hy;

	//is i am dead or ...
	if (World::map[15 - hy][hx] == SNAKE_FOOD) {
		//�߳���
		snakeLength++;
		if (snakeLength >= MAX_LENGTH) {
			isLive = false;
			return -1;
		}
		counter = --counter % MAX_LENGTH;
		//restore snake tail
		snakeTail = --snakeTail % MAX_LENGTH;
		if (snakeTail < 0) snakeTail += MAX_LENGTH;  //C aka bug is difficult to find!!!
		int x = snakeX[snakeTail];
		int y = snakeY[snakeTail];
		World::map[15 - y][x] = SNAKE_BODY;        
		//update now head on map
		World::map[15 - hy][hx] = SNAKE_HEAD;
		//put a new food
		return 1;
	}
	else if (World::map[15 - hy][hx] == BLANK_CELL) {
		//update now head on map
		World::map[15 - hy][hx] = SNAKE_HEAD;
	} 
	else {
		// snake dead
		isLive = false;
		return -1;
	}
	return 0;
}

int Snake::getDirection(){
	return direction;
}

void Snake::setDirection(int d){
	direction = d;
}

int Snake::getSnameX( int x )
{
	return snakeX[x];
}

int Snake::getSnameY( int y )
{
	return snakeY[y];
}

int Snake::getLength()
{
	return snakeLength;
}

int Snake::getHead()
{
	return snakeHead;
}

int Snake::getTail()
{
	return snakeTail;
}

int Snake::getCounter()
{
	return counter;
}

int Snake::autoMove( int x, int y )
{
	//������ƶ��ķ���
	int directionArray[3] = {0};
	if(direction == 1){
		directionArray[0] = 1;
		directionArray[1] = 2;
		directionArray[2] = 4;
	}
	else if(direction == 2){
		directionArray[0] = 2;
		directionArray[1] = 3;
		directionArray[2] = 1;
	}
	else if(direction == 3){
		directionArray[0] = 3;
		directionArray[1] = 4;
		directionArray[2] = 2;
	}
	else{
		directionArray[0] = 4;
		directionArray[1] = 1;
		directionArray[2] = 3;
	}
	//������һ������ʳ�����Сֵ
	int minLength = 100;
	for(int i = 0; i < 3; ++i){
		int dx = 0, dy = 0;
		if(directionArray[i] == 1)
			dx = 1;
		else if(directionArray[i] == 2)
			dy = -1;
		else if(directionArray[i] == 3)
			dx = -1;
		else
			dy = 1;
		int nextLocX = snakeX[snakeHead] + dx;
		int nextLocY = snakeY[snakeHead] + dy;
		if(World::map[15 - nextLocY][nextLocX] == BLANK_CELL || World::map[15 - nextLocY][nextLocX] == SNAKE_FOOD){
			int l = abs(x - nextLocX) + abs(y - nextLocY);
			if(l < minLength){
				minLength = l;
				direction = directionArray[i];
			}
			CCLOG("l: %d", l);
		}
		//CCLOG("X: %d, Y: %d, next: %c", snakeX[snakeHead], snakeY[snakeHead], World::map[nextLocY][nextLocX]);
	}
	int dx = 0,dy = 0;
	if(direction == 1){
		dx = 1;
		dy = 0;
	}
	else if(direction == 2){
		dy = -1;
		dx = 0;
	}
	else if(direction == 3){
		dx = -1;
		dy = 0;
	}
	else{
		dy = 1;
		dx = 0;
	}
	CCLOG("minLength: %d", minLength);
	CCLOG("direction: %d", direction);
	return move(dy, dx);
}
