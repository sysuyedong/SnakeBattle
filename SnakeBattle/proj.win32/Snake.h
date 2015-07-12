#define MAX_LENGTH 40

class Snake {
public:
	Snake(int type);
	int move(int dy, int dx);
	bool isAlive();
	void eat(int x,int y);
	int getDirection();
	void setDirection(int d);
	int getSnameX(int x);
	int getSnameY(int y);
	int getLength();
	int getHead();
	int getTail();
	int getCounter();
	int autoMove(int x, int y);

private:
	bool isLive;
	int snakeX[MAX_LENGTH];
	int snakeY[MAX_LENGTH];
	int direction;
	int snakeLength;
	int snakeHead;
	int snakeTail;
	int counter;
};