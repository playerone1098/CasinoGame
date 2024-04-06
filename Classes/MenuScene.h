//
// Created by Ashok Choudhary on 05/04/24.
//

#ifndef PROJ_ANDROID_MENUSCENE_H
#define PROJ_ANDROID_MENUSCENE_H


#include "2d/CCScene.h"
USING_NS_CC;

class MenuScene : public cocos2d::Scene
{
public:
static cocos2d::Scene* createScene();
virtual bool init();

CREATE_FUNC(MenuScene);


    Sprite* titleImage;

    void onPlayButtonClicked(Ref* sender);
    void onSettingsButtonClicked(Ref* sender);

};

#endif //PROJ_ANDROID_MENUSCENE_H
