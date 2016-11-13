//
//  DouDiZhu.cpp
//  KillTime
//
//  Created by keltonxian on 8/28/14.
//
//

#include "DouDiZhu.h"

#include "MainScene.h"
#include "GameTool.h"
#include "Ai.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#import "KUtilIOS.h"
#endif

//#define SHOW_LOG

#define Y_DOWN_CARD_UNPRESS   140
#define Y_DOWN_CARD_PRESS     200
#define Y_PLAY_CARD           400
#define BTN_NULL       0
#define BTN_READY      1
#define BTN_CALLHOST   2
#define BTN_GRABHOST   3
#define BTN_DOUBLE     4
#define BTN_PLAY       5
#define SIDE_DOWN      1
#define SIDE_RIGHT     2
#define SIDE_LEFT      3
#define DDZ_WAIT       0
#define DDZ_CALLHOST   1
#define DDZ_GRABHOST   2
#define DDZ_DOUBLE     3
#define DDZ_PLAY       4
#define DDZ_RESULT     5
#define TAG_PLAYER_TALK   101
#define TAG_PLAYER_ROLE   102
#define SCALE_POKER_TABLE 0.7

Scene* DouDiZhu::createScene()
{
    auto scene = Scene::create();
    auto layer = DouDiZhu::create();
    scene->addChild(layer, 1, 0);
    return scene;
}

bool DouDiZhu::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    isCanTouch = true;
    PokerDDZ::getPack(&pack);
    setupPoker();
    sidePlayerX = 150;
    sidePlayerY = FULL_HEIGHT - 150;
    auto bg = KUtil::addSprite(this, KUtil::getPath(F_BG, "bg_3.png"), Point(HALF_WIDTH, 0), ANCHOR_CENTER_DOWN, 0);
    bg->setScale(KUtil::getScale(FULL_WIDTH, FULL_HEIGHT, bg->getContentSize(), true));
    
    auto table = KUtil::addSprite(this, KUtil::getPath(F_BG, "bg_4.png"), Point(HALF_WIDTH, 0), ANCHOR_CENTER_DOWN, 0);
    table->setScale(KUtil::getScale(FULL_WIDTH, FULL_HEIGHT, bg->getContentSize(), true));
    
    KUtil::addSprite(this, KUtil::getPath(F_FRAME, "icon_coin.png"), Point(sidePlayerX-60, sidePlayerY-85), ANCHOR_LEFT_UP, 0);
    KUtil::addSprite(this, KUtil::getPath(F_FRAME, "icon_coin.png"), Point(FULL_WIDTH-sidePlayerX-60, sidePlayerY-85), ANCHOR_LEFT_UP, 0);
    KUtil::addSprite(this, KUtil::getPath(F_FRAME, "icon_coin.png"), Point(sidePlayerX-90, Y_PLAY_CARD-85), ANCHOR_LEFT_UP, 0);
    coinLeft = GameTool::addLabelOutlineDefault(this, "0", 30, Point(sidePlayerX-15, sidePlayerY-82), Color4B(255, 255, 0, 255), Color4B(0, 0, 0, 255), 2, ANCHOR_LEFT_UP, 10);
    coinRight = GameTool::addLabelOutlineDefault(this, "0", 30, Point(FULL_WIDTH-sidePlayerX-15, sidePlayerY-82), Color4B(255, 255, 0, 255), Color4B(0, 0, 0, 255), 2, ANCHOR_LEFT_UP, 10);
    coinDown = GameTool::addLabelOutlineDefault(this, "0", 30, Point(sidePlayerX-45, Y_PLAY_CARD-82), Color4B(255, 255, 0, 255), Color4B(0, 0, 0, 255), 2, ANCHOR_LEFT_UP, 10);
    int cardNumMarkY = sidePlayerY-130;
    coverLeft = KUtil::addSprite(this, KUtil::getPath(F_FRAME, "icon_cover.png"), Point(sidePlayerX-60, cardNumMarkY), ANCHOR_LEFT_UP, 30);
    coverRight = KUtil::addSprite(this, KUtil::getPath(F_FRAME, "icon_cover.png"), Point(FULL_WIDTH-sidePlayerX-60, cardNumMarkY), ANCHOR_LEFT_UP, 30);
    
    countLeft = KUtil::addLabelBmf(this, "0", KUtil::getPath(F_FONT, "font_num_1.fnt"), Point(sidePlayerX-20, cardNumMarkY+5), ANCHOR_LEFT_UP, 30);
    countLeft->setScale(2);
    
    countRight = KUtil::addLabelBmf(this, "0", KUtil::getPath(F_FONT, "font_num_1.fnt"), Point(FULL_WIDTH-sidePlayerX-20, cardNumMarkY+5), ANCHOR_LEFT_UP, 30);
    countRight->setScale(2);
    
    int btnFontSize = 32;
    int btnY = Y_DOWN_CARD_UNPRESS+150;
    int btnOffsetX = 200;
    Vector<MenuItem *> itemArray;
    
    btnReady = GameTool::addBtn2(&itemArray, "准备", btnFontSize, Point(HALF_WIDTH, btnY), ANCHOR_CENTER_DOWN, CC_CALLBACK_1(DouDiZhu::callbackReady, this));
    btnReady->setVisible(false);
    
    btnCallHost = GameTool::addBtn2(&itemArray, "叫地主", btnFontSize, Point(HALF_WIDTH-btnOffsetX, btnY), ANCHOR_CENTER_DOWN, CC_CALLBACK_1(DouDiZhu::callbackCallHost, this));
    btnCallHost->setTag(1);
    btnCallHost->setVisible(false);
    
    btnNotCallHost = GameTool::addBtn2(&itemArray, "不叫", btnFontSize, Point(HALF_WIDTH+btnOffsetX, btnY), ANCHOR_CENTER_DOWN, CC_CALLBACK_1(DouDiZhu::callbackCallHost, this));
    btnNotCallHost->setTag(2);
    btnNotCallHost->setVisible(false);
    
    btnGrabHost = GameTool::addBtn2(&itemArray, "抢地主", btnFontSize, Point(HALF_WIDTH-btnOffsetX, btnY), ANCHOR_CENTER_DOWN, CC_CALLBACK_1(DouDiZhu::callbackGrabHost, this));
    btnGrabHost->setTag(1);
    btnGrabHost->setVisible(false);
    
    btnNotGrabHost = GameTool::addBtn2(&itemArray, "不抢", btnFontSize, Point(HALF_WIDTH+btnOffsetX, btnY), ANCHOR_CENTER_DOWN, CC_CALLBACK_1(DouDiZhu::callbackGrabHost, this));
    btnNotGrabHost->setTag(2);
    btnNotGrabHost->setVisible(false);
    
    btnDouble2 = GameTool::addBtn2(&itemArray, "2倍", btnFontSize, Point(HALF_WIDTH-btnOffsetX/2*3, btnY), ANCHOR_CENTER_DOWN, CC_CALLBACK_1(DouDiZhu::callbackDouble, this));
    btnDouble2->setTag(2);
    btnDouble2->setVisible(false);
    
    btnDouble4 = GameTool::addBtn2(&itemArray, "4倍", btnFontSize, Point(HALF_WIDTH, btnY), ANCHOR_CENTER_DOWN, CC_CALLBACK_1(DouDiZhu::callbackDouble, this));
    btnDouble4->setTag(4);
    btnDouble4->setVisible(false);
    
    btnNotDouble = GameTool::addBtn2(&itemArray, "不加倍", btnFontSize, Point(HALF_WIDTH+btnOffsetX/2*3, btnY), ANCHOR_CENTER_DOWN, CC_CALLBACK_1(DouDiZhu::callbackDouble, this));
    btnNotDouble->setTag(1);
    btnNotDouble->setVisible(false);
    
    btnPlay = GameTool::addBtn2(&itemArray, "出牌", btnFontSize, Point(HALF_WIDTH+btnOffsetX/2*3, btnY), ANCHOR_CENTER_DOWN, CC_CALLBACK_1(DouDiZhu::callbackPlay, this));
    btnPlay->setVisible(false);
    
    btnPass = GameTool::addBtn2(&itemArray, "不出", btnFontSize, Point(HALF_WIDTH-btnOffsetX/2*3, btnY), ANCHOR_CENTER_DOWN, CC_CALLBACK_1(DouDiZhu::callbackPass, this));
    btnPass->setVisible(false);
    
    btnTip = GameTool::addBtn2(&itemArray, "提示", btnFontSize, Point(HALF_WIDTH, btnY), ANCHOR_CENTER_DOWN, CC_CALLBACK_1(DouDiZhu::callbackTip, this));
    btnTip->setVisible(false);
    
    KUtil::addMenu(this, &itemArray, 150);
    
    toolbar = KUtil::addSprite(this,  KUtil::getPath(F_FRAME, "toolbar.png"), Point(HALF_WIDTH, FULL_HEIGHT), ANCHOR_CENTER_UP, 0);
    
    Vector<MenuItem *> toolbarItemArray;
    
    KUtil::addItemSpriteSimple(&toolbarItemArray, KUtil::getPath(F_BTN, "btn_back.png"), Point(25, 10), ANCHOR_LEFT_DOWN, CC_CALLBACK_1(DouDiZhu::callbackBack, this));
    
    KUtil::addMenu(toolbar, &toolbarItemArray, 150);
    
    GameTool::addLabelOutlineDefault(toolbar, "底分", 30, Point(140, 55), Color4B(0, 0, 0, 255), Color4B(255, 255, 255, 255), 2, ANCHOR_LEFT_DOWN, 10);
    baseCoin = GameTool::addLabelOutlineDefault(toolbar, "0", 30, Point(230, 55), Color4B(0, 55, 0, 255), Color4B(255, 255, 255, 255), 2, ANCHOR_LEFT_DOWN, 10);
    
    GameTool::addLabelOutlineDefault(toolbar, "倍数", 30, Point(140, 15), Color4B(0, 0, 0, 255), Color4B(255, 255, 255, 255), 2, ANCHOR_LEFT_DOWN, 10);
    numMultiple = GameTool::addLabelOutlineDefault(toolbar, "0", 30, Point(230, 15), Color4B(0, 55, 55, 255), Color4B(255, 255, 255, 255), 2, ANCHOR_LEFT_DOWN, 10);

    GameTool::addLabelOutlineDefault(toolbar, "时钟", 30, Point(400, 15), Color4B(0, 0, 0, 255), Color4B(255, 255, 255, 255), 2, ANCHOR_LEFT_DOWN, 10);
    labelClock = KUtil::addLabelBmf(toolbar, "0", KUtil::getPath(F_FONT, "font_num_1.fnt"), Point(480, 12), ANCHOR_LEFT_DOWN, 10);
    labelClock->setScale(1.2);
    updateClock(0);
    
    schedule(schedule_selector(DouDiZhu::updateClock), 1);
    
