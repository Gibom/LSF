﻿#ifndef __LSFMain_SCENE_H__
#define __LSFMain_SCENE_H__

#include "cocos2d.h"
#include "D:\LSF_Project\cocos2d\external\json\rapidjson.h"
#include "D:\LSF_Project\cocos2d\external\json\document.h"
#include "D:\LSF_Project\cocos2d\external\json\stringbuffer.h"
#include "D:\LSF_Project\cocos2d\external\json\writer.h"

using namespace cocos2d;

class LSFMain : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(LSFMain);

	Sprite* btn_Start;

	virtual void onEnter();
	virtual void onExit();
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
	
	void doPushSceneTran(Ref * pSender);
	cocos2d::TransitionScene* createTransition(int nIndex, float t, cocos2d::Scene* s);
};

#endif // __LSFMain_SCENE_H__
