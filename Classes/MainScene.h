//
//  MainScene.h
//  KillTime
//
//  Created by keltonxian on 7/11/14.
//
//

#ifndef __KillTime__MainScene__
#define __KillTime__MainScene__

#include "KHeader.h"

class MainScene : public Layer {
public:
    static Scene* createScene();
    virtual bool init() override;
    CREATE_FUNC(MainScene);
private:
    void addPlayer(const char *altas, int iconIndex, int objIndex);
    void initPlayerData();
    void initMyData();
    virtual void onEnter() override;
    virtual void onExit() override;
    bool onTouchBegan(Touch* touch, Event* event) override;
    void onTouchMoved(Touch* touch, Event* event) override;
    void onTouchEnded(Touch* touch, Event* event) override;
    virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) override;
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) override;
    
    void callbackRandomMatch(cocos2d::Ref* pSender);
    void callbackRank(cocos2d::Ref* pSender);
};

#endif /* defined(__KillTime__MainScene__) */