#ifdef SHOW_LOG
    logCount = 0;
    llog = GameTool::addLabelOutlineDefault(this, "", 25, Point(HALF_WIDTH, HALF_HEIGHT), Color4B(0, 0, 0, 255), Color4B(255, 255, 255, 255), 2, ANCHOR_CENTER_DOWN, 200, Size(400, HALF_HEIGHT/4*3), TextHAlignment::LEFT, TextVAlignment::TOP);
#endif
    
    showSideCount(false);
    initPlayer();
    initTable();
    
    return true;
}

int DouDiZhu::nextState()
{
    roundCount = 1;
    int begin = DDZ_WAIT;
    int end = DDZ_RESULT;
    _state = (++_state)>end?begin:_state;
    return _state;
}

void DouDiZhu::initTable()
{
    baseCoin->setString("3");
    numMultiple->setString("0");
    PokerDDZ::randomPack(&pack);
    auto defaultPos = Point(HALF_WIDTH, HALF_HEIGHT);
    for (int i = 0; i < pack.size(); i++) {
        auto poker = pack.at(i);
        poker->setPosition(defaultPos);
        poker->setScale(1);
        poker->setAnchorPoint(ANCHOR_CENTER_CENTER);
        poker->setTag((int)pack.size()-i);
        poker->setVisible(false);
        poker->removeLordMark();
    }
    updatePlayerCoin();
    _state = DDZ_WAIT;
    winSide = -1;
    roundCount = 0;
    grabSide = -1;
    currentSide = randomFirstHand();
    firstHand = currentSide;
    logic();
}

void DouDiZhu::cleanTable()
{
#ifdef SHOW_LOG
    llog->setString("");
    logCount = 0;
#endif
    removeAllTalk();
    removeShowHost();
    showSideCount(false);
    getPlayer(SIDE_DOWN)->cleanPokerList();
    getPlayer(SIDE_LEFT)->cleanPokerList();
    getPlayer(SIDE_RIGHT)->cleanPokerList();
    getPlayer(SIDE_DOWN)->setCallHost(true);
    getPlayer(SIDE_LEFT)->setCallHost(true);
    getPlayer(SIDE_RIGHT)->setCallHost(true);
    removeAllRoleFlag();
}

int DouDiZhu::randomFirstHand()
{
    srand((int)time(NULL));
    ssize_t m = rand() % 100;
    if (0 <= m && m < 34) {
        return SIDE_LEFT;
    }
    if (34 <= m && m < 68) {
        return SIDE_RIGHT;
    }
    return SIDE_DOWN;
}

void DouDiZhu::updatePlayerCoin()
{
    char numStr[50] = {0};
    memset(numStr, 0, 50);
    sprintf(numStr, "%d", getPlayer(SIDE_LEFT)->getCoin());
    coinLeft->setString(numStr);
    memset(numStr, 0, 50);
    sprintf(numStr, "%d", getPlayer(SIDE_RIGHT)->getCoin());
    coinRight->setString(numStr);
    memset(numStr, 0, 50);
    sprintf(numStr, "%d", getPlayer(SIDE_DOWN)->getCoin());
    coinDown->setString(numStr);
}

PlayerDDZ* DouDiZhu::nextPlayer()
{
    /**
 #define SIDE_DOWN      1
 #define SIDE_RIGHT     2
 #define SIDE_LEFT      3
     */
    int begin = SIDE_DOWN;
    int end = SIDE_LEFT;
    currentSide = (++currentSide)>end?begin:currentSide;
    PlayerDDZ *player = getPlayer(currentSide);
    return player;
}

void DouDiZhu::removeTalk(int side)
{
    auto player = getPlayer(side);
    if (NULL == player) {
        return;
    }
    auto sprite = player->getSprite()->getChildByTag(TAG_PLAYER_TALK);
    if (nullptr == sprite) {
        return;
    }
    sprite->removeFromParentAndCleanup(true);
}

void DouDiZhu::removeAllTalk()
{
    for (int i = SIDE_DOWN; i <= SIDE_LEFT; i++) {
        removeTalk(i);
    }
}

void DouDiZhu::removeShowHost()
{
    for (int i = 0; i < hostPokerShow.size(); i++) {
        auto poker = hostPokerShow.at(i);
        poker->removeFromParentAndCleanup(true);
    }
}

void DouDiZhu::callbackRemove(Node* pSender)
{
    pSender->removeFromParentAndCleanup(true);
}

void DouDiZhu::showTip(const char *tip)
{
    auto size = Size(300, 150);
    auto bg = GameTool::addTextBg(this, size, Point(HALF_WIDTH, HALF_HEIGHT), ANCHOR_CENTER_CENTER, 100);
    GameTool::addLabelOutlineDefault(bg, tip, 30, Point(0, 0), Color4B(255, 255, 255, 255), Color4B(100, 100, 100, 255), 2, ANCHOR_LEFT_DOWN, 5, size, TextHAlignment::CENTER, TextVAlignment::CENTER);
    Vector<FiniteTimeAction *> list;
    FiniteTimeAction *action;
    action = MoveBy::create(0.5, Point(0, 50));
    list.pushBack(action);
    action = DelayTime::create(0.5);
    list.pushBack(action);
    action = CallFuncN::create(CC_CALLBACK_1(DouDiZhu::callbackRemove, this));
    list.pushBack(action);
    action = Sequence::create(list);
    bg->runAction(action);
}

