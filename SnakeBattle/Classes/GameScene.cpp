#include "GameScene.h"
#include "World.h"
#include <list>
#include "SimpleAudioEngine.h"
#include "EndScene.h"
#include <sstream>
#include <iostream>
#include <string>

using namespace cocos2d;

World *world;
Snake *earthSnake, *marsSnake;
CCSprite* food;
std::list<CCSprite*> earthSnakeList, marsSnakeList;

enum GDirection        //当前手势方向;
{
	kGDirectionUp = 0,
	kGDirectionDown,
	kGDirectionLeft,
	kGDirectionRight,
	kGDirectionNo
};
bool b_click;                   //判断当前是否是单击;
bool b_debug;                   //调试用;
bool b_circle;                  //其实这个圆形的判断不是很精确;
bool cache_directionshape[4];   //方向缓存，move中用它来判断是否是单向手势
GDirection gd_direction;        //手势方向;
CCPoint ccp_last,ccp_now;       //记录起始、当前坐标
CCLabelTTF* pLabel;

CCScene* GameScene::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        GameScene *layer = GameScene::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	world = new World();
	earthSnake = new Snake(1);
	marsSnake = new Snake(2);
	earthSnakeList.clear();
	marsSnakeList.clear();
    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////

        CC_BREAK_IF(! CCLayer::init());

        //////////////////////////////////////////////////////////////////////////
        // add your codes below...
        //////////////////////////////////////////////////////////////////////////

        // 1. Add a menu item with "X" image, which is clicked to quit the program.

        // Create a "close" menu item with close icon, it's an auto release object.
		CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
			"CloseNormal.png",
			"CloseSelected.png",
			this,
            menu_selector(GameScene::menuCloseCallback));
        CC_BREAK_IF(! pCloseItem);

        // Place the menu item bottom-right conner.
        pCloseItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20));

        // Create a menu with the "close" menu item, it's an auto release object.
		CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
        pMenu->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenu);

        // Add the menu to HelloWorld layer as a child layer.
        this->addChild(pMenu, 1);

        // 2. Add a label shows "Hello World".

        // Create a label and initialize with string "Hello World".
        pLabel = CCLabelTTF::create("Your Score: 0" , "Arial", 24);
        CC_BREAK_IF(! pLabel);
        // Get window size and place the label upper. 
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        pLabel->setPosition(ccp(size.width / 2, size.height - 50));

        // Add the label to HelloWorld layer as a child layer.
        this->addChild(pLabel, 1);

		//绘制背景
		for(int i = 0; i < 24; ++i)
			for(int j = 0; j < 16; ++j){
				CCSprite* pSprite;
				if(i == 0 || j == 0 || i == 23 || j == 15)
					pSprite = CCSprite::create("wall.png");
				else
					pSprite = CCSprite::create("path.png");
				CC_BREAK_IF(! pSprite);
				pSprite->setPosition(ccp(20*i, 20*j));
				this->addChild(pSprite);
			}

		//初始化地球蛇
		redrawSnake(*earthSnake, 1);
		
		//初始化火星蛇
		redrawSnake(*marsSnake, 2);
		
		//放食物
		world->putFood();
		food = CCSprite::create("food.png");
		food->setPosition(ccp(20*world->getFoodX(),20*world->getFoodY()));
		this->addChild(food, 0);

        bRet = true;
    } while (0);
	this->schedule(schedule_selector(GameScene::step),0.3f);
	//背景音乐
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("backgroundMusic.wav", true);
	this->setTouchEnabled(true);
    return bRet;
}

void GameScene::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}

