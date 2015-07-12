#include "World.h"
#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

char World::map[16][24];

World::World()
{
	for(int i=0; i<16; ++i)
		for(int j=0; j<24; ++j){
			if(i == 0 || j == 0 || i == 15 || j == 23)
				World::map[i][j] = WALL_CELL;
			else
				World::map[i][j] = BLANK_CELL;
		}
	World::map[14][1] = SNAKE_BODY;
	World::map[14][2] = SNAKE_BODY;
	World::map[14][3] = SNAKE_BODY;
	World::map[14][4] = SNAKE_BODY;
	World::map[14][5] = SNAKE_HEAD;

	World::map[5][1] = SNAKE_BODY;
	World::map[5][2] = SNAKE_BODY;
	World::map[5][3] = SNAKE_BODY;
	World::map[5][4] = SNAKE_BODY;
	World::map[5][5] = SNAKE_HEAD;
}

void World::initialize()
{

}

void World::putFood()
{
	srand((int)time(NULL));   //初始化随机数 
	do {                 //生成随机坐标，当随机坐标位置为空时，成功
		foodX = rand()%24 + 1;                   
		foodY = rand()%16 + 1;        
	} while (map[15 - foodY][foodX] != BLANK_CELL);
	map[15 - foodY][foodX] = SNAKE_FOOD;
	return;
}

int World::getFoodX()
{
	return foodX;
}

int World::getFoodY()
{
	return foodY;
}