void DouDiZhu::showTalk(PlayerDDZ *player, const char *talk)
{
    if (roundCount == 1) {
        removeAllTalk();
    }
    auto sprite = player->getSprite();
    sprite->removeChildByTag(TAG_PLAYER_TALK);
    Point position;
    Point anchorPoint;
    if (player->getSide()==SIDE_RIGHT) {
        position = Point(0, 73);
        anchorPoint = ANCHOR_RIGHT_CENTER;
    } else if (player->getSide()==SIDE_LEFT) {
        position = Point(126, 73);
        anchorPoint = ANCHOR_LEFT_CENTER;
    } else {
        position = Point(126, 73);
        anchorPoint = ANCHOR_LEFT_CENTER;
    }
    auto label = GameTool::addLabelOutlineDefault(sprite, talk, 30, position, Color4B(0, 255, 0, 255), Color4B(100, 100, 100, 255), 2, anchorPoint, 5);
    label->setTag(TAG_PLAYER_TALK);
    Vector<FiniteTimeAction *> list;
    FiniteTimeAction *action;
    action = ScaleTo::create(0.3, 2);
    list.pushBack(action);
    action = CallFuncN::create(CC_CALLBACK_1(DouDiZhu::callbackLogic, this));
    list.pushBack(action);
    action = Sequence::create(list);
    label->runAction(action);
}

void DouDiZhu::hidePlayedPoker(Vector<PokerDDZ *> playedList)
{
    for (int i = 0; i < playedList.size(); i++) {
        auto poker = playedList.at(i);
        poker->setVisible(false);
    }
}

void DouDiZhu::updatePlayList(PokerDDZ *poker, bool isAdd)
{
    if (true == isAdd) {
        playList.pushBack(poker);
        return;
    }
    int index = -1;
    for (int i = 0; i < playList.size(); i++) {
        auto p = playList.at(i);
        if (p->getPtype() == poker->getPtype()) {
            index = i;
            break;
        }
    }
    if (-1 == index) {
        return;
    }
    playList.erase(index);
}

void DouDiZhu::aiPlay(PlayerDDZ *player)
{
    if (player->getSide() == SIDE_LEFT) {
        hidePlayedPoker(sideLeftPlayedList);
    } else { // SIDE_RIGHT
        hidePlayedPoker(sideRightPlayedList);
    }
    Vector<PokerDDZ *> tipPokerList;
    Vector<PokerDDZ *> pokerList = *(player->getPokerList());
    CardType *cardType = CardType::create(lastCardType, lastNum);
    int ret = Ai::choosePoker(pokerList, pack, cardType, lastPlayListSize, 0, &tipPokerList);
    size_t size = tipPokerList.size();
    if (ret != 0 || size == 0) {
        showTalk(player, "不出!!");
        return;
    }
    std::sort( std::begin(tipPokerList), std::end(tipPokerList), PlayerDDZ::DDZPokerListComparisonLess );
    CardType *validCardType = Ai::validCardsAndGetCardType(CardType::create(lastCardType, lastNum), lastPlayListSize, tipPokerList, -1);
    CCLOG("ai出牌判断 cardType[%d],num[%d]", validCardType->getCardType(), validCardType->getNum());
    if (validCardType->isIllegeal() || validCardType->isNone()) {
        showTalk(player, "不出!!");
        return;
    }
    CardType *ct = Ai::getCardType(tipPokerList);
    lastCardType = ct->getCardType();
    lastNum = ct->getNum();
    lastPlayListSize = (int)tipPokerList.size();
    //
    auto width = 0;
    if (size > 0) {
        width = tipPokerList.at(0)->getSizeScale().width;
    }
    float gapy = width / 3;
    //
    float offsetx = 170;
    float offsety = -150;
    for (int i = 0; i < size; i++) {
        PokerDDZ *poker = tipPokerList.at(i);
        poker->setVisible(true);
        poker->setScale(SCALE_POKER_TABLE);
//        CCLOG("side[%d] play p[%d,%d]", player->getSide(), poker->getFace(), poker->getNum());
        player->removePoker(poker);
        Point position;
        if (player->getSide() == SIDE_LEFT) {
            sideLeftPlayedList.pushBack(poker);
            updatePlayerPokerCount(player, countLeft);
            position = Point(sidePlayerX + offsetx + gapy*i, sidePlayerY+offsety);
        } else { // SIDE_RIGHT
            sideRightPlayedList.pushBack(poker);
            updatePlayerPokerCount(player, countRight);
            position = Point(FULL_WIDTH - sidePlayerX - offsetx - gapy*(size-1-i), sidePlayerY+offsety);
        }
        poker->setPosition(position);
    }
    sortPlaySprite(tipPokerList);
    sortPokerSprite(player, 0.5, true);
    lastPlaySide = player->getSide();
    
    printCardTypeLog();
}

void DouDiZhu::logic()
{
    roundCount++;
    int state;
    PlayerDDZ *player = nullptr;
    if (roundCount > 3 && _state != DDZ_PLAY) {
        if (_state == DDZ_CALLHOST) {
            restart();
            return;
        }
        state = nextState();
        if (_state == DDZ_CALLHOST) {
            roundCount = 0;
            lastCardType = 0;
            lastNum = 0;
            lastPlayListSize = 0;
            lastPlaySide = -1;
            hostPokerShow.clear();
            playList.clear();
            sideDownPlayedList.clear();
            sideLeftPlayedList.clear();
            sideRightPlayedList.clear();
            showSideCount(true);
            dealPoker();
            return;
        }
        if (_state == DDZ_DOUBLE) {
            roundCount = 0;
            nextState(); // skip double
            removeAllTalk();
            currentSide = -1;
            addRoleFlag(grabSide);
            dealHostPoker(grabSide);
            return;
        }
//        if (_state == DDZ_PLAY) {
//            for (int i = 0; i < playList.size(); i++) {
//                auto poker = playList.at(i);
//                pokerUp(poker, false);
//            }
//            playList.clear();
//            removeAllTalk();
//            currentSide = grabSide;
//            player = getPlayer(currentSide);
//        }
    }
    if (_state == DDZ_PLAY) {
        if (-1 == currentSide) {
            for (int i = 0; i < playList.size(); i++) {
                auto poker = playList.at(i);
                pokerUp(poker, false);
            }
            playList.clear();
            removeAllTalk();
            currentSide = grabSide;
            player = getPlayer(currentSide);
        }
        if (true == checkGameOver()) {
            isCanTouch = false;
            this->scheduleOnce(schedule_selector(DouDiZhu::showResult), 0.2);
            return;
        }
    }
    if (nullptr == player) {
        player = nextPlayer();
    }
    if (_state == DDZ_GRABHOST && grabSide == player->getSide()) {
        showBtn(BTN_NULL);
        logic();
        return;
    }
    if (_state == DDZ_PLAY) {
        removeTalk(player->getSide());
        if (-1 != lastPlaySide && lastPlaySide == player->getSide()) {
            lastCardType = 0;
            lastNum = 0;
            lastPlayListSize = 0;
        }
    }
    if (false == player->isAi()) {
        if (_state == DDZ_WAIT) {
            showBtn(BTN_READY);
        } else if (_state == DDZ_CALLHOST) {
            showBtn(BTN_CALLHOST);
        } else if (_state == DDZ_GRABHOST) {
            auto isCall = player->getCallHost();
            if (false == isCall) {
                showTalk(getPlayer(SIDE_DOWN), "不抢!!");
                return;
            }
            showBtn(BTN_GRABHOST);
        } else if (_state == DDZ_DOUBLE) {
            showBtn(BTN_DOUBLE);
        } else if (_state == DDZ_PLAY) {
            hidePlayedPoker(sideDownPlayedList);
            showBtn(BTN_PLAY);
        }
        return;
    }
    showBtn(BTN_NULL);
    if (_state == DDZ_WAIT) {
        showTalk(player, "准备!!");
        return;
    }
    if (_state == DDZ_CALLHOST) {
        bool isCall = false;
        srand((int)time(NULL));
        ssize_t m = rand() % 20;
        if (m < 15) {
            isCall = true;
        }
        player->setCallHost(isCall);
        if (true == isCall) {
            grabSide = player->getSide();
            nextState();
            roundCount = 0;
            showTalk(player, "叫地主!!");
            numMultiple->setString("3");
            return;
        }
        showTalk(player, "不叫!!");
        return;
    }
    if (_state == DDZ_GRABHOST) {
        bool isGrab = false;
        srand((int)time(NULL));
        ssize_t m = rand() % 20;
        if (m < 6) {
            isGrab = true;
        }
        auto isCall = player->getCallHost();
        if (true == isCall && true == isGrab) {
            grabSide = player->getSide();
            showTalk(player, "抢地主!!");
            setDoubleMultiple();
            return;
        }
        showTalk(player, "不抢!!");
        return;
    }
    if (_state == DDZ_DOUBLE) {
        showTalk(player, "不加倍!!");
        return;
    }
    if (_state == DDZ_PLAY) {
        aiPlay(player);
        return;
    }
}

