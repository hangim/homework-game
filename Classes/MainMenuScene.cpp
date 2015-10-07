#include "MainMenuScene.h"
#include "BackgroundLayer.h"
#include "GameScene.h"
#include "Config.h"

USING_NS_CC;

Scene *MainMenuScene::createScene() {
	auto scene = Scene::create();
	scene->addChild(MainMenuScene::create());
	return scene;
}

bool MainMenuScene::init() {
    if (!Layer::init()) {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    this->addChild(BackgroundLayer::create());

    MenuItemImage *playItem =  MenuItemImage::create("res/button_play.png", "res/button_play.png", CC_CALLBACK_1(MainMenuScene::goToGameScene, this));
    playItem->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 2 / 3);
    Menu *menu = Menu::create(playItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu);

    return true;
}

void MainMenuScene::goToGameScene(Ref *sender) {
	auto scene = GameScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}
