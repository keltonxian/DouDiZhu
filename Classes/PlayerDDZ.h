//
//  PlayerDDZ.h
//  KillTime
//
//  Created by keltonxian on 8/31/14.
//
//

#ifndef __KillTime__PlayerDDZ__
#define __KillTime__PlayerDDZ__

#include "KHeader.h"

#include "PokerDDZ.h"

class PlayerDDZ : public Ref {
public:
    static PlayerDDZ* createPlayer(int pid, int side, std::string name, int icon, bool isAi, int coin);
    static bool DDZPokerListComparisonLess(PokerDDZ* p1, PokerDDZ* p2);
    int getPid();
    int getSide();
    std::string getName();
    int getIcon();
    bool isAi();
    void setSprite(Sprite *sprite);
    Sprite* getSprite();
    Vector<PokerDDZ *>* getPokerList();
    void insertPoker(PokerDDZ* poker);
    void insertPoker(PokerDDZ* poker, int pos);
    void removePoker(PokerDDZ* poker);
    void cleanPokerList();
    int getPokerCount();
    void sortPokerList();
    void setCallHost(bool isCallHost);
    bool getCallHost();
    void setGrabHost(bool isGrabHost);
    bool getGrabHost();
    void setIsHost(bool isHost);
    bool getIsHost();
    void addCoin(int coin);
    int getCoin();
    PlayerDDZ();
    virtual ~PlayerDDZ();
    void printPoker();
private:
    int _coin;
    bool _isHost;
    bool _isCallHost;
    bool _isGrabHost;
    int _side;
    Sprite *_sprite;
    bool _isAi;
    std::string _name;
    int _pid;
    int _icon;
    Vector<PokerDDZ *> *_pokerList;
};

#endif /* defined(__KillTime__PlayerDDZ__) */
