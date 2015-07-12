#ifndef __BEGINSCENE_SCENE_H__
#define __BEGINSCENE_SCENE_H__

#include "cocos2d.h"

class BeginScene : public cocos2d::CCLayer
{
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();

	// a selector callback
	void menuCloseCallback(CCObject* pSender);
	void menuStartCallback(CCObject* pSender);
	void menuInfoCallback(CCObject* pSender);

	// implement the "static node()" method manually
	CREATE_FUNC(BeginScene);
};

#endif  // __BEGINSCENE_SCENE_H__