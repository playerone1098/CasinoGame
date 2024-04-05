#include "MenuScene.h"
#include "2d/CCSprite.h"
#include "2d/CCMenuItem.h"
#include "2d/CCMenu.h"
#include "base/CCDirector.h"


USING_NS_CC;

Scene* MenuScene::createScene() {
    return MenuScene::create();
}

bool MenuScene::init() {
    if (!Scene::init()) {
        return false;
    }

    // Get screen size
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Create background sprite
    Sprite *bgSprite = Sprite::create("background.jpg");
    bgSprite->setPosition(
            Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(bgSprite);

    MenuItemImage *playButton = MenuItemImage::create("play.png", "play.png",
                                                      CC_CALLBACK_1(MenuScene::onPlayButtonClicked,
                                                                    this));
    playButton->setPosition(
            Vec2(visibleSize.width*0.75+ origin.x, visibleSize.height / 2 + origin.y));

    MenuItemImage *settingsButton = MenuItemImage::create("setting.png", "setting.png",
                                                          CC_CALLBACK_1(
                                                                  MenuScene::onSettingsButtonClicked,
                                                                  this));
    settingsButton->setPosition(Vec2(visibleSize.width + origin.x-25,
                                     visibleSize.height + origin.y - 25));
    settingsButton->setScale(0.5);

    Menu *menu = Menu::create(playButton, settingsButton, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);

    return true;
}

void MenuScene::onPlayButtonClicked(Ref* sender) {
    CCLOG("Play button clicked");

}

void MenuScene::onSettingsButtonClicked(Ref* sender) {
    CCLOG("Settings button clicked");
}
