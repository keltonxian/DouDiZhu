//
//  PokerDDZ.h
//  KillTime
//
//  Created by keltonxian on 8/28/14.
//
//

#ifndef __KillTime__PokerDDZ__
#define __KillTime__PokerDDZ__

#include "KHeader.h"

#define PD_ZORDER_NORMAL 100

#define STYLE_COVER  0
#define STYLE_FRONT  1
#define STYLE_SMALL_COVER  2
#define STYLE_SMALL_FRONT  3

typedef enum {
    DDZ_SPADE_3=0,DDZ_SPADE_4,DDZ_SPADE_5,DDZ_SPADE_6,DDZ_SPADE_7,DDZ_SPADE_8,DDZ_SPADE_9,DDZ_SPADE_10,DDZ_SPADE_J,DDZ_SPADE_Q,DDZ_SPADE_K,DDZ_SPADE_A,DDZ_SPADE_2,
	DDZ_HEART_3,DDZ_HEART_4,DDZ_HEART_5,DDZ_HEART_6,DDZ_HEART_7,DDZ_HEART_8,DDZ_HEART_9,DDZ_HEART_10,DDZ_HEART_J,DDZ_HEART_Q,DDZ_HEART_K,DDZ_HEART_A,DDZ_HEART_2,
	DDZ_CLUB_3,DDZ_CLUB_4,DDZ_CLUB_5,DDZ_CLUB_6,DDZ_CLUB_7,DDZ_CLUB_8,DDZ_CLUB_9,DDZ_CLUB_10,DDZ_CLUB_J,DDZ_CLUB_Q,DDZ_CLUB_K,DDZ_CLUB_A,DDZ_CLUB_2,
	DDZ_DIAMOND_3,DDZ_DIAMOND_4,DDZ_DIAMOND_5,DDZ_DIAMOND_6,DDZ_DIAMOND_7,DDZ_DIAMOND_8,DDZ_DIAMOND_9,DDZ_DIAMOND_10,DDZ_DIAMOND_J,DDZ_DIAMOND_Q,DDZ_DIAMOND_K,DDZ_DIAMOND_A,DDZ_DIAMOND_2,
	DDZ_JOKER_B,DDZ_JOKER_R, DDZ_CARD_COUNT
} POKERDDZ_TYPE;

typedef enum {
    DDZ_THREE = 0, DDZ_FOUR, DDZ_FIVE, DDZ_SIX, DDZ_SEVEN, DDZ_EIGHT, DDZ_NIGHT, DDZ_TEN, DDZ_JACK, DDZ_QUEEN, DDZ_KING, DDZ_ACE, DDZ_TWO, DDZ_J_B, DDZ_J_R
} POKERDDZ_NUM;

typedef enum {
    DDZ_SPADE = 0, DDZ_HEART, DDZ_CLUB, DDZ_DIAMOND, DDZ_JOKER_RED, DDZ_JOKER_BLACK,
} POKERDDZ_FACE;

class PokerDDZ : public Sprite {
public:
    static void getPack(Vector<PokerDDZ *> *list);
    static void randomPack(Vector<PokerDDZ *> *list);
    static PokerDDZ* create(int style, int ptype);
    bool isInRect(Touch* touch);
    void setPress(bool isPressed);
    bool isPress();
    int getPtype();
    int getNum();
    int getFace();
    void flipFront(float delay, float time);
    void flipBack(float delay, float time);
    Size getSizeScale();
    void addLordMark();
    void removeLordMark();
private:
    int _style;
    int _ptype;
    int _num;
    int _face;
    float _width;
    float _height;
    bool _isPress;
    void setSize();
    Rect getRect();
    PokerDDZ(int ptype);
    virtual ~PokerDDZ();
    
    void setFront();
    void setFace();
    void setNumIcon();
    void setPattern();
    
    void addActionFlipFront(Vector<FiniteTimeAction *> *actionList, float time);
    void cbTurnFront(Node *pSender);
    void turnFront();
    void addActionFlipBack(Vector<FiniteTimeAction *> *actionList, float time);
    void cbTurnBack(Node *pSender);
    void turnBack();
    
    virtual void onEnter() override;
    virtual void onExit() override;
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
};

#endif /* defined(__KillTime__PokerDDZ__) */
