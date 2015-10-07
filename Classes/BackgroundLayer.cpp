#include "BackgroundLayer.h"

USING_NS_CC;

bool BackgroundLayer::init() {
    if (!Layer::init()) {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    Sprite *background = Sprite::create("res/background.png");
    background->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    this->addChild(background);

    return true;
}
