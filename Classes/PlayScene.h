//
// Created by Ashok Choudhary on 05/04/24.
//

#ifndef PROJ_ANDROID_PlayScene_H
#define PROJ_ANDROID_PlayScene_H


#include "2d/CCScene.h"
#include "ui/CocosGUI.h"
#include <vector>

USING_NS_CC;

class PlayScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    void knobButtonCallback(Ref* sender);
    void playClickEffect(Node* node);
    CREATE_FUNC(PlayScene);

private:
    int totalColumns;
    int totalRows;
    int betValue;
    bool canSpin;
    Size visibleSize;
    Vec2 origin;
    Node* lastSymbol[3];
    Sprite* slotMachine, *arrow;
    Sprite* symbols[3][10];
    Label *scoreText;

    void update(float delta);
    void backButtonCallback(Ref* sender);
    void createCoinsPanel();
    Node* getNearestSymbol();
    void setScore(int deltaScore);
    void scrollSymbolsToArrow(float diffY);
    void arrangeSymbols();

    void giveReward();

    void showBigWinPopup();
};

#endif //PROJ_ANDROID_PlayScene_H