void DouDiZhu::addRoleFlag(int hostSide)
{
    for (int i = 0; i < playerList.size(); i++) {
        auto player = playerList.at(i);
        auto sprite = player->getSprite();
        char fname[50] = {0};
        memset(fname, 0, 50);
        if (player->getSide() == hostSide) {
            sprintf(fname, "%s", "flag_host.png");
        } else {
            sprintf(fname, "%s", "flag_farmer.png");
        }
        auto f = KUtil::addSprite(sprite, KUtil::getPath(F_FRAME, fname), Point(0, 0), ANCHOR_CENTER_DOWN, 1);
        f->setTag(TAG_PLAYER_ROLE);
        f->setScale(3);
        Vector<FiniteTimeAction *> list;
        FiniteTimeAction *action;
//        action = EaseOut::create(ScaleTo::create(0.5, 1.2), 0.5);
//        list.pushBack(action);
        action = EaseOut::create(ScaleTo::create(1, 1), 1);
        list.pushBack(action);
        action = Sequence::create(list);
        f->runAction(action);
//        auto bg = KUtil::addFrame1(sprite, Size(120, 56), Point(126, 0), ANCHOR_CENTER_DOWN, 20);
//        bg->setTag(TAG_PLAYER_ROLE);
//        Color4B color;
//        char str[50] = {0};
//        memset(str, 0, 50);
//        if (player->getSide() == hostSide) {
//            sprintf(str, "%s", "地主");
//            color = Color4B(255, 0, 0, 255);
//            sprintf(fname, "%s", "landlord.png");
//        } else {
//            sprintf(str, "%s", "农民");
//            color = Color4B(0, 255, 0, 255);
//            sprintf(fname, "%s", "farmer.png");
//        }
//        KUtil::addLabelOutline1(bg, str, 22, Point(60, 28), color, Color4B(100, 100, 100, 255), 2, ANCHOR_CENTER_CENTER, 1);
    }
}

void DouDiZhu::removeAllRoleFlag()
{
    for (int i = 0; i < playerList.size(); i++) {
        auto player = playerList.at(i);
        auto sprite = player->getSprite();
        auto f = sprite->getChildByTag(TAG_PLAYER_ROLE);
        if (nullptr != f) {
            f->removeFromParentAndCleanup(true);
        }
    }
}

void DouDiZhu::callbackLogic(Node* pSender)
{
    logic();
}

bool DouDiZhu::checkGameOver()
{
    for (int i = 0; i < playerList.size(); i++) {
        auto player = playerList.at(i);
        auto len = (*player->getPokerList()).size();
        if (0 == len) {
            winSide = player->getSide();
            return true;
        }
    }
    return false;
}

void DouDiZhu::showResult(float delta)
{
    int base = atoi(baseCoin->getString().c_str());
    int multiple = atoi(numMultiple->getString().c_str());
    int coin = base * multiple;
    PlayerDDZ *playerWin = nullptr;
    for (int i = 0; i < playerList.size(); i++) {
        auto player = playerList.at(i);
        player->setMatchCount(player->getMatchCount() + 1);
        if (player->getSide() == winSide) {
            playerWin = player;
            continue;
        }
    }
    playerWin->setMatchWinCount(playerWin->getMatchWinCount() + 1);
    auto layer = ResultLayer::create();
    layer->setData(coin, winSide, playerList, CC_CALLBACK_1(DouDiZhu::callbackResult, this));
    KUtil::addLayer(layer, 20, 0);
    saveGame();
}

void DouDiZhu::callbackResult(Ref* pSender)
{
    restart();
    isCanTouch = true;
}

void DouDiZhu::callbackEnableTouch(float delta)
{
    isCanTouch = true;
}

void DouDiZhu::restart()
{
    cleanTable();
    initTable();
}

#pragma mark button handler start
void DouDiZhu::showBtn(int btnType)
{
    switch (btnType) {
        case BTN_READY:
        {
            btnReady->setVisible(true);
            btnCallHost->setVisible(false);
            btnNotCallHost->setVisible(false);
            btnGrabHost->setVisible(false);
            btnNotGrabHost->setVisible(false);
            btnDouble2->setVisible(false);
            btnDouble4->setVisible(false);
            btnNotDouble->setVisible(false);
            btnPlay->setVisible(false);
            btnPass->setVisible(false);
            btnTip->setVisible(false);
        }
            break;
        case BTN_CALLHOST:
        {
            btnReady->setVisible(false);
            btnCallHost->setVisible(true);
            btnNotCallHost->setVisible(true);
            btnGrabHost->setVisible(false);
            btnNotGrabHost->setVisible(false);
            btnDouble2->setVisible(false);
            btnDouble4->setVisible(false);
            btnNotDouble->setVisible(false);
            btnPlay->setVisible(false);
            btnPass->setVisible(false);
            btnTip->setVisible(false);
        }
            break;
        case BTN_GRABHOST:
        {
            btnReady->setVisible(false);
            btnCallHost->setVisible(false);
            btnNotCallHost->setVisible(false);
            btnGrabHost->setVisible(true);
            btnNotGrabHost->setVisible(true);
            btnDouble2->setVisible(false);
            btnDouble4->setVisible(false);
            btnNotDouble->setVisible(false);
            btnPlay->setVisible(false);
            btnPass->setVisible(false);
            btnTip->setVisible(false);
        }
            break;
        case BTN_DOUBLE:
        {
            btnReady->setVisible(false);
            btnCallHost->setVisible(false);
            btnNotCallHost->setVisible(false);
            btnGrabHost->setVisible(false);
            btnNotGrabHost->setVisible(false);
            btnDouble2->setVisible(true);
            btnDouble4->setVisible(true);
            btnNotDouble->setVisible(true);
            btnPlay->setVisible(false);
            btnPass->setVisible(false);
            btnTip->setVisible(false);
        }
            break;
        case BTN_PLAY:
        {
            btnReady->setVisible(false);
            btnCallHost->setVisible(false);
            btnNotCallHost->setVisible(false);
            btnGrabHost->setVisible(false);
            btnNotGrabHost->setVisible(false);
            btnDouble2->setVisible(false);
            btnDouble4->setVisible(false);
            btnNotDouble->setVisible(false);
            btnPlay->setVisible(true);
            btnPass->setVisible(true);
            btnTip->setVisible(true);
        }
            break;
            
        default:
        {
            btnReady->setVisible(false);
            btnCallHost->setVisible(false);
            btnNotCallHost->setVisible(false);
            btnGrabHost->setVisible(false);
            btnNotGrabHost->setVisible(false);
            btnDouble2->setVisible(false);
            btnDouble4->setVisible(false);
            btnNotDouble->setVisible(false);
            btnPlay->setVisible(false);
            btnPass->setVisible(false);
            btnTip->setVisible(false);
        }
            break;
    }
}

void DouDiZhu::callbackReady(cocos2d::Ref* pSender)
{
    showTalk(getPlayer(SIDE_DOWN), "准备!!");
}

void DouDiZhu::callbackCallHost(cocos2d::Ref* pSender)
{
    int tag = ((MenuItemSprite *)pSender)->getTag();
    bool isCall = false;
    if (1 == tag) {
        grabSide = SIDE_DOWN;
        nextState();
        roundCount = 0;
        showTalk(getPlayer(SIDE_DOWN), "叫地主!!");
        numMultiple->setString("3");
        isCall = true;
    } else {
        showTalk(getPlayer(SIDE_DOWN), "不叫!!");
        isCall = false;
    }
    getPlayer(SIDE_DOWN)->setCallHost(isCall);
}

