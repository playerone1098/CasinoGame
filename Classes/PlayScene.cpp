#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "PlayScene.h"
#include "MenuScene.h"
#include "CommonUtils.h"

#include <vector>

USING_NS_CC;
using namespace cocos2d::ui;


Scene* PlayScene::createScene() {
    return PlayScene::create();
}

bool PlayScene::init() {
    if (!Scene::init()) {
        return false;
    }

    totalColumns = 3;
    totalRows = 5;
betValue =50;

    canSpin = true;
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    Sprite* bg = Sprite::create("background.jpg");
    addChild(bg);


    slotMachine = Sprite::create("slot_machine.png");
    slotMachine->setPosition(visibleSize/2 + (Size)origin/2);
    slotMachine->setScale(1.5);
    addChild(slotMachine, 10);

    Button* knobButton = Button::create("knob.png", "knob.png");
    knobButton->setPosition(Vec2(slotMachine->getContentSize().width - knobButton->getContentSize().width/2 , slotMachine->getContentSize().height*0.65));
    knobButton->addClickEventListener(CC_CALLBACK_1(PlayScene::knobButtonCallback, this));
    knobButton->setScale(1);
    slotMachine->addChild(knobButton);

    arrow = Sprite::create("arrow.png");
    arrow->setPosition(arrow->getContentSize().width/2, slotMachine->getContentSize().height/2);
    slotMachine->addChild(arrow);

    auto clippingNode = ClippingNode::create();
    addChild(clippingNode);
    clippingNode->setStencil(slotMachine);
    clippingNode->setInverted(false);



    for(int i = 0 ;i<totalColumns;i++) {
        float posY = slotMachine->getPositionY() + slotMachine->getContentSize().height;

        for (int j = 0; j < totalRows; j++) {
            symbols[i][j] = Sprite::create(
                    __String::createWithFormat("slot_%d.png", random(1, 4))->getCString());
            symbols[i][j]->setPosition(
                    (slotMachine->getPositionX() - (slotMachine->getContentSize().width / 2)) +
                    (slotMachine->getContentSize().width) * (i + 1) * 0.025 + (i * 65), posY);

            clippingNode->addChild(symbols[i][j], -1);

            posY = posY - (symbols[i][j]->getContentSize().height);

            if(j==totalRows-1)
            {
                lastSymbol[i] = symbols[i][j];
            }
        }
    }





    Button* backButton = Button::create("back_button.png", "back_button.png");
    backButton->setPosition(Vec2(25, origin.y + visibleSize.height -25));
    backButton->addClickEventListener(CC_CALLBACK_1(PlayScene::backButtonCallback, this));
    backButton->setScale(1);
    this->addChild(backButton);

    createCoinsPanel();
arrangeSymbols();

    return true;
}
void PlayScene::update(float delta)
{
    Rect slotMachineBoundingBox = slotMachine->getBoundingBox();

    for (int i = 0; i < totalColumns; i++) {
        float newY = 10;
        for (int j = 0; j < totalRows; j++) {

            symbols[i][j]->setPosition(symbols[i][j]->getPosition()+Vec2(0, newY));

            if(symbols[i][j]->getPositionY() > slotMachine->getPositionY()+slotMachine->getContentSize().height/2)
            {
                symbols[i][j]->setPositionY(lastSymbol[i]->getPositionY() - symbols[i][j]->getContentSize().height);
                lastSymbol[i] = symbols[i][j];
            }
        }
    }
}


