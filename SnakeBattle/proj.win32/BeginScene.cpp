#include "BeginScene.h"
#include "GameScene.h"
#include "InfoScene.h"

using namespace cocos2d;

CCScene* BeginScene::scene()
{
	CCScene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		BeginScene *layer = BeginScene::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}

bool BeginScene::init()
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
			menu_selector(BeginScene::menuCloseCallback));
		CC_BREAK_IF(! pCloseItem);
		pCloseItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20));

		CCMenuItemImage *pStartItem = CCMenuItemImage::create(
			"startGame.png",
			"startGame.png",
			this,
			menu_selector(BeginScene::menuStartCallback));
		pStartItem->setPosition(ccp(100, 280));

		CCMenuItemImage *pInfoItem = CCMenuItemImage::create(
			"gameInfo.png",
			"gameInfo.png",
			this,
			menu_selector(BeginScene::menuInfoCallback));
		pInfoItem->setPosition(ccp(380,280));

		CCMenu* pMenu = CCMenu::create(pCloseItem, pStartItem, pInfoItem, NULL);
		pMenu->setPosition(CCPointZero);
		CC_BREAK_IF(! pMenu);

		// Add the menu to HelloWorld layer as a child layer.
		this->addChild(pMenu, 1);

		CCSprite* pSprite = CCSprite::create("begin.png");
		CC_BREAK_IF(! pSprite);

		// Place the sprite on the center of the screen
		pSprite->setPosition(ccp(size.width/2, size.height/2));

		// Add the sprite to HelloWorld layer as a child layer.
		this->addChild(pSprite, 0);	

		bRet = true;
	}while (0);
	return bRet;
}

void BeginScene::menuCloseCallback(CCObject* pSender)
{
	// "close" menu item clicked
	CCDirector::sharedDirector()->end();
}

void BeginScene::menuStartCallback( CCObject* pSender )
{
	CCScene *pScene = GameScene::scene();
	CCDirector::sharedDirector()->replaceScene(pScene);
}

void BeginScene::menuInfoCallback( CCObject* pSender )
{
	CCScene *pScene = InfoScene::scene();
	CCDirector::sharedDirector()->replaceScene(pScene);
}