void DouDiZhu::callbackGrabHost(cocos2d::Ref* pSender)
{
    int tag = ((MenuItemSprite *)pSender)->getTag();
    if (1 == tag) {
        grabSide = SIDE_DOWN;
        showTalk(getPlayer(SIDE_DOWN), "抢地主!!");
        setDoubleMultiple();
    } else {
        showTalk(getPlayer(SIDE_DOWN), "不抢!!");
    }
}

void DouDiZhu::callbackDouble(cocos2d::Ref* pSender)
{
    int tag = ((MenuItemSprite *)pSender)->getTag();
    if (2 == tag) {
        showTalk(getPlayer(SIDE_DOWN), "2倍!!");
    } else if (4 == tag) {
        showTalk(getPlayer(SIDE_DOWN), "4倍!!");
    } else {
        showTalk(getPlayer(SIDE_DOWN), "不加倍!!");
    }
}

void DouDiZhu::callbackPlay(cocos2d::Ref* pSender)
{
    CCLOG("========");
    if (0 == playList.size()) {
        showTip("请选择要出的牌");
        return;
    }
    CCLOG("出牌 lastCardType[%d],lastNum[%d],lastPlayListSize[%d]", lastCardType, lastNum, lastPlayListSize);
    
    std::sort( std::begin(playList), std::end(playList), PlayerDDZ::DDZPokerListComparisonLess );
    for (int i = 0; i < playList.size(); i++) {
        auto p = playList.at(i);
        CCLOG("出牌的牌 num[%d],face[%d]", p->getNum(), p->getFace());
    }
    CardType *cardType = Ai::validCardsAndGetCardType(CardType::create(lastCardType, lastNum), lastPlayListSize, playList, -1);
    CCLOG("出牌判断 cardType[%d],num[%d]", cardType->getCardType(), cardType->getNum());
    if (cardType->isIllegeal() || cardType->isNone()) {
        showTip("出牌不合规则");
        return;
    }
    CCLOG("========");
//    cardType = Ai::getCardType(playList);
//    CCLOG("Play CardType[%d][%d]", cardType->getCardType(), cardType->getNum());
    lastCardType = cardType->getCardType();
    lastNum = cardType->getNum();
    lastPlayListSize = (int)playList.size();
    showBtn(BTN_NULL);
    
    size_t len = playList.size();
    auto width = 0;
    if (len > 0) {
        width = playList.at(0)->getSizeScale().width;
    }
    float gapy = width / 3;
    auto player = getPlayer(SIDE_DOWN);
    float startX = HALF_WIDTH - ((gapy * (len-1) + width))/2;
    for (int i = 0; i < playList.size(); i++) {
        auto poker = playList.at(i);
//        CCLOG("play p[%d,%d]", poker->getFace(), poker->getNum());
        player->removePoker(poker);
        sideDownPlayedList.pushBack(poker);
        Point pos = Point(startX+width/2+gapy*i, Y_PLAY_CARD);
        moveScaleAnimation(poker, pos, SCALE_POKER_TABLE, 0, 0.2);
    }
    sortPlaySprite(playList);
    sortPokerSprite(player, 0, true);
    playList.clear();
    lastPlaySide = SIDE_DOWN;
    
    printCardTypeLog();
}

void DouDiZhu::callbackPass(cocos2d::Ref* pSender)
{
    if (-1 == lastPlaySide) {
        showTip("请出牌");
        return;
    }
    auto player = getPlayer(SIDE_DOWN);
    if (-1 != lastPlaySide && lastPlaySide == player->getSide()) {
        showTip("请出牌");
        return;
    }
    for (int i = 0; i < playList.size(); i++) {
        auto poker =playList.at(i);
        pokerUp(poker, false);
    }
    playList.clear();
    showTalk(player, "不出!!");
}

void DouDiZhu::callbackTip(cocos2d::Ref* pSender)
{
    resetPlayList();
    Vector<PokerDDZ *> tipPokerList;
    Vector<PokerDDZ *> myPokerList = *(getPlayer(SIDE_DOWN)->getPokerList());
    CardType *cardType = CardType::create(lastCardType, lastNum);
    int ret = Ai::choosePoker(myPokerList, pack, cardType, lastPlayListSize, 0, &tipPokerList);
    if (ret != 0) {
        return;
    }
    CardType *ct = Ai::getCardType(tipPokerList);
    CCLOG("CardType[%d][%d]", ct->getCardType(), ct->getNum());
    if (ct->getCardType() == Ai::CARD_TYPE_NONE || ct->getCardType() == Ai::CARD_TYPE_ILLEGAL) {
        callbackPass(nullptr);
        return;
    }
    for (int i = 0; i < tipPokerList.size(); i++) {
        PokerDDZ *poker = tipPokerList.at(i);
        pokerUp(poker, true);
    }
}

void DouDiZhu::callbackBack(cocos2d::Ref* pSender)
{
    Director::getInstance()->replaceScene(TransitionSlideInB::create(0.4, MainScene::createScene()));
}
#pragma mark button handler end

#pragma mark time start
void DouDiZhu::setDoubleMultiple()
{
    std::string numStr = numMultiple->getString();
    int num = atoi(numStr.c_str());
    char str[50] = {0};
    memset(str, 0, 50);
    sprintf(str, "%d", num*2);
    numMultiple->setString(str);
}

void DouDiZhu::updateClock(float delta)
{
    struct timeval nowTimeval;
    gettimeofday(&nowTimeval, NULL);
    struct tm * tm;
    time_t time_sec ;
    time_sec = nowTimeval.tv_sec;
    tm = localtime(&time_sec);
    int nMinute = tm->tm_min;
    int nSecond = tm->tm_sec;
    int nHour = tm->tm_hour;
    char timeStr[50] = {0};
    memset(timeStr, 0, 50);
    sprintf(timeStr, "%02d:%02d:%02d", nHour, nMinute, nSecond);
    labelClock->setString(timeStr);
}

void DouDiZhu::showSideCount(bool isShow)
{
    coverLeft->setVisible(isShow);
    coverRight->setVisible(isShow);
    countLeft->setVisible(isShow);
    countRight->setVisible(isShow);
}
#pragma mark time end

#pragma mark player start
void DouDiZhu::addPlayer(PlayerDDZ *player)
{
    playerList.pushBack(player);
    Point position;
    if (player->getIcon() < 0 || player->getIcon() > 2) {
        CCLOG("ERROR DouDiZhu::addPlayer iconType icon(%d)", player->getIcon());
        return;
    }
    char iconType[20] = {0};
    memset(iconType, 0, 20);
    sprintf(iconType, "icon_%d.png", player->getIcon());
    switch (player->getSide()) {
        case SIDE_DOWN:
        {
            position = Point(sidePlayerX-30, Y_PLAY_CARD);
        }
            break;
        case SIDE_LEFT:
        {
            position = Point(sidePlayerX, sidePlayerY);
        }
            break;
        case SIDE_RIGHT:
        {
            position = Point(FULL_WIDTH-sidePlayerX, sidePlayerY);
        }
            break;
            
        default:
            CCLOG("ERROR DouDiZhu::addPlayer iconPos side(%d)", player->getSide());
            return;
    }
    auto frame = KUtil::addSprite(this, KUtil::getPath(F_FRAME, "frame_icon.png"), position, ANCHOR_CENTER_CENTER, 10);
    auto icon = KUtil::addSprite(frame, KUtil::getPath(F_ICON, iconType), Point(61, 72), ANCHOR_CENTER_CENTER, -1);
    icon->setScale(0.95);
    GameTool::addLabelOutlineDefault(frame, player->getName(), 30, Point(63, 146), Color4B(0, 0, 0, 255), Color4B(255, 255, 255, 255), 2, ANCHOR_CENTER_DOWN, 1);
    player->setSprite(frame);
}

