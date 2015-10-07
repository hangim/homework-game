#ifndef __GAMEOVER_SCENE_H__
#define __GAMEOVER_SCENE_H__

#include "cocos2d.h"

class GameOverScene : public cocos2d::Layer {
public:
    static cocos2d::Scene *createScene(int _score);

    virtual bool init();
    
    CREATE_FUNC(GameOverScene);

private:
    void goToGameScene(cocos2d::Ref *sender);

    void goToMainMenuScene(cocos2d::Ref *sender);
};

#endif // __GAMEOVER_SCENE_H__
