//
// Created by Ashok Choudhary on 07/04/24.
//

#include "CommonUtils.h"
#include "SimpleAudioEngine.h"

void CommonUtils::playButtonSound() {

    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("button_touch.mp3");
}
