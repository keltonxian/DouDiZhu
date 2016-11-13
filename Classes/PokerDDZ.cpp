//
//  PokerDDZ.cpp
//  KillTime
//
//  Created by keltonxian on 8/28/14.
//
//

#include "PokerDDZ.h"
#include "GameHeader.h"

#define TAG_LORD_MARK  123

void PokerDDZ::getPack(Vector<PokerDDZ *> *list)
{
    for (int i = POKERDDZ_TYPE::DDZ_SPADE_3; i <= POKERDDZ_TYPE::DDZ_JOKER_R; i++) {
        (*list).pushBack(PokerDDZ::create(STYLE_FRONT, i));
    }
    randomPack(list);
}

void PokerDDZ::randomPack(Vector<PokerDDZ *> *list)
{
    // need random seed
    srand((int)time(NULL));
    for (int i = 0; i < 100; i++) {
        ssize_t index1 = rand() % (*list).size();
        ssize_t index2 = rand() % (*list).size();
        (*list).swap(index1, index2);
    }
}

PokerDDZ* PokerDDZ::create(int style, int ptype)
{
    PokerDDZ *poker = new PokerDDZ(ptype);
    poker->_style = style;
    char path[500];
    if (style == STYLE_FRONT) {
        KUtil::getPath(path, 500, F_POKER, "card_1.png");
    } else if (style == STYLE_SMALL_FRONT) {
        KUtil::getPath(path, 500, F_POKER, "card_2.png");
    } else if (style == STYLE_SMALL_COVER) {
        KUtil::getPath(path, 500, F_POKER, "cover_2.png");
    } else {
        KUtil::getPath(path, 500, F_POKER, "cover_1.png");
    }
    if (poker && poker->initWithFile(path)) {
    poker->setSize();
    if (style == STYLE_FRONT) {
        poker->setFront();
    }
    poker->autorelease();
    return poker;
    }
    CC_SAFE_DELETE(poker);
    return NULL;
}

void PokerDDZ::setSize()
{
    _width = this->getContentSize().width;
    _height = this->getContentSize().height;
}

Rect PokerDDZ::getRect()
{
    auto scale = this->getScale();
    return Rect(-_width*scale / 2, -_height*scale / 2, _width*scale, _height*scale);
}

Size PokerDDZ::getSizeScale()
{
    auto scale = this->getScale();
    return Size(_width*scale, _height*scale);
}

PokerDDZ::PokerDDZ(int ptype):_ptype(ptype),_isPress(false)
{
    if (ptype == POKERDDZ_TYPE::DDZ_JOKER_B) {
        _num = POKERDDZ_NUM::DDZ_J_B;
        _face = POKERDDZ_FACE::DDZ_JOKER_BLACK;
    } else if (ptype == POKERDDZ_TYPE::DDZ_JOKER_R) {
        _num = POKERDDZ_NUM::DDZ_J_R;
        _face = POKERDDZ_FACE::DDZ_JOKER_RED;
    } else {
        _num = ptype % 13;
        _face = floor(ptype / 13);
    }
}

PokerDDZ::~PokerDDZ()
{
    
}