void GameScene::step(float dt){
	int result1, result2;	
	switch(earthSnake->getDirection()){
		case 1:
			result1 = earthSnake->move(0,1);
			break;
		case 2:
			result1 = earthSnake->move(-1,0);
			break;
		case 3:
			result1 = earthSnake->move(0,-1);
			break;
		case 4:
			result1 = earthSnake->move(1,0);
			break;
		default:
			break;
	}
	
	result2 = marsSnake->autoMove(world->getFoodX(), world->getFoodY());
	//重新放置食物
	if(result1 == 1 || result2 == 1){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("effect.wav");
		world->putFood();
		this->removeChild(food);
		food = CCSprite::create("food.png");
		food->setPosition(ccp(20*world->getFoodX(),20*world->getFoodY()));
		this->addChild(food);
		
		char score[20] = "Your Score: ";
		int v = earthSnake->getLength() - marsSnake->getLength();
		sprintf(score, "%s%d", score, v);

		pLabel->setString(score);
	}
	//游戏结束
	else if(result1 == -1 || result2 == -1){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("gameover.wav");
		CCScene *endScene = EndScene::scene();
		CCDirector::sharedDirector()->replaceScene(endScene);
	}
	//清除旧蛇
	std::list<CCSprite*>::iterator it; 
	for(it=earthSnakeList.begin() ; it!=earthSnakeList.end(); ++it){
		this->removeChild(*it);
	}
	earthSnakeList.clear();
	for(it=marsSnakeList.begin() ; it!=marsSnakeList.end(); ++it){
		this->removeChild(*it);
	}
	marsSnakeList.clear();
	//更新蛇的坐标
	redrawSnake(*earthSnake, 1);
	redrawSnake(*marsSnake, 2);
}


//重绘蛇
void GameScene::redrawSnake(Snake snake, int type){
	CCSprite* pSprite;
	//更新蛇的坐标
	for(int i=0 ; i<snake.getLength() ; ++i){
		if(i == 0){    //蛇尾
			if(snake.getSnameX((1 + snake.getCounter()) % MAX_LENGTH) == snake.getSnameX((0 + snake.getCounter()) % MAX_LENGTH)){
				if(snake.getSnameY((1 + snake.getCounter()) % MAX_LENGTH) - snake.getSnameY((0 + snake.getCounter()) % MAX_LENGTH) == 1){
					pSprite = CCSprite::create("tail-up.jpg");
				}
				else{
					pSprite = CCSprite::create("tail-down.jpg");
				}
			}
			else{
				if(snake.getSnameX((1 + snake.getCounter()) % MAX_LENGTH) - snake.getSnameX((0 + snake.getCounter()) % MAX_LENGTH) == 1){
					pSprite = CCSprite::create("tail-right.jpg");
				}
				else{
					pSprite = CCSprite::create("tail-left.jpg");
				}
			}
		}
		//蛇头
		else if(i == snake.getLength() - 1){
			switch(snake.getDirection()){
			case 1:
				if(type == 1)
					pSprite = CCSprite::create("head-left_1.jpg");
				else
					pSprite = CCSprite::create("head-left.jpg");
				break;
			case 2:
				if(type == 1)
					pSprite = CCSprite::create("head-down_1.jpg");
				else
					pSprite = CCSprite::create("head-down.jpg");
				break;
			case 3:
				if(type == 1)
					pSprite = CCSprite::create("head-right_1.jpg");
				else
					pSprite = CCSprite::create("head-right.jpg");
				break;
			case 4:
				if(type == 1)
					pSprite = CCSprite::create("head-up_1.jpg");
				else
					pSprite = CCSprite::create("head-up.jpg");
				break;
			default:
				break;
			}
		}
		//蛇身
		else{
			if(snake.getSnameX((i - 1 + snake.getCounter()) % MAX_LENGTH) == snake.getSnameX((i + 1 + snake.getCounter()) % MAX_LENGTH)){
				pSprite = CCSprite::create("body-f.jpg");
			}
			else if(snake.getSnameY((i - 1 + snake.getCounter()) % MAX_LENGTH) == snake.getSnameY((i + 1 + snake.getCounter()) % MAX_LENGTH)){
				pSprite = CCSprite::create("body-e.jpg");
			}
			else if(snake.getSnameX((i + snake.getCounter()) % MAX_LENGTH) - snake.getSnameX((i - 1 + snake.getCounter()) % MAX_LENGTH) + snake.getSnameX((i + snake.getCounter()) % MAX_LENGTH) - snake.getSnameX((i + 1 + snake.getCounter()) % MAX_LENGTH)
				+ snake.getSnameY((i + snake.getCounter()) % MAX_LENGTH) - snake.getSnameY((i - 1 + snake.getCounter()) % MAX_LENGTH) + snake.getSnameY((i + snake.getCounter()) % MAX_LENGTH) - snake.getSnameY((i + 1 + snake.getCounter()) % MAX_LENGTH) == 2){
				pSprite = CCSprite::create("body-d.jpg");
			}
			else if(snake.getSnameX((i + snake.getCounter()) % MAX_LENGTH) - snake.getSnameX((i - 1 + snake.getCounter()) % MAX_LENGTH) + snake.getSnameX((i + snake.getCounter()) % MAX_LENGTH) - snake.getSnameX((i + 1 + snake.getCounter()) % MAX_LENGTH)
				+ snake.getSnameY((i + snake.getCounter()) % MAX_LENGTH) - snake.getSnameY((i - 1 + snake.getCounter()) % MAX_LENGTH) + snake.getSnameY((i + snake.getCounter()) % MAX_LENGTH) - snake.getSnameY((i + 1 + snake.getCounter()) % MAX_LENGTH) == -2){
				pSprite = CCSprite::create("body-b.jpg");
			}
			else if(snake.getSnameY((i + snake.getCounter()) % MAX_LENGTH) - snake.getSnameY((i - 1 + snake.getCounter()) % MAX_LENGTH) + snake.getSnameY((i + snake.getCounter()) % MAX_LENGTH) - snake.getSnameY((i + 1 + snake.getCounter()) % MAX_LENGTH) == 1){
				pSprite = CCSprite::create("body-a.jpg");
			}
			else{
				pSprite = CCSprite::create("body-c.jpg");
			}
		}
		CC_BREAK_IF(! pSprite);

		// Place the sprite on the center of the screen
		pSprite->setPosition(ccp(snake.getSnameX((i + snake.getCounter()) % MAX_LENGTH)*20, snake.getSnameY((i + snake.getCounter()) % MAX_LENGTH)*20));

		// Add the sprite to HelloWorld layer as a child layer.
		this->addChild(pSprite, 0);
		if(type == 1)
			earthSnakeList.push_back(pSprite);
		else if(type == 2)
			marsSnakeList.push_back(pSprite);
	}
}

