#include "EndScene.h"
#include "GameScene.h"
#include "BeginScene.h"

using namespace cocos2d;

CCScene* EndScene::scene()
{
	CCScene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		EndScene *layer = EndScene::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}

bool EndScene::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(! CCLayer::init());
		CCSize size = CCDirector::sharedDirector()->getWinSize();

		CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
			"CloseNormal.png",
			"CloseSelected.png",
			this,
			menu_selector(EndScene::menuCloseCallback));
		CC_BREAK_IF(! pCloseItem);
		pCloseItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20));

		CCMenuItemImage *pRestartItem = CCMenuItemImage::create(
			"restart.png",
			"restart.png",
			this,
			menu_selector(EndScene::menuRestartCallback));
		pRestartItem->setPosition(ccp(120, 120));
		
		CCMenuItemImage *pReturnItem = CCMenuItemImage::create(
			"return.png",
			"return.png",
			this,
			menu_selector(EndScene::menuReturnCallback));
		pReturnItem->setPosition(ccp(360, 120));

		CCMenu* pMenu = CCMenu::create(pCloseItem, pRestartItem, pReturnItem, NULL);
		pMenu->setPosition(CCPointZero);
		CC_BREAK_IF(! pMenu);

		// Add the menu to HelloWorld layer as a child layer.
		this->addChild(pMenu, 1);

		CCSprite* pSprite = CCSprite::create("endBackground.png");
		CC_BREAK_IF(! pSprite);

		// Place the sprite on the center of the screen
		pSprite->setPosition(ccp(size.width/2, size.height/2));

		// Add the sprite to HelloWorld layer as a child layer.
		this->addChild(pSprite, 0);	

		bRet = true;
	}while (0);
	return bRet;
}

void EndScene::menuCloseCallback(CCObject* pSender)
{
	// "close" menu item clicked
	CCDirector::sharedDirector()->end();
}

void EndScene::menuRestartCallback( CCObject* pSender )
{
	CCScene *gameScene = GameScene::scene();
	CCDirector::sharedDirector()->replaceScene(gameScene);
}

void EndScene::menuReturnCallback( CCObject* pSender )
{
	CCScene *beginScene = BeginScene::scene();
	CCDirector::sharedDirector()->replaceScene(beginScene);
}