void PokerDDZ::onEnter()
{
    Sprite::onEnter();
    
//    auto listener = EventListenerTouchOneByOne::create();
//    listener->setSwallowTouches(true);
//
//    listener->onTouchBegan = CC_CALLBACK_2(PokerDDZ::onTouchBegan, this);
//    listener->onTouchMoved = CC_CALLBACK_2(PokerDDZ::onTouchMoved, this);
//    listener->onTouchEnded = CC_CALLBACK_2(PokerDDZ::onTouchEnded, this);
//
//    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void PokerDDZ::onExit()
{
    //    _eventDispatcher->removeEventListenersForTarget(this);
    Sprite::onExit();
}

bool PokerDDZ::onTouchBegan(Touch* touch, Event* event)
{
    return true;
}

void PokerDDZ::onTouchMoved(Touch* touch, Event* event)
{
    
}

void PokerDDZ::onTouchEnded(Touch* touch, Event* event)
{
    
}

void PokerDDZ::setFront()
{
//    Color3B color;
//    if (_face == POKERDDZ_FACE::DDZ_SPADE || _face == POKERDDZ_FACE::DDZ_CLUB || _face == POKERDDZ_FACE::DDZ_JOKER_RED) {
//        color = Color3B(0, 0, 0);
//    } else {
//        color = Color3B(255, 0, 0);
//    }
//    
//    if (_face != POKERDDZ_FACE::DDZ_JOKER_BLACK && _face != POKERDDZ_FACE::DDZ_JOKER_RED) {
//        char numStr[5] = {0};
//        memset(numStr, 0, 5);
//        if (_num == POKERDDZ_NUM::DDZ_JACK) {
//            sprintf(numStr, "%s", "J");
//        } else if (_num == POKERDDZ_NUM::DDZ_QUEEN) {
//            sprintf(numStr, "%s", "Q");
//        } else if (_num == POKERDDZ_NUM::DDZ_KING) {
//            sprintf(numStr, "%s", "K");
//        } else if (_num == POKERDDZ_NUM::DDZ_ACE) {
//            sprintf(numStr, "%s", "A");
//        } else if (_num == POKERDDZ_NUM::DDZ_TWO) {
//            sprintf(numStr, "%s", "2");
//        } else {
//            sprintf(numStr, "%d", _num+3);
//        }
//        KUtil::addLabel(this, numStr, 30, color, Point(10, _height-10), ANCHOR_LEFT_UP, 10);
//    }
    this->setPattern();
    this->setNumIcon();
    this->setFace();
}

void PokerDDZ::setNumIcon()
{
    if (_num == POKERDDZ_NUM::DDZ_J_B || _num == POKERDDZ_NUM::DDZ_J_R) {
        return;
    }
    char numStr[20] = {0};
    memset(numStr, 0, 20);
    if (_num == POKERDDZ_NUM::DDZ_JACK) {
        sprintf(numStr, "%s", "j");
    } else if (_num == POKERDDZ_NUM::DDZ_QUEEN) {
        sprintf(numStr, "%s", "q");
    } else if (_num == POKERDDZ_NUM::DDZ_KING) {
        sprintf(numStr, "%s", "k");
    } else if (_num == POKERDDZ_NUM::DDZ_ACE) {
        sprintf(numStr, "%s", "a");
    } else if (_num == POKERDDZ_NUM::DDZ_TWO) {
        sprintf(numStr, "%s", "2");
    } else {
        sprintf(numStr, "%d", _num+3);
    }
    char fname1[20] = {0};
    memset(fname1, 0, 20);
    if (_face == POKERDDZ_FACE::DDZ_SPADE || _face == POKERDDZ_FACE::DDZ_CLUB || _face == POKERDDZ_FACE::DDZ_JOKER_BLACK) {
        sprintf(fname1, "b_%s", numStr);
    } else {
        sprintf(fname1, "r_%s", numStr);
    }
    int x = 0;
    int y = 0;
    char fname[20] = {0};
    memset(fname, 0, 20);
    if (_style == STYLE_SMALL_FRONT) {
        x = 5;
        y = _height - 5;
        sprintf(fname, "%s_2.png", fname1);
    } else {
        x = 10;
        y = _height - 15;
        sprintf(fname, "%s_1.png", fname1);
    }
    KUtil::addSprite(this, KUtil::getPath(F_POKER, fname), Point(x, y), ANCHOR_LEFT_UP, 15);
}

void PokerDDZ::setFace()
{
    char filename[20] = {0};
    memset(filename, 0, 20);
    switch (_face) {
        case POKERDDZ_FACE::DDZ_SPADE:
            sprintf(filename, "%s", "spade");
            break;
        case POKERDDZ_FACE::DDZ_HEART:
            sprintf(filename, "%s", "heart");
            break;
        case POKERDDZ_FACE::DDZ_DIAMOND:
            sprintf(filename, "%s", "diamond");
            break;
        case POKERDDZ_FACE::DDZ_CLUB:
            sprintf(filename, "%s", "club");
            break;
        case POKERDDZ_FACE::DDZ_JOKER_RED:
            sprintf(filename, "%s", "t_joker_r");
            break;
        case POKERDDZ_FACE::DDZ_JOKER_BLACK:
            sprintf(filename, "%s", "t_joker_b");
            break;
            
        default:
            return;
    }
    char fname[20] = {0};
    memset(fname, 0, 20);
    if (_style == STYLE_SMALL_FRONT) {
        sprintf(fname, "%s_2.png", filename);
    } else {
        sprintf(fname, "%s_1.png", filename);
    }
    int x = 0;
    int y = 0;
    if (_face == POKERDDZ_FACE::DDZ_JOKER_BLACK || _face == POKERDDZ_FACE::DDZ_JOKER_RED) {
        if (_style == STYLE_SMALL_FRONT) {
            x = 5;
            y = _height - 5;
        } else {
            x = 10;
            y = _height - 15;
        }
    } else {
        if (_style == STYLE_SMALL_FRONT) {
            x = 5;
            y = _height - 30;
        } else {
            x = 10;
            y = _height - 80;
        }
    }
    KUtil::addSprite(this, KUtil::getPath(F_POKER, fname), Point(x, y), ANCHOR_LEFT_UP, 15);
}

void PokerDDZ::setPattern()
{
    if (_style == STYLE_SMALL_FRONT && _num != POKERDDZ_NUM::DDZ_J_R && _num != POKERDDZ_NUM::DDZ_J_B) {
        return;
    }
    char numStr[20] = {0};
    memset(numStr, 0, 20);
    if (_num == POKERDDZ_NUM::DDZ_JACK) {
        sprintf(numStr, "%s", "jack");
    } else if (_num == POKERDDZ_NUM::DDZ_QUEEN) {
        sprintf(numStr, "%s", "queen");
    } else if (_num == POKERDDZ_NUM::DDZ_KING) {
        sprintf(numStr, "%s", "king");
    } else if (_num == POKERDDZ_NUM::DDZ_J_B) {
        sprintf(numStr, "%s", "joker");
    } else if (_num == POKERDDZ_NUM::DDZ_J_R) {
        sprintf(numStr, "%s", "joker");
    } else {
        return;
    }
    char fname1[20] = {0};
    memset(fname1, 0, 20);
    if (_face == POKERDDZ_FACE::DDZ_SPADE || _face == POKERDDZ_FACE::DDZ_CLUB || _face == POKERDDZ_FACE::DDZ_JOKER_BLACK) {
        sprintf(fname1, "%s_b", numStr);
    } else {
        sprintf(fname1, "%s_r", numStr);
    }
    char fname[20] = {0};
    memset(fname, 0, 20);
    if (_style == STYLE_SMALL_FRONT) {
        sprintf(fname, "icon_%s_2.png", fname1);
    } else {
        sprintf(fname, "icon_%s_1.png", fname1);
    }
    KUtil::addSprite(this, KUtil::getPath(F_POKER, fname), Point(_width-5, 5), ANCHOR_RIGHT_DOWN, 10);
}

bool PokerDDZ::isInRect(Touch* touch)
{
    if (getRect().containsPoint(convertTouchToNodeSpaceAR(touch))) {
        return true;
    }
    return false;
}

void PokerDDZ::setPress(bool isPressed)
{
    _isPress = isPressed;
}

bool PokerDDZ::isPress()
{
    return _isPress;
}

int PokerDDZ::getNum()
{
    return _num;
}

int PokerDDZ::getFace()
{
    return _face;
}

int PokerDDZ::getPtype()
{
    return _ptype;
}

void PokerDDZ::flipFront(float delay, float time)
{
    if (_style == STYLE_FRONT || _style == STYLE_SMALL_FRONT) {
        return;
    }
    
    Vector<FiniteTimeAction *> list;
    FiniteTimeAction *action;
    action = DelayTime::create(delay);
    list.pushBack(action);
    addActionFlipFront(&list, time);
    
    action = Sequence::create(list);
    this->runAction(action);
}

void PokerDDZ::addActionFlipFront(Vector<FiniteTimeAction *> *actionList, float time)
{
    Vector<FiniteTimeAction *> list;
    FiniteTimeAction *action;
    action = OrbitCamera::create(time/2, 1, 0, 0, -90, 0, 0);
    list.pushBack(action);
    action = CallFuncN::create(CC_CALLBACK_1(PokerDDZ::cbTurnFront, this));
    list.pushBack(action);
    action = OrbitCamera::create(time/2, 1, 0, 90, -90, 0, 0);
    list.pushBack(action);
    action = Sequence::create(list);
    (*actionList).pushBack(action);
}

void PokerDDZ::cbTurnFront(Node *pSender)
{
    this->turnFront();
}

void PokerDDZ::turnFront()
{
    if (_style == STYLE_FRONT || _style == STYLE_SMALL_FRONT) {
        return;
    }
    this->removeAllChildren();
    char path[500];
    if (_style == STYLE_FRONT) {
        _style = STYLE_FRONT;
        KUtil::getPath(path, 500, F_POKER, "card_1.png");
    } else {
        _style = STYLE_SMALL_FRONT;
        KUtil::getPath(path, 500, F_POKER, "card_2.png");
    }
    this->setTexture(path);
    this->setFront();
}

void PokerDDZ::flipBack(float delay, float time)
{
    if (_style == STYLE_COVER || _style == STYLE_SMALL_COVER) {
        return;
    }
    
    Vector<FiniteTimeAction *> list;
    FiniteTimeAction *action;
    action = DelayTime::create(delay);
    list.pushBack(action);
    addActionFlipBack(&list, time);
    action = Sequence::create(list);
    this->runAction(action);
}

void PokerDDZ::addActionFlipBack(Vector<FiniteTimeAction *> *actionList, float time)
{
    Vector<FiniteTimeAction *> list;
    FiniteTimeAction *action;
    action = OrbitCamera::create(time/2, 1, 0, 0, -90, 0, 0);
    list.pushBack(action);
    action = CallFuncN::create(CC_CALLBACK_1(PokerDDZ::cbTurnBack, this));
    list.pushBack(action);
    action = OrbitCamera::create(time/2, 1, 0, 90, -90, 0, 0);
    list.pushBack(action);
    action = Sequence::create(list);
    (*actionList).pushBack(action);
}

void PokerDDZ::cbTurnBack(Node *pSender)
{
    this->turnBack();
}

void PokerDDZ::turnBack()
{
    if (_style == STYLE_COVER || _style == STYLE_SMALL_COVER) {
        return;
    }
    this->removeAllChildren();
    char path[500];
    if (_style == STYLE_COVER) {
        _style = STYLE_COVER;
        KUtil::getPath(path, 500, F_POKER, "cover_1.png");
    } else {
        _style = STYLE_SMALL_COVER;
        KUtil::getPath(path, 500, F_POKER, "cover_2.png");
    }
    this->setTexture(path);
}

void PokerDDZ::addLordMark()
{
    auto lordMark = KUtil::addSprite(this, KUtil::getPath(F_FRAME, "lord.png"), Point(_width, _height), ANCHOR_RIGHT_UP, 15);
    lordMark->setTag(TAG_LORD_MARK);
}

void PokerDDZ::removeLordMark()
{
    auto lordMark = this->getChildByTag(TAG_LORD_MARK);
    if (nullptr == lordMark) {
        return;
    }
    lordMark->removeFromParentAndCleanup(true);
}



