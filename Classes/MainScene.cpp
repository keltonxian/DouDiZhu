//
//  MainScene.cpp
//  KillTime
//
//  Created by keltonxian on 7/11/14.
//
//

#include "MainScene.h"
#include "DouDiZhu.h"
#include "GameTool.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <platform/android/jni/JniHelper.h>
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#import "KUtilIOS.h"
#endif

Scene* MainScene::createScene()
{
    auto scene = Scene::create();
    auto layer = MainScene::create();
    scene->addChild(layer);
    return scene;
}

bool MainScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    auto bg = KUtil::addSprite(this, KUtil::getPath(F_BG, "bg_3.png"), Point(HALF_WIDTH, 0), ANCHOR_CENTER_DOWN, 0);
    bg->setScale(KUtil::getScale(FULL_WIDTH, FULL_HEIGHT, bg->getContentSize(), true));
    
    Vector<MenuItem *> itemArray;
    
    GameTool::addBtn2(&itemArray, "进入", 32, Point(HALF_WIDTH, HALF_HEIGHT), ANCHOR_CENTER_CENTER, CC_CALLBACK_1(MainScene::callbackRandomMatch, this));
    
    GameTool::addBtn2(&itemArray, "排行榜", 32, Point(FULL_WIDTH, FULL_HEIGHT), ANCHOR_RIGHT_UP, CC_CALLBACK_1(MainScene::callbackRank, this));
    
    KUtil::addMenu(this, &itemArray, 30);
    
    initPlayerData();
    initMyData();
    
    return true;
}

void MainScene::addPlayer(const char *altas, int iconIndex, int objIndex)
{
    bool isPlayerExist = KUtilIOS::checkEntityExistByIndex("Player", objIndex);
    if (true == isPlayerExist) {
        return;
    }
    KUtilIOS::saveStringValueByKey("Player", "atlas", altas, objIndex);
    KUtilIOS::saveIntValueByKey("Player", "icon", iconIndex, objIndex);
    KUtilIOS::saveIntValueByKey("Player", "normal_match_score", 0, objIndex);
    KUtilIOS::saveIntValueByKey("Player", "normal_match_win_count", 0, objIndex);
    KUtilIOS::saveIntValueByKey("Player", "normal_match_count", 0, objIndex);
}

void MainScene::initPlayerData()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    addPlayer("潜力股", 0, 0);
    addPlayer("婕婕姐", 1, 1);
    addPlayer("冬瓜哥", 2, 2);
#else
    auto p = FileUtils::getInstance()->getWritablePath() + SAVE_DATA_1;
    ValueMap sd = FileUtils::getInstance()->getValueMapFromFile(p);
    CCLOG("initPlayerData data size[%zd]", sd.size());
    if (sd.size() >= 3) {
        return;
    }
    ValueMap saveData;
    saveData.insert(make_pair("1", Value("潜力股 0 800")));
    saveData.insert(make_pair("2", Value("婕婕姐 1 800")));
    saveData.insert(make_pair("3", Value("冬瓜哥 2 800")));
//    string("abc");
//    saveData.insert(make_pair("1", Value(string("越秀山凯尔顿 1 800"))));
////    saveData.insert(make_pair("1", Value("kelton 1 800")));
//    saveData.insert(make_pair("2", Value("natassa 2 800")));
//    saveData.insert(make_pair("3", Value("donggua 3 800")));
    
    auto path = FileUtils::getInstance()->getWritablePath() + SAVE_DATA_1;
    FileUtils::getInstance()->writeToFile(saveData, path);
#endif
}

void MainScene::initMyData()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    int winMatchCount = 0;
    int matchCount = 0;
    int countWin1 = KUtilIOS::getIntValueByKey("Player", "normal_match_win_count", 0, 0);
    winMatchCount = countWin1;
    int count1 = KUtilIOS::getIntValueByKey("Player", "normal_match_count", 0, 0);
    matchCount = count1;
    std::string atlas = KUtilIOS::getStringValueByKey("Player", "atlas", "小赌神", 0);
    int iconIndex = KUtilIOS::getIntValueByKey("Player", "icon", 0, 0);
    int normalMatchCoin = KUtilIOS::getIntValueByKey("Player", "normal_match_score", 0, 0);
    int matchCoin = normalMatchCoin;