PlayerDDZ* DouDiZhu::loadPlayerById(int pid, int side)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    std::string atlas = KUtilIOS::getStringValueByKey("Player", "atlas", "路人", pid);
    int iconIndex = KUtilIOS::getIntValueByKey("Player", "icon", 0, pid);
    int score = KUtilIOS::getIntValueByKey("Player", "normal_match_score", 0, pid);
    int matchWinCount = KUtilIOS::getIntValueByKey("Player", "normal_match_win_count", 0, pid);
    int matchCount = KUtilIOS::getIntValueByKey("Player", "normal_match_count", 0, pid);
    auto player = PlayerDDZ::createPlayer(pid, side, atlas, iconIndex, (side==SIDE_DOWN)?false:true, score, matchWinCount, matchCount);
    //Player::createPlayer(pid, atlas, iconIndex, (side==SIDE_DOWN)?false:true, score, matchWinCount, matchCount, side);
    return player;
#else
    auto path = FileUtils::getInstance()->getWritablePath() + SAVE_DATA_1;
    ValueMap saveData = FileUtils::getInstance()->getValueMapFromFile(path);
    if (0 == saveData.size()) {
        return PlayerDDZ::createPlayer(pid, side, "路人", 1, (side==SIDE_DOWN)?false:true, 100);
    }
    char key[20] = {0};
    memset(key, 0, 20);
    sprintf(key, "%d", pid);
//    char v[200] = {0};
//    memset(v, 0, 200);
//    sprintf(v, "%s_%d_%d", player->getName().c_str(), player->getIcon(), player->getCoin());
    ValueMap::iterator iter = saveData.find(key);
    if (iter == saveData.end()) {
        return PlayerDDZ::createPlayer(pid, side, "路人", 1, (side==SIDE_DOWN)?false:true, 100);
    }
//    const char *v = saveData.at(key).asString().c_str();
    std::string s = saveData.at(key).asString();
    char name[100] = {0};
    memset(name, 0, 100);
    int icon;
    int coin;
//    int ret = sscanf(v, "%s %d %d", name, &icon, &coin);
    int ret = sscanf(s.c_str(), "%s %d %d", name, &icon, &coin);
    if (3 != ret) {
        return PlayerDDZ::createPlayer(pid, side, "路人", 1, (side==SIDE_DOWN)?false:true, 100);
    }
    return PlayerDDZ::createPlayer(pid, side, name, icon, (side==SIDE_DOWN)?false:true, coin);
#endif
}

void DouDiZhu::initPlayer()
{
//    addPlayer(PlayerDDZ::createPlayer(1, SIDE_DOWN, "kelton", 1, false, 100));
//    addPlayer(PlayerDDZ::createPlayer(2, SIDE_LEFT, "natassa", 2, true, 100));
//    addPlayer(PlayerDDZ::createPlayer(3, SIDE_RIGHT, "donggua", 3, true, 100));
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    addPlayer(loadPlayerById(0, SIDE_DOWN));
    addPlayer(loadPlayerById(1, SIDE_LEFT));
    addPlayer(loadPlayerById(2, SIDE_RIGHT));
#else
    addPlayer(loadPlayerById(1, SIDE_DOWN));
    addPlayer(loadPlayerById(2, SIDE_LEFT));
    addPlayer(loadPlayerById(3, SIDE_RIGHT));
#endif
}

PlayerDDZ* DouDiZhu::getPlayer(int side)
{
    ssize_t total = playerList.size();
    for (int i = 0; i < total; i++) {
        auto player = playerList.at(i);
        if (player->getSide() == side) {
            return player;
        }
    }
    return NULL;
}
#pragma mark player end

void DouDiZhu::setupPoker()
{
    ssize_t total = pack.size();
    auto defaultPos = Point(HALF_WIDTH, HALF_HEIGHT);
    
    for (int i = 0; i < total; i++) {
        auto poker = pack.at(i);
        poker->setPosition(defaultPos);
        poker->setScale(SCALE_X);
        poker->setAnchorPoint(ANCHOR_CENTER_CENTER);
        poker->setTag((int)total-i);
        poker->setVisible(false);
        this->addChild(poker, PD_ZORDER_NORMAL+1);
    }
}

void DouDiZhu::dealPoker()
{
    isCanTouch = false;
    PlayerDDZ *player = getPlayer(SIDE_DOWN);
    ssize_t total = pack.size();
    auto size = pack.at(0)->getSizeScale();
    float delay = 0;
    int index = (int)total-1;
    for (int i = index; i >= 20; i--, index--) {
        auto poker = pack.at(i);
        delay = moveSideAnimation(poker, delay, i%2==0);
    }
    for (int i = index; i >= 3; i--, index--) {
        auto poker = pack.at(i);
        player->insertPoker(poker);
    }
    player->sortPokerList();
    auto list = *(player->getPokerList());
    int len = player->getPokerCount();
    float hwidth = (FULL_WIDTH-size.width) / (len-1);
    if (hwidth > size.width/3*2) {
        hwidth = size.width/3*2;
    }
    float startx = HALF_WIDTH - (hwidth*(len-1)+size.width)/2;
    float speed = 250;
    float timeWaitTouch = 0;
    for (int i = 0; i < len; i++) {
        auto poker = list.at(i);
        poker->setVisible(true);
        poker->setTag(i);
        this->reorderChild(poker, i);
        poker->setPosition(Point(HALF_WIDTH, Y_DOWN_CARD_UNPRESS));
        Point pos = Point(startx + size.width/2 + i * hwidth, Y_DOWN_CARD_UNPRESS);
        auto time = abs(pos.x-HALF_WIDTH) / speed;
        if (time > timeWaitTouch) {
            timeWaitTouch = time;
        }
        this->moveAnimation(poker, pos, (9-abs(9-i))/10, time);
    }
//    float hostScale = 1;
    for (int i = 2; i >= 0; i--) {
        auto poker = pack.at(i);
//        poker->setVisible(true);
        poker->setTag(2-i);
//        poker->setScale(hostScale);
//        delay = this->moveAnimation(poker, Point(HALF_WIDTH+(1-i)*50, FULL_HEIGHT/4*3), 0, 0.5);
        delay = this->moveAnimation(poker, Point(200+(3-i)*30, FULL_HEIGHT), 0, 0.5);
        delay += 0.1;
        auto showPoker = PokerDDZ::create(STYLE_SMALL_COVER, poker->getPtype());
        auto position = Point(270+(3-i)*30, 20);
        showHostPoker(showPoker, delay, position);
        if (0 == i) {
            delayCallLogic(poker, delay);
        }
    }
    
    scheduleOnce(schedule_selector(DouDiZhu::callbackEnableTouch), timeWaitTouch);
}

void DouDiZhu::showHostPoker(PokerDDZ *poker, float delay, Point position)
{
    poker->setPosition(position);
    poker->setAnchorPoint(ANCHOR_CENTER_DOWN);
//    poker->setScale(0.5);
    toolbar->addChild(poker, PD_ZORDER_NORMAL+1);
    hostPokerShow.pushBack(poker);
    Vector<FiniteTimeAction *> list;
    FiniteTimeAction *action;
    action = Hide::create();
    list.pushBack(action);
    action = DelayTime::create(delay);
    list.pushBack(action);
    action = Show::create();
    list.pushBack(action);
    action = Sequence::create(list);
    poker->runAction(action);
}

void DouDiZhu::dealHostPoker(int side)
{
    isCanTouch = false;
    for (int i = 0; i < hostPokerShow.size(); i++) {
        auto poker = hostPokerShow.at(i);
        poker->flipFront(0, 0.5);
    }
    hostSide = side;
    PlayerDDZ *player;
    for (int i = 0; i < playerList.size(); i++) {
        auto p = playerList.at(i);
        p->setIsHost(side==p->getSide());
        if (true == p->getIsHost()) {
            player = p;
        }
    }
//    auto player = getPlayer(side);
    Point position = player->getSprite()->getPosition();
    float delay = 0;
    for (int i = 2; i >= 0; i--) {
        auto poker = pack.at(i);
        poker->addLordMark();
        poker->setScale(1);
        poker->setPosition(position);
        if (side == SIDE_DOWN) {
            poker->setVisible(true);
            player->insertPoker(poker);
        }
        delay = moveAnimation(poker, position, delay, 0.1);
        if (side != SIDE_DOWN) {
            delay = moveSideAnimation(poker, delay, side==SIDE_LEFT);
        }
        delay += 0.1;
    }
    delay += 0.1;
    playList.clear();
    delay = sortPokerSprite(player, delay, true);
    scheduleOnce(schedule_selector(DouDiZhu::callbackEnableTouch), delay);
}

