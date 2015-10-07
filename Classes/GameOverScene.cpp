#include "GameOverScene.h"
#include "GameScene.h"
#include "BackgroundLayer.h"
#include "MainMenuScene.h"
#include "Config.h"

USING_NS_CC;

int score;

Scene* GameOverScene::createScene(int _score) {
    score = _score;

	auto scene = Scene::create();
    
    auto layer = GameOverScene::create();
    scene->addChild(layer);

    return scene;
}

bool GameOverScene::init() {
    if (!Layer::init()) {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    this->addChild(BackgroundLayer::create());

    unsigned highestScore = UserDefault::getInstance()->getIntegerForKey("HIGHESTSCORE", 0);
    if (score > highestScore) {
    	highestScore = score;
    	UserDefault::getInstance()->setIntegerForKey("HIGHESTSCORE", highestScore);
    	UserDefault::getInstance()->flush();
    }

    String *tmpHighestScoreString = String::createWithFormat("The highest score is %d", highestScore);
    String *tmpScoreString = String::createWithFormat("Your score is %d", score);
    Label *highestScoreLabel = Label::createWithTTF(tmpHighestScoreString->getCString(), "fonts/arial.ttf", visibleSize.height * SCORE_FONT_SIZE);
    Label *scoreLabel = Label::create(tmpScoreString->getCString(), "fonts/arial.ttf", visibleSize.height * SCORE_FONT_SIZE);
    highestScoreLabel->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.75);
    scoreLabel->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    this->addChild(highestScoreLabel);
    this->addChild(scoreLabel);

    MenuItemImage *retryItem = MenuItemImage::create("res/button_retry.png", "res/button_retry.png", CC_CALLBACK_1(GameOverScene::goToGameScene, this));
    retryItem->setPosition(origin.x + visibleSize.width / 3, origin.y + visibleSize.height / 4);
    MenuItemImage *mainMenuItem = MenuItemImage::create("res/button_menu.png", "res/button_menu.png", CC_CALLBACK_1(GameOverScene::goToMainMenuScene, this));
    mainMenuItem->setPosition(origin.x + visibleSize.width * 2 / 3, origin.y + visibleSize.height / 4);
    Menu *menu = Menu::create(retryItem, mainMenuItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu);

    return true;
}

void GameOverScene::goToGameScene(Ref *sender) {
	auto scene = GameScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}

void GameOverScene::goToMainMenuScene(Ref *sender) {
	auto scene = MainMenuScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}
