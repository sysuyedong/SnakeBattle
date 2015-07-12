#ifndef __GAMESCENE_SCENE_H__
#define __GAMESCENE_SCENE_H__

#include "cocos2d.h"
#include "Snake.h"

using namespace cocos2d;

class GameScene : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(GameScene);

	void step(float dt);
	void redrawSnake(Snake snake, int type);

	virtual void registerWithTouchDispatcher();

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);// ±ÿ–Î µœ÷
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
};

#endif  // __GAMESCENE_SCENE_H__