float DouDiZhu::sortPokerSprite(PlayerDDZ *player, float delay, bool callLogic)
{
    player->sortPokerList();
    
    int len = player->getPokerCount();
    auto list = *(player->getPokerList());
    if (player->getSide() != SIDE_DOWN) {
//        for (int i = 0; i < len; i++) {
//            auto poker = list.at(i);
//            poker->setVisible(false);
//        }
//        logic();
        if (true == callLogic) {
            delayCallLogic(player->getSprite(), delay);
        }
        return delay;
    }
    Size size;
    if (list.size() > 0) {
        size = list.at(0)->getSizeScale();
    } else {
        size = pack.at(0)->getSizeScale();
    }
    float hwidth = (FULL_WIDTH-size.width) / (len-1);
    if (hwidth > size.width/3*2) {
        hwidth = size.width/3*2;
    }
    float startx = HALF_WIDTH - (hwidth*(len-1)+size.width)/2;
    for (int i = 0; i < len; i++) {
        auto poker = list.at(i);
        poker->setTag(i);
        Point pos = Point(startx + size.width/2 + i * hwidth, Y_DOWN_CARD_UNPRESS);
        this->moveAnimation(poker, pos, delay, 0.5);
    }
    if (true == callLogic) {
        delay += 1;
        delayCallLogic(player->getSprite(), delay);
    }
    return delay;
}

float DouDiZhu::delayCallLogic(Sprite *sprite, float delay)
{
    Vector<FiniteTimeAction *> list;
    FiniteTimeAction *action;
    action = DelayTime::create(delay);
    list.pushBack(action);
    action = CallFuncN::create(CC_CALLBACK_1(DouDiZhu::callbackLogic, this));
    list.pushBack(action);
    action = Sequence::create(list);
    sprite->runAction(action);
    
    return delay;
}

void DouDiZhu::sortPlaySprite(Vector<PokerDDZ *> list)
{
    for (int i = 0; i < list.size(); i++) {
        auto poker = list.at(i);
        this->reorderChild(poker, PD_ZORDER_NORMAL+i);
    }
}

float DouDiZhu::moveAnimation(Sprite *sprite, Point targetPos, float delay, float time)
{
    Vector<FiniteTimeAction *> list;
    FiniteTimeAction *action;
    action = DelayTime::create(delay);
    list.pushBack(action);
    action = CallFuncN::create(CC_CALLBACK_1(DouDiZhu::callback_move, this));
    list.pushBack(action);
    action = MoveTo::create(time, targetPos);
    list.pushBack(action);
    action = Sequence::create(list);
    sprite->runAction(action);
    
    return delay + time;
}

float DouDiZhu::moveScaleAnimation(Sprite *sprite, Point targetPos, float scaleTo, float delay, float time)
{
    Vector<FiniteTimeAction *> list;
    FiniteTimeAction *action;
    action = DelayTime::create(delay);
    list.pushBack(action);
    Vector<FiniteTimeAction *> listSpawn;
    action = MoveTo::create(time, targetPos);
    listSpawn.pushBack(action);
    action = ScaleTo::create(time, scaleTo);
    listSpawn.pushBack(action);
    action = Spawn::create(listSpawn);
    list.pushBack(action);
    action = Sequence::create(list);
    sprite->runAction(action);
    
    return delay + time;
}

void DouDiZhu::callback_move(Node* pSender)
{
    int tag = pSender->getTag();
    this->reorderChild(pSender, PD_ZORDER_NORMAL+tag);
}

float DouDiZhu::moveSideAnimation(Sprite *sprite, float delay, bool isLeft)
{
    Vector<FiniteTimeAction *> list;
    FiniteTimeAction *action;
    action = DelayTime::create(delay);
    list.pushBack(action);
    if (true == isLeft) {
        action = CallFuncN::create(CC_CALLBACK_1(DouDiZhu::callbackMoveLeftSide, this));
    } else { // right
        action = CallFuncN::create(CC_CALLBACK_1(DouDiZhu::callbackMoveRightSide, this));
    }
    list.pushBack(action);
    action = Sequence::create(list);
    sprite->runAction(action);
    
    return delay + 0.05;
}

void DouDiZhu::callbackMoveLeftSide(Node* pSender)
{
    PlayerDDZ *player = getPlayer(SIDE_LEFT);
    PokerDDZ *poker = (PokerDDZ *)pSender;
    poker->setVisible(false); // for sortPokerSprite
    poker->setPosition(Point(HALF_WIDTH/2, HALF_HEIGHT));
//    poker->setVisible(false);
    player->insertPoker(poker);
    updatePlayerPokerCount(player, countLeft);
}

void DouDiZhu::callbackMoveRightSide(Node* pSender)
{
    PlayerDDZ *player = getPlayer(SIDE_RIGHT);
    PokerDDZ *poker = (PokerDDZ *)pSender;
    poker->setVisible(false); // for sortPokerSprite
    poker->setPosition(Point(FULL_WIDTH/4*3, HALF_HEIGHT));
    poker->setVisible(false);
    player->insertPoker(poker);
    updatePlayerPokerCount(player, countRight);
}

void DouDiZhu::saveGame()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    for (int i = 0; i < playerList.size(); i++) {
        auto player = playerList.at(i);
        int pid = player->getPid();
        //        int side = player->getSide();
        int score = player->getCoin();
        int matchWinCount = player->getMatchWinCount();
        int matchCount = player->getMatchCount();
        
        KUtilIOS::saveIntValueByKey("Player", "normal_match_score", score, pid);
        KUtilIOS::saveIntValueByKey("Player", "normal_match_win_count", matchWinCount, pid);
        KUtilIOS::saveIntValueByKey("Player", "normal_match_count", matchCount, pid);
    }
#else
    int winCount = 0;
    int loseCount = 0;
    auto path = FileUtils::getInstance()->getWritablePath() + SAVE_DATA_1;
    ValueMap loadData = FileUtils::getInstance()->getValueMapFromFile(path);
    ValueMap::iterator iter = loadData.find("WinCount");
    if (iter != loadData.end()) {
        winCount = loadData.at("WinCount").asInt();
    }
    iter = loadData.find("LoseCount");
    if (iter != loadData.end()) {
        loseCount = loadData.at("LoseCount").asInt();
    }
    
    ValueMap saveData;
    for (int i = 0; i < playerList.size(); i++) {
        auto player = playerList.at(i);
        
        char key[20] = {0};
        memset(key, 0, 20);
        sprintf(key, "%d", player->getPid());
        char v[200] = {0};
        memset(v, 0, 200);
        sprintf(v, "%s %d %d", player->getName().c_str(), player->getIcon(), player->getCoin());
        Value value = Value(v);
        saveData.insert(make_pair(key, value));
    }
    
    if (winSide == SIDE_DOWN) {
        winCount++;
    } else if (getPlayer(winSide)->getIsHost() == getPlayer(SIDE_DOWN)->getIsHost()) {
        winCount++;
    } else {
        loseCount++;
    }
    saveData.insert(make_pair("WinCount", Value(winCount)));
    saveData.insert(make_pair("LoseCount", Value(loseCount)));
    
//    auto path = FileUtils::getInstance()->getWritablePath() + SAVE_DATA_1;
    FileUtils::getInstance()->writeToFile(saveData, path);
#endif
}

bool DouDiZhu::loadGame()
{
    return false;
}

void DouDiZhu::pokerUp(PokerDDZ *poker, bool isSetUp)
{
    if (isSetUp == poker->isPress()) {
        return;
    }
    if (isSetUp) {
        poker->setPositionY(Y_DOWN_CARD_PRESS);
    } else {
        poker->setPositionY(Y_DOWN_CARD_UNPRESS);
    }
    updatePlayList(poker, isSetUp);
    poker->setPress(isSetUp);
}

void DouDiZhu::resetPlayList()
{
    for (int i = 0; i < playList.size(); i++) {
        auto poker = playList.at(i);
        poker->setPositionY(Y_DOWN_CARD_UNPRESS);
        poker->setPress(false);
    }
    playList.clear();
}

