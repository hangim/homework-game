#include "GameScene.h"
#include "BackgroundLayer.h"
#include "Config.h"
#include "GameOverScene.h"

USING_NS_CC;

Scene* GameScene::createScene() {
    auto scene = Scene::createWithPhysics();
//    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    auto layer = GameScene::create();
    scene->addChild(layer);

    return scene;
}

bool GameScene::init() {
    if (!Layer::init()) {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    this->addChild(BackgroundLayer::create());


    PhysicsBody *edgeBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT);
    edgeBody->setCategoryBitmask(EDGE_CATEGORY_BITMASK);
    edgeBody->setContactTestBitmask(BIRD_CATEGORY_BITMASK);
    edgeBody->setCollisionBitmask(BIRD_CATEGORY_BITMASK);

    Node *edgeNode = Node::create();
    edgeNode->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    edgeNode->setPhysicsBody(edgeBody);
    this->addChild(edgeNode);


    bird = Sprite::create("res/bird.png");
    PhysicsBody *body = PhysicsBody::createCircle(bird->getContentSize().width / 2);
    body->setCategoryBitmask(BIRD_CATEGORY_BITMASK);
    body->setContactTestBitmask(PIPE_CATEGORY_BITMASK | EDGE_CATEGORY_BITMASK | POINT_CATEGORY_BITMASK);
    body->setCollisionBitmask(PIPE_CATEGORY_BITMASK | EDGE_CATEGORY_BITMASK | POINT_CATEGORY_BITMASK);
    body->setGravityEnable(false);
    bird->setPhysicsBody(body);
    bird->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    this->addChild(bird);


    for (int i = 0; i < 2; ++i) {
    	topPipe[i] = Sprite::create("res/pipe.png");
    	topPipe[i]->setPosition(origin.x + visibleSize.width * (0.5 * i + 1), RandomTopPipeY());
    	PhysicsBody *topBody = PhysicsBody::createBox(topPipe[0]->getContentSize());
    	topBody->setCategoryBitmask(PIPE_CATEGORY_BITMASK);
    	topBody->setContactTestBitmask(BIRD_CATEGORY_BITMASK);
    	topBody->setCollisionBitmask(0x00);

    	topBody->setGravityEnable(false);
    	topPipe[i]->setPhysicsBody(topBody);
    	this->addChild(topPipe[i]);

    	bottomPipe[i] = Sprite::create("res/pipe.png");
    	bottomPipe[i]->setPosition(origin.x + visibleSize.width * (0.5 * i + 1),
    			topPipe[i]->getPositionY() - topPipe[i]->getContentSize().height - bird->getContentSize().height * PIPE_GAP_DISTANCE);
    	PhysicsBody *bottomBody = PhysicsBody::createBox(topPipe[0]->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
    	bottomBody->setCategoryBitmask(PIPE_CATEGORY_BITMASK);
    	bottomBody->setContactTestBitmask(BIRD_CATEGORY_BITMASK);
    	bottomBody->setCollisionBitmask(0x00);

    	bottomBody->setGravityEnable(false);
    	bottomPipe[i]->setPhysicsBody(bottomBody);
    	this->addChild(bottomPipe[i]);

    	point[i] = Sprite::create();
    	auto pointBody = PhysicsBody::createBox(Size(1, bird->getContentSize().height * PIPE_GAP_DISTANCE));
    	pointBody->setCategoryBitmask(POINT_CATEGORY_BITMASK);
    	pointBody->setCollisionBitmask(0x00);
    	pointBody->setContactTestBitmask(BIRD_CATEGORY_BITMASK);
    	pointBody->setGravityEnable(false);
    	point[i]->setPhysicsBody(pointBody);
    	point[i]->setPosition(topPipe[i]->getPositionX(),
    			topPipe[i]->getPositionY() - topPipe[i]->getContentSize().height / 2 - bird->getContentSize().height * PIPE_GAP_DISTANCE / 2);
    	this->addChild(point[i]);
    }


    auto coutactListener = EventListenerPhysicsContact::create();
    coutactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(coutactListener, this);

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);


    score = 0;
    auto tmpScoreString = String::createWithFormat("%d", score);
    scoreLabel = Label::createWithTTF(tmpScoreString->getCString(), "fonts/arial.ttf", visibleSize.height * SCORE_FONT_SIZE);
    scoreLabel->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.9);
    scoreLabel->setColor(Color3B::YELLOW);
    this->addChild(scoreLabel);

    return true;
}

void GameScene::onEnterTransitionDidFinish() {
    bird->getPhysicsBody()->setGravityEnable(true);
	this->scheduleUpdate();
}

bool GameScene::onContactBegin(PhysicsContact &contact) {
	auto a = contact.getShapeA();
	auto b = contact.getShapeB();
	CCLOG("contact %d %d", a->getCategoryBitmask(), b->getCategoryBitmask());

	auto mask = a->getCategoryBitmask() | b->getCategoryBitmask();
	if (mask == (BIRD_CATEGORY_BITMASK | POINT_CATEGORY_BITMASK)) {
		++score;
	    auto tmpScoreString = String::createWithFormat("%d", score);
	    scoreLabel->setString(tmpScoreString->getCString());
	} else {
		// game over
		this->unscheduleUpdate();
		auto scene = GameOverScene::createScene(score);
		Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
	}
	return true;
}

bool GameScene::onTouchBegan(Touch *touch, Event *event) {
	CCLOG("GameScene::onTouchBegan");
	bird->getPhysicsBody()->setVelocity(Vect(0, BIRD_VELOCITY));
	return true;
}

void GameScene::update(float dt) {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    for (int i = 0; i < 2; ++i) {
    	float x = topPipe[i]->getPositionX();
    	x -= PIPE_VELOCITY * visibleSize.width;

    	if (x < origin.x) {
    		x = origin.x + visibleSize.width;
    		topPipe[i]->setPositionY(RandomTopPipeY());
    		bottomPipe[i]->setPositionY(topPipe[i]->getPositionY() - bird->getContentSize().height * PIPE_GAP_DISTANCE - topPipe[i]->getContentSize().height);
    		point[i]->setPositionY(topPipe[i]->getPositionY() - topPipe[i]->getContentSize().height / 2 - bird->getContentSize().height * PIPE_GAP_DISTANCE / 2);
    	}

    	topPipe[i]->setPositionX(x);
    	bottomPipe[i]->setPositionX(x);
    	point[i]->setPositionX(x);

    	if (x > origin.x + visibleSize.width / 2) {
    		topPipe[i]->setTexture("res/pipe.png");
    		bottomPipe[i]->setTexture("res/pipe.png");
    	} else {
    		topPipe[i]->setTexture("res/pipe_passed.png");
    		bottomPipe[i]->setTexture("res/pipe_passed.png");
    	}
    }

    float velocity = bird->getPhysicsBody()->getVelocity().y;
    bird->setRotation(-velocity / 4);
}

float GameScene::RandomTopPipeY() {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	return (rand_0_1() * (16 - 2 - PIPE_GAP_DISTANCE) / 16.0 + (1 + PIPE_GAP_DISTANCE) / 16.0) * visibleSize.height
			+ topPipe[0]->getContentSize().height / 2 + origin.y;
}
