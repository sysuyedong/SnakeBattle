#define SNAKE_HEAD 'H'
#define SNAKE_BODY 'X'
#define BLANK_CELL ' '
#define SNAKE_FOOD '$'
#define WALL_CELL '*'

class World {
public:
	static char map[16][24];
	World();
	void initialize();
	void putFood();
	int getFoodX();
	int getFoodY();
private:
	int height, width;
	int foodX, foodY;
};