void DouDiZhu::checkTouchPoker(Touch* touch)
{
    PlayerDDZ *playerDown = DouDiZhu::getPlayer(SIDE_DOWN);
    auto list = *(playerDown->getPokerList());
    for (int i = (int)list.size()-1; i >= 0; i--) {
        auto poker = list.at(i);
        if (!poker->isInRect(touch)) {
            continue;
        }
        if (poker == lastTouchPoker) {
            break;
        }
        bool isPress = poker->isPress();
        pokerUp(poker, !isPress);
        lastTouchPoker = poker;
        break;
    }
}

void DouDiZhu::updatePlayerPokerCount(PlayerDDZ *player, Label *label)
{
    char count[5];
    memset(count, 0, 5);
    sprintf(count, "%d", player->getPokerCount());
    label->setString(count);
}

void DouDiZhu::printCardTypeLog()
{
#ifdef SHOW_LOG
    logCount++;
    if (logCount > 6) {
        llog->setString("");
        logCount = 0;
    }
    char ctName[50] = {0};
    Ai::getCardTypeName(ctName, 50, lastCardType);
    char numName[50] = {0};
    Ai::getCardTypeNumName(numName, 50, lastNum);
    char sSide[20] = {0};
    memset(sSide, 0, 20);
    switch (lastPlaySide) {
        case SIDE_DOWN:
            sprintf(sSide, "%s", "下方");
            break;
        case SIDE_RIGHT:
            sprintf(sSide, "%s", "右方");
            break;
        case SIDE_LEFT:
            sprintf(sSide, "%s", "左方");
            break;
            
        default:
            sprintf(sSide, "%s", "未知");
            break;
    }
    char logs[1000] = {0};
    memset(logs, 0, 1000);
    sprintf(logs, "上手:玩家[%s]型[%s],数[%s],量[%d]\n\n%s", sSide, ctName, numName, lastPlayListSize, llog->getString().c_str());
    llog->setString(logs);
#endif
}

DouDiZhu::DouDiZhu():lastTouchPoker(NULL)
{
    
}

DouDiZhu::~DouDiZhu()
{
//    CC_SAFE_DELETE(playerDown);
//    CC_SAFE_DELETE(playerLeft);
//    CC_SAFE_DELETE(playerRight);
}

void DouDiZhu::onEnter()
{
    Layer::onEnter();
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(DouDiZhu::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(DouDiZhu::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(DouDiZhu::onTouchEnded, this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void DouDiZhu::onExit()
{
    Layer::onExit();
}

bool DouDiZhu::onTouchBegan(Touch* touch, Event* event)
{
    if (false == isCanTouch) {
        return true;
    }
    checkTouchPoker(touch);
    return true;
}

void DouDiZhu::onTouchMoved(Touch* touch, Event* event)
{
    if (false == isCanTouch) {
        return;
    }
    checkTouchPoker(touch);
}

void DouDiZhu::onTouchEnded(Touch* touch, Event* event)
{
    if (false == isCanTouch) {
        return;
    }
    lastTouchPoker = NULL;
}


#pragma mark ResultLayer start
bool ResultLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    auto layerColor = LayerColor::create(Color4B(0, 0, 0, 200), FULL_WIDTH, FULL_HEIGHT);
    layerColor->setPosition(Point(0, 0));
    this->addChild(layerColor);
    
    Vector<MenuItem *> itemArray;
    
    GameTool::addBtn2(&itemArray, "继续", 32, Point(HALF_WIDTH, Y_DOWN_CARD_UNPRESS), ANCHOR_CENTER_DOWN, CC_CALLBACK_1(ResultLayer::callbackNext, this));
    
    KUtil::addMenu(this, &itemArray, 30);
    
    return true;
}

void ResultLayer::setData(int coin, int winSide, Vector<PlayerDDZ *> playerList, const ccMenuCallback& callback)
{
    _callback = callback;
    bool hostWin = false;
    for (int i = 0; i < playerList.size(); i++) {
        auto player = playerList.at(i);
        if (player->getSide() == winSide && true == player->getIsHost()) {
            hostWin = true;
            break;
        }
    }
    for (int i = 0; i < playerList.size(); i++) {
        auto player = playerList.at(i);
        if (player->getIcon() < 0 || player->getIcon() > 2) {
            CCLOG("ERROR DouDiZhu::addPlayer iconType icon(%d)", player->getIcon());
            continue;
        }
        char iconType[20] = {0};
        memset(iconType, 0, 20);
        Point position = Point(200, Y_DOWN_CARD_UNPRESS+180+180*i);
        sprintf(iconType, "icon_%d.png", player->getIcon());
        auto frame = KUtil::addSprite(this, KUtil::getPath(F_FRAME, "frame_icon.png"), position, ANCHOR_LEFT_DOWN, 10);
        auto icon = KUtil::addSprite(frame, KUtil::getPath(F_ICON, iconType), Point(61, 72), ANCHOR_CENTER_CENTER, -1);
        icon->setScale(0.95);
        GameTool::addLabelOutlineDefault(frame, player->getName(), 30, Point(150, 0), Color4B(0, 0, 0, 255), Color4B(255, 255, 255, 255), 2, ANCHOR_LEFT_DOWN, 1);
        bool isHost = false;
        if (true == player->getIsHost()) {
            isHost = true;
        }
        char fname[50] = {0};
        memset(fname, 0, 50);
        if (true == isHost) {
            sprintf(fname, "%s", "flag_host.png");
        } else {
            sprintf(fname, "%s", "flag_farmer.png");
        }
        KUtil::addSprite(frame, KUtil::getPath(F_FRAME, fname), Point(0, 0), ANCHOR_CENTER_DOWN, 1);
        
        Color4B color;
        char winStr[50] = {0};
        memset(winStr, 0, 50);
        if ((true == hostWin && true == isHost) || (false == hostWin && false == isHost)) {
            sprintf(winStr, "%s", "胜利!!!");
            color = Color4B(0, 255, 0, 255);
        } else {
            sprintf(winStr, "%s", "失败!!!");
            color = Color4B(200, 200, 200, 255);
        }
        auto win = GameTool::addLabelOutlineDefault(frame, winStr, 50, Point(360, 0), color, Color4B(255, 255, 255, 255), 2, ANCHOR_LEFT_DOWN, 10);
        win->setRotation(-20);
        
        auto iconCoin = KUtil::addSprite(frame, KUtil::getPath(F_FRAME, "icon_coin.png"), Point(460, 0), ANCHOR_LEFT_DOWN, 0);
        iconCoin->setScale(2);
        
        char numStr[50] = {0};
        memset(numStr, 0, 50);
        if ((true == hostWin && true == isHost) || (false == hostWin && false == isHost)) {
            sprintf(numStr, "+%d", (true==isHost)?(coin*2):coin);
            color = Color4B(255, 255, 0, 255);
            player->addCoin((true==isHost)?(coin*2):coin);
        } else {
            sprintf(numStr, "-%d", (true==isHost)?(coin*2):coin);
            color = Color4B(200, 200, 200, 255);
            player->addCoin(-((true==isHost)?(coin*2):coin));
        }
        GameTool::addLabelOutlineDefault(frame, numStr, 50, Point(540, 0), color, Color4B(255, 255, 255, 255), 2, ANCHOR_LEFT_DOWN, 10);
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
        if (player->getSide() == SIDE_DOWN) {
            KUtilIOS::insertRank(IOS_KEY_SCORE_RANK, player->getCoin());
        }
#endif
    }
}

void ResultLayer::callbackNext(cocos2d::Ref* pSender)
{
    if (nullptr == _callback) {
        return;
    }
    _callback(nullptr);
//    removeFromParentAndCleanup(true);
    KUtil::removeLayer(this);
}

ResultLayer::ResultLayer()
{
    
}

ResultLayer::~ResultLayer()
{
    
}

void ResultLayer::onEnter()
{
    Layer::onEnter();
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(ResultLayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(ResultLayer::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(ResultLayer::onTouchEnded, this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void ResultLayer::onExit()
{
    Layer::onExit();
}

bool ResultLayer::onTouchBegan(Touch* touch, Event* event)
{
    return true;
}

void ResultLayer::onTouchMoved(Touch* touch, Event* event)
{
    
}

void ResultLayer::onTouchEnded(Touch* touch, Event* event)
{
    
}
#pragma mark ResultLayer end