#else
    int winMatchCount = 0;
    int loseCount = 0;
    auto path = FileUtils::getInstance()->getWritablePath() + SAVE_DATA_1;
    ValueMap data = FileUtils::getInstance()->getValueMapFromFile(path);
    ValueMap::iterator iter = data.find("WinCount");
    if (iter != data.end()) {
        winMatchCount = data.at("WinCount").asInt();
    }
    iter = data.find("LoseCount");
    if (iter != data.end()) {
        loseCount = data.at("LoseCount").asInt();
    }
    int matchCount = winMatchCount + loseCount;
    
    char atlas[100] = {0};
    memset(atlas, 0, 100);
    int iconIndex = 1;
    int matchCoin = 0;
    
    iter = data.find("1");
    if (iter != data.end()) {
//        CCLOG("a:[%s]", data.at("1").asString().c_str());
        std::string s = data.at("1").asString();
//        const char *v = data.at("1").asString().c_str();
    //        CCLOG("v:[%s]", v);
//        int ret = sscanf(v, "%s %d %d", name, &icon, &coin);
        CCLOG("v:[%s]", s.c_str());
        int ret = sscanf(s.c_str(), "%s %d %d", atlas, &iconIndex, &matchCoin);
        if (3 != ret) {
            sprintf(atlas, "me");
            iconIndex = 1;
            matchCoin = 0;
        }
    }
#endif
    
    GameTool::addTextBg(this, Size(FULL_WIDTH, 100), Point(0, 0), ANCHOR_LEFT_DOWN, 10);
    int x = 50;
    int y = 30;
    char iconType[20] = {0};
    memset(iconType, 0, 20);
    sprintf(iconType, "icon_%d.png", iconIndex);
    auto frame = KUtil::addSprite(this, KUtil::getPath(F_FRAME, "frame_icon.png"), Point(x, y), ANCHOR_LEFT_DOWN, 20);
    auto iconSprite = KUtil::addSprite(frame, KUtil::getPath(F_ICON, iconType), Point(61, 72), ANCHOR_CENTER_CENTER, -1);
    iconSprite->setScale(0.95);
    x += 140;
    GameTool::addLabelOutlineDefault(this, atlas, 30, Point(x, y), Color4B(0, 0, 0, 255), Color4B(255, 255, 255, 255), 2, ANCHOR_LEFT_DOWN, 20);
    
    x = FULL_WIDTH - 800;
    GameTool::addLabelOutlineDefault(this, "统计:", 35, Point(x, y), Color4B(200, 150, 200, 255), Color4B(0, 0, 0, 255), 2, ANCHOR_LEFT_DOWN, 20);
    
    x += 110;
    char str[500] = {0};
    memset(str, 0, 500);
    sprintf(str, "%d(胜)/%d(总)", winMatchCount, matchCount);
    GameTool::addLabelOutlineDefault(this, str, 35, Point(x, y), Color4B(200, 150, 0, 255), Color4B(0, 0, 0, 255), 2, ANCHOR_LEFT_DOWN, 20);
    
    x = FULL_WIDTH - 300;
    KUtil::addSprite(this, KUtil::getPath(F_FRAME, "icon_coin.png"), Point(x, y), ANCHOR_LEFT_DOWN, 20);
    x += 50;
    char coinStr[20] = {0};
    memset(coinStr, 0, 20);
    sprintf(coinStr, "%d", matchCoin);
    GameTool::addLabelOutlineDefault(this, coinStr, 40, Point(x, y), Color4B(255, 255, 0, 255), Color4B(0, 0, 0, 255), 2, ANCHOR_LEFT_DOWN, 20);
}

void MainScene::onEnter()
{
    Layer::onEnter();
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(MainScene::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(MainScene::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(MainScene::onTouchEnded, this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    auto keyPadListener = EventListenerKeyboard::create();
    keyPadListener->onKeyPressed = CC_CALLBACK_2(MainScene::onKeyPressed, this);
    keyPadListener->onKeyReleased = CC_CALLBACK_2(MainScene::onKeyReleased, this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyPadListener, this);
}

void MainScene::onExit()
{
    Layer::onExit();
}

void MainScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
//    CCLOG("onKeyPressed");
//    CCLOG("onKeyPressed keyCode[%d]", keyCode);
}

void MainScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    if (EventKeyboard::KeyCode::KEY_BACK == keyCode) {
        JniMethodInfo t;
        
        if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/AppActivity", "exitGame", "()V"))
        {
            t.env->CallStaticVoidMethod(t.classID, t.methodID);
            t.env->DeleteLocalRef(t.classID);
        }
    }
#endif
//    CCLOG("onKeyReleased keyCode[%d]", keyCode);
}

bool MainScene::onTouchBegan(Touch* touch, Event* event)
{
//    CCLOG("onTouchBegan");
    return true;
}

void MainScene::onTouchMoved(Touch* touch, Event* event)
{
    
}

void MainScene::onTouchEnded(Touch* touch, Event* event)
{
    
}

void MainScene::callbackRandomMatch(cocos2d::Ref* pSender)
{
    Director::getInstance()->replaceScene(TransitionSlideInT::create(0.4, DouDiZhu::createScene()));
}

void MainScene::callbackRank(cocos2d::Ref* pSender)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    KUtilIOS::showRank(IOS_KEY_SCORE_RANK);
#endif
}


