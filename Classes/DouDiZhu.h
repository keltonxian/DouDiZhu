//
//  DouDiZhu.h
//  KillTime
//
//  Created by keltonxian on 8/28/14.
//
//

#ifndef __KillTime__DouDiZhu__
#define __KillTime__DouDiZhu__

#include "KHeader.h"

#include "PlayerDDZ.h"

class DouDiZhu : public Layer {
public:
    static Scene* createScene();
    virtual bool init() override;
    CREATE_FUNC(DouDiZhu);
private:
    int _state;
    int currentSide;
    int grabSide;
    int nextState();
    int randomFirstHand();
    int firstHand;
    int winSide;
    void initTable();
    void cleanTable();
    void restart();
    PlayerDDZ* nextPlayer();
    void removeTalk(int side);
    void removeAllTalk();
    void removeShowHost();
    void callbackRemove(Node* pSender);
    void showTip(const char *tip);
    void showTalk(PlayerDDZ *player, const char *talk);
    void hidePlayedPoker(Vector<PokerDDZ *> playedList);
    void updatePlayList(PokerDDZ *poker, bool isAdd);
    void aiPlay(PlayerDDZ *player);
    void logic();
    void callbackLogic(Node* pSender);
    bool checkGameOver();
    void showResult(float delta);
    void callbackResult(Ref* pSender);
    bool isCanTouch;
    void callbackEnableTouch(float delta);
    int roundCount;
    int hostSide;
    Vector<PokerDDZ *> pack;
    Vector<PokerDDZ *> hostPokerShow;
    Vector<PokerDDZ *> playList;
    Vector<PokerDDZ *> sideDownPlayedList;
    Vector<PokerDDZ *> sideLeftPlayedList;
    Vector<PokerDDZ *> sideRightPlayedList;
    Vector<PlayerDDZ *> playerList;
    int lastCardType;
    int lastNum;
    int lastPlayListSize;
    int lastPlaySide;
    PokerDDZ *lastTouchPoker;
    Sprite *coverLeft;
    Sprite *coverRight;
    Label *coinLeft;
    Label *coinRight;
    Label *coinDown;
    Label *countLeft;
    Label *countRight;
    void updatePlayerCoin();
    void showSideCount(bool isShow);
    void addRoleFlag(int hostSide);
    void removeAllRoleFlag();
    Label *llog;
    
    PlayerDDZ* loadPlayerById(int pid, int side);
    void initPlayer();
    void addPlayer(PlayerDDZ *player);
    PlayerDDZ* getPlayer(int side);
    void setupPoker();
    void showHostPoker(PokerDDZ *poker, float delay, Point position);
    void dealPoker();
    void dealHostPoker(int side);
    float sortPokerSprite(PlayerDDZ *player, float delay, bool callLogic);
    void sortPlaySprite(Vector<PokerDDZ *> list);
    float delayCallLogic(Sprite *sprite, float delay);
    float moveSideAnimation(Sprite *sprite, float delay, bool isLeft);
    void callbackMoveLeftSide(Node* pSender);
    void callbackMoveRightSide(Node* pSender);
    float moveAnimation(Sprite *sprite, Point targetPos, float delay, float time);
    float moveScaleAnimation(Sprite *sprite, Point targetPos, float scaleTo, float delay, float time);
    void callback_move(Node* pSender);
    void saveGame();
    bool loadGame();
    void checkTouchPoker(Touch* touch);
    void updatePlayerPokerCount(PlayerDDZ *player, Label *label);
    void printCardTypeLog();
    int logCount;
    
    void showBtn(int btnType);
    MenuItemSprite *btnReady;
    void callbackReady(Ref* pSender);
    MenuItemSprite *btnCallHost;
    MenuItemSprite *btnNotCallHost;
    void callbackCallHost(Ref* pSender);
    MenuItemSprite *btnGrabHost;
    MenuItemSprite *btnNotGrabHost;
    void callbackGrabHost(Ref* pSender);
    MenuItemSprite *btnDouble2;
    MenuItemSprite *btnDouble4;
    MenuItemSprite *btnNotDouble;
    void callbackDouble(Ref* pSender);
    MenuItemSprite *btnPlay;
    void callbackPlay(Ref* pSender);
    MenuItemSprite *btnPass;
    void callbackPass(Ref* pSender);
    MenuItemSprite *btnTip;
    void callbackTip(Ref* pSender);
    void callbackBack(Ref* pSender);
    
    Sprite *toolbar;
    Label *labelClock;
    void updateClock(float delta);
    Label *totalCoin;
    Label *baseCoin;
    Label *numMultiple;
    void pokerUp(PokerDDZ *poker, bool isSetUp);
    void resetPlayList();
    void setDoubleMultiple();
    
    int sidePlayerX;
    int sidePlayerY;
    DouDiZhu();
    virtual ~DouDiZhu();
    virtual void onEnter() override;
    virtual void onExit() override;
    bool onTouchBegan(Touch* touch, Event* event) override;
    void onTouchMoved(Touch* touch, Event* event) override;
    void onTouchEnded(Touch* touch, Event* event) override;
};

class ResultLayer : public Layer {
public:
    virtual bool init() override;
    CREATE_FUNC(ResultLayer);
    void setData(int coin, int winSide, Vector<PlayerDDZ *> playerList, const ccMenuCallback& callback);
private:
    ccMenuCallback _callback;
    void callbackNext(cocos2d::Ref* pSender);
    ResultLayer();
    virtual ~ResultLayer();
    virtual void onEnter() override;
    virtual void onExit() override;
    bool onTouchBegan(Touch* touch, Event* event) override;
    void onTouchMoved(Touch* touch, Event* event) override;
    void onTouchEnded(Touch* touch, Event* event) override;
};

#endif /* defined(__KillTime__DouDiZhu__) */