void GameScene::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority + 1, true);
}

bool GameScene::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	ccp_last = pTouch->getLocationInView();
	ccp_last = CCDirector::sharedDirector()->convertToGL(ccp_last);

	b_click = true;
	b_circle = false;
	gd_direction = kGDirectionNo;
	for (int i = 0 ; i < 4 ; i++)
	{
		cache_directionshape[i] = false;
	}
	return true;
}

void GameScene::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
	b_click = false;
	ccp_now = pTouch->getLocationInView();
	ccp_now = CCDirector::sharedDirector()->convertToGL(ccp_now);
	float adsx = ccp_now.x - ccp_last.x;
	float adsy = ccp_now.y - ccp_last.y;
	if(abs(adsx) > abs(adsy))   //X方向增量大
	{
		if(adsx < 0){
			gd_direction = kGDirectionLeft;
		}
		else{
			gd_direction = kGDirectionRight;
		}
	}
	else
	{
		if(adsy < 0){
			gd_direction = kGDirectionDown;
		}
		else{
			gd_direction = kGDirectionUp;
		}
	}
}

void GameScene::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	if(gd_direction == kGDirectionUp && earthSnake->getDirection() != 2){
		earthSnake->setDirection(4);
	}
	else if(gd_direction == kGDirectionDown && earthSnake->getDirection() != 4){
		earthSnake->setDirection(2);
	}
	else if(gd_direction == kGDirectionLeft && earthSnake->getDirection() != 1){
		earthSnake->setDirection(3);
	}
	else if(gd_direction == kGDirectionRight && earthSnake->getDirection() != 3){
		earthSnake->setDirection(1);
	}
}

void GameScene::ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
{

}