void PlayScene::createCoinsPanel() {

    auto *coinIcon = Sprite::create("coin.png");
    coinIcon->setPosition((Vec2) (visibleSize) + origin + Vec2(-100, (coinIcon->getContentSize().height/2)*(-1)));
    addChild(coinIcon);
    coinIcon->runAction(RepeatForever::create(Sequence::create(
            ScaleTo::create(0.25, -1, 1),
            ScaleTo::create(0.25, 1, 1),
            DelayTime::create(2),NULL)));

    scoreText = Label::create("1000", "Ariel", 10, Size(100, 50), TextHAlignment::LEFT,
                                    TextVAlignment::CENTER);
    scoreText->setPosition(
            coinIcon->getPosition() + Vec2((scoreText->getContentSize().width / 2) + (coinIcon->getContentSize().width/2)+5, 0));
    this->addChild(scoreText);
}
void PlayScene::playClickEffect(Node *node) {
    float duration = 0.15;
    float normalScale = 1;
    float pressedScale = 0.8;
    node->runAction(Sequence::create(ScaleTo::create(duration, pressedScale), ScaleTo::create(duration, normalScale), NULL));

}
void PlayScene::backButtonCallback(cocos2d::Ref *sender) {

    CommonUtils::playButtonSound();
    Button *btn = (Button*)sender;
    playClickEffect(btn);
    Director::getInstance()->replaceScene(TransitionFade::create(0.25, MenuScene::createScene()));
}
void PlayScene::knobButtonCallback(Ref* sender) {
    if (!canSpin) { return; }

    CommonUtils::playButtonSound();
    Button *btn = (Button *) sender;
    playClickEffect(btn);
    canSpin = false;
    scheduleUpdate();
    setScore(-betValue);


    this->runAction(Sequence::create(DelayTime::create(random(1, 4)), CallFunc::create([this]() {
        unscheduleUpdate();
        canSpin = true;

      arrangeSymbols();
       giveReward();

    }), NULL));
}


void PlayScene::arrangeSymbols(){
    Node *node = getNearestSymbol();
    Vec2 arrowPosInSymbolsParent = symbols[0][0]->getParent()->convertToNodeSpace(
            arrow->getParent()->convertToWorldSpace(arrow->getPosition()));

    Vec2 nearestSymbolPosInSymbolsParent = symbols[0][0]->getParent()->convertToNodeSpace(
            node->getParent()->convertToWorldSpace(node->getPosition()));


    float diffY = arrowPosInSymbolsParent.y - nearestSymbolPosInSymbolsParent.y;
    scrollSymbolsToArrow(diffY);
}


Node* PlayScene::getNearestSymbol() {
    float arrowY = arrow->getParent()->convertToWorldSpace(arrow->getPosition()).y;
    Vec2 arrowPosInSymbolSpace = symbols[0][0]->getParent()->convertToNodeSpace(arrow->getParent()->convertToWorldSpace(arrow->getPosition()));
arrowY = arrowPosInSymbolSpace.y;

    float distance = 1000;
    Node* nearestSymbol = nullptr;

    for (int i = 0; i < totalColumns; ++i) {
        Node* symbol = symbols[0][i];
        float symbolY = symbol->getPositionY();
        float deltaY = arrowY - symbolY;

        if (distance > std::abs(deltaY)) {
            distance = std::abs(deltaY);
            nearestSymbol = symbol;
        }
    }

    return nearestSymbol;
}


void PlayScene::scrollSymbolsToArrow(float diffY) {

    for(int i=0; i<totalColumns; i++){
        for (int j = 0; j < totalRows; j++) {
            symbols[i][j]->runAction(MoveBy::create(0.25,  Vec2(0, diffY)));
        }
    }
}
void PlayScene::setScore(int deltaScore) {
    int value = std::stoi(scoreText->getString());
    value += deltaScore;
    scoreText->setString(std::to_string(value));
}

void PlayScene::giveReward() {
    float smallWinProb = 0.7;
    float mediumWinProb = 0.1;

    float randValue = rand_0_1();
    log("%f", randValue);
    if (randValue <= smallWinProb) {
        randValue = random(0,50);
    } else if (randValue <= (smallWinProb + mediumWinProb)) {
        randValue = random(50, 500);
    } else {
        randValue = random(500, 1000);

        showBigWinPopup();
    }
    setScore(randValue);
}

void PlayScene::showBigWinPopup() {

    auto *bigWin = Sprite::create("big_win.png");
    bigWin->setPosition(visibleSize / 2 + (Size) origin);
    this->addChild(bigWin, 100);
    bigWin->setScale(0.35);
    bigWin->setOpacity(0);

    float fadeDuration = 0.4;
    auto *fadeto = FadeTo::create(fadeDuration, 100);

    bigWin->runAction(Sequence::create(
            FadeIn::create(fadeDuration),
            fadeto,
            FadeIn::create(fadeDuration),
            fadeto,
            FadeIn::create(fadeDuration),
            FadeOut::create(fadeDuration),
            RemoveSelf::create(false),
            NULL));
}
