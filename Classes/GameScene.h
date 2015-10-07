#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

class GameScene : public cocos2d::Layer {
public:
    static cocos2d::Scene *createScene();

    virtual bool init();
    virtual void onEnterTransitionDidFinish();
    
    CREATE_FUNC(GameScene);

    bool onContactBegin(cocos2d::PhysicsContact &contact);
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);

    void update(float dt);

private:
    cocos2d::Sprite *bird;

    cocos2d::Sprite *topPipe[2];
    cocos2d::Sprite *bottomPipe[2];
    cocos2d::Sprite *point[2];

    cocos2d::Label *scoreLabel;

    int score;

    float RandomTopPipeY();
};

#endif // __GAME_SCENE_H__
