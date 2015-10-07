#ifndef __BACKGROUND_LAYER_H__
#define __BACKGROUND_LAYER_H__

#include "cocos2d.h"

class BackgroundLayer : public cocos2d::Layer {
public:
	BackgroundLayer() = default;

    virtual bool init();
    
    CREATE_FUNC(BackgroundLayer);
};

#endif // __BACKGROUND_LAYER_H__
