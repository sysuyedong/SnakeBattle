#include "InfoScene.h"
#include "BeginScene.h"

using namespace cocos2d;

CCScene* InfoScene::scene()
{
	CCScene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		InfoScene *layer = InfoScene::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}

bool InfoScene::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(! CCLayer::init());
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		
		CCMenuItemImage *pReturnItem = CCMenuItemImage::create(
			"return.png",
			"return.png",
			this,
			menu_selector(InfoScene::menuReturnCallback));
		pReturnItem->setPosition(ccp(440, 35));

		CCMenu* pMenu = CCMenu::create(pReturnItem, NULL);
		pMenu->setPosition(CCPointZero);
		CC_BREAK_IF(! pMenu);

		// Add the menu to HelloWorld layer as a child layer.
		this->addChild(pMenu, 1);

		CCSprite* pSprite = CCSprite::create("detail.png");
		CC_BREAK_IF(! pSprite);

		// Place the sprite on the center of the screen
		pSprite->setPosition(ccp(size.width/2, size.height/2));

		// Add the sprite to HelloWorld layer as a child layer.
		this->addChild(pSprite, 0);	

		bRet = true;
	}while (0);
	return bRet;
}

void InfoScene::menuCloseCallback(CCObject* pSender)
{
	// "close" menu item clicked
	CCDirector::sharedDirector()->end();
}

void InfoScene::menuReturnCallback( CCObject* pSender )
{
	CCScene *beginScene = BeginScene::scene();
	CCDirector::sharedDirector()->replaceScene(beginScene);
}
