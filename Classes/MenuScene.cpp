#include "PlayScene.h"
#include "2d/CCSprite.h"
#include "2d/CCMenuItem.h"
#include "2d/CCMenu.h"
#include "base/CCDirector.h"
#include "MenuScene.h"
#include "2d/CCTransition.h"
#include "2d/CCActionInterval.h"
#include "BridgeClass.h"
#include "cocos2d.h"
#include "CommonUtils.h"

USING_NS_CC;

Scene* MenuScene::createScene() {
    return MenuScene::create();
}

bool MenuScene::init() {
    if (!Scene::init()) {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    Sprite *bgSprite = Sprite::create("background.jpg");
    bgSprite->setPosition(
            Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(bgSprite);

     titleImage = Sprite::create("title_image.png");
            titleImage->setPosition(visibleSize.width*0.25, visibleSize.height/2 + origin.y);
    addChild(titleImage);
    titleImage->setScale(0.25);
    titleImage->runAction(RepeatForever::create(
            Sequence::create(
                    ScaleTo::create(0.15, 0.23),
                    ScaleTo::create(0.15, 0.25),
                    ScaleTo::create(0.15, 0.23),
                    ScaleTo::create(0.15, 0.25),
                    DelayTime::create(3),
                    NULL
                    )
            ));


    MenuItemImage *playButton = MenuItemImage::create("play.png", "play.png",
                                                      CC_CALLBACK_1(MenuScene::onPlayButtonClicked,
                                                                    this));
    playButton->setPosition(
            Vec2(visibleSize.width*0.75+ origin.x, visibleSize.height / 2 + origin.y - 50));

playButton->runAction(RepeatForever::create(Sequence::create(MoveBy::create(2, Vec2(0,10)),
                                                             MoveBy::create(0.4, Vec2(0,-10)),
                                                             NULL)));

    MenuItemImage *settingsButton = MenuItemImage::create("setting.png", "setting.png",
                                                          CC_CALLBACK_1(
                                                                  MenuScene::onSettingsButtonClicked,
                                                                  this));
    settingsButton->setPosition(Vec2(visibleSize.width + origin.x-25,
                                     visibleSize.height + origin.y - 25));
    settingsButton->setScale(0.5);

    settingsButton->runAction(RepeatForever::create(Sequence::create(
            RotateBy::create(1, 360),
            DelayTime::create(3),
                                                                     NULL)));



    Menu *menu = Menu::create(playButton, settingsButton, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);

    return true;
}

void MenuScene::onPlayButtonClicked(Ref* sender) {

    CommonUtils::playButtonSound();
Director::getInstance()->replaceScene(TransitionFade::create(0.25,PlayScene::createScene()));
}

void MenuScene::onSettingsButtonClicked(Ref* sender) {

    CommonUtils::playButtonSound();
    BridgeClass::showToast("settings not implemented");
}