//
//  PlayerDDZ.cpp
//  KillTime
//
//  Created by keltonxian on 8/31/14.
//
//

#include "PlayerDDZ.h"

PlayerDDZ* PlayerDDZ::createPlayer(int pid, int side, std::string name, int icon, bool isAi, int coin, int matchWinCount, int matchCount)
{
    PlayerDDZ *player = new PlayerDDZ();
    player->autorelease();
    player->_pid = pid;
    player->_side = side;
    player->_name = name;
    player->_icon = icon;
    player->_isAi = isAi;
    player->_isHost = false;
    player->_isCallHost = true; // is case of skip call to grab
    player->_coin = coin;
    player->_matchWinCount = matchWinCount;
    player->_matchCount = matchCount;
    return player;
}

int PlayerDDZ::getPid()
{
    return _pid;
}

int PlayerDDZ::getSide()
{
    return _side;
}

std::string PlayerDDZ::getName()
{
    return _name;
}

int PlayerDDZ::getIcon()
{
    return _icon;
}

bool PlayerDDZ::isAi()
{
    return _isAi;
}

void PlayerDDZ::setSprite(Sprite *sprite)
{
    _sprite = sprite;
}

Sprite* PlayerDDZ::getSprite()
{
    return _sprite;
}

PlayerDDZ::PlayerDDZ():_pokerList(NULL)
{
    _pokerList = new Vector<PokerDDZ *>();
}

PlayerDDZ::~PlayerDDZ()
{
    CC_SAFE_DELETE(_pokerList);
}

Vector<PokerDDZ *>* PlayerDDZ::getPokerList()
{
    return _pokerList;
}

void PlayerDDZ::insertPoker(PokerDDZ* poker)
{
    (*_pokerList).pushBack(poker);
}

void PlayerDDZ::insertPoker(PokerDDZ* poker, int pos)
{
    if (pos >= (*_pokerList).size()) {
        insertPoker(poker);
        return;
    }
    (*_pokerList).insert(pos, poker);
}

void PlayerDDZ::removePoker(PokerDDZ* poker)
{
    (*_pokerList).eraseObject(poker);
}

void PlayerDDZ::cleanPokerList()
{
    (*_pokerList).clear();
}

int PlayerDDZ::getPokerCount()
{
    return (int)((*_pokerList).size());
}

bool PlayerDDZ::DDZPokerListComparisonLess(PokerDDZ* p1, PokerDDZ* p2)
{
    int p1Num = p1->getNum();
    int p1Face = p1->getFace();
    int p2Num = p2->getNum();
    int p2Face = p2->getFace();
    if (p1Num > p2Num) {
        return true;
    }
    if (p1Num == p2Num) {
        if (p1Face > p2Face) {
            return true;
        }
    }
    return false;
}

void PlayerDDZ::sortPokerList()
{
//    auto list = *(player->getPokerList());
    //    int listLen = player->getPokerCount();
    std::sort( std::begin(*_pokerList), std::end(*_pokerList), PlayerDDZ::DDZPokerListComparisonLess );
}

void PlayerDDZ::printPoker()
{
    for (int i = 0; i < (*_pokerList).size(); i++) {
        auto p = (*_pokerList).at(i);
        CCLOG("player[%s] p[%d,%d]", _name.c_str(), p->getFace(), p->getNum());
    }
}

void PlayerDDZ::setCallHost(bool isCallHost)
{
    _isCallHost = isCallHost;
}

bool PlayerDDZ::getCallHost()
{
    return _isCallHost;
}

void PlayerDDZ::setGrabHost(bool isGrabHost)
{
    _isGrabHost = isGrabHost;
}

bool PlayerDDZ::getGrabHost()
{
    return _isGrabHost;
}

void PlayerDDZ::setIsHost(bool isHost)
{
    _isHost = isHost;
}

bool PlayerDDZ::getIsHost()
{
    return _isHost;
}

void PlayerDDZ::addCoin(int coin)
{
    _coin += coin;
}

int PlayerDDZ::getCoin()
{
    return _coin;
}

void PlayerDDZ::setMatchWinCount(int matchWinCount)
{
    _matchWinCount = matchWinCount;
}

int PlayerDDZ::getMatchWinCount()
{
    return _matchWinCount;
}

void PlayerDDZ::setMatchCount(int matchCount)
{
    _matchCount = matchCount;
}

int PlayerDDZ::getMatchCount()
{
    return _matchCount;
}



