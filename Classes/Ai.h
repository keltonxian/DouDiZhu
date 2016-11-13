//
//  Ai.h
//  DouDiZhu
//
//  Created by keltonxian on 1/28/15.
//
//

#ifndef __DouDiZhu__Ai__
#define __DouDiZhu__Ai__

#include "KHeader.h"
#include "PokerDDZ.h"

class CardType : public Ref {
public:
    static CardType* create(int cardType, int num);
    bool isIllegeal();
    bool isNone();
    bool isRocket();
    int getCardType();
    int getNum();
protected:
    CardType(int cardType, int num);
    virtual ~CardType();
private:
    int cardType;
    int num;
};

class TmpFaceCard : public Ref {
public:
    static TmpFaceCard* create(PokerDDZ *poker);
    int getNum();
    Vector<PokerDDZ *>* getCardList();
protected:
    void init(PokerDDZ *poker);
    TmpFaceCard();
    virtual ~TmpFaceCard();
private:
    int num;
    Vector<PokerDDZ *> *cardList;
};

class CardList : public Ref {
public:
    static CardList* create(Vector<PokerDDZ *> cards, int naizi);
    int getNaiziNum();
    int getCardNum();
    int getNaizi();
    ValueVector* getFirstList();
    ValueVector* getSecondList();
    ValueVector* getThirdList();
    ValueVector* getFourthList();
    int getFirstList(int index);
    int getSecondList(int index);
    int getThirdList(int index);
    int getFourthList(int index);
    bool hasJoker();
    bool hasNum(int num, ValueVector *list);
protected:
    CardList();
    virtual ~CardList();
private:
    int naizi;
    int cardNum; // total poker in hand
    int naiziNum; 
    void init(Vector<PokerDDZ *> cards, int naizi);
    void init(ValueMapIntKey *map, ValueVector *cards);
    ValueVector* firstList;
    ValueVector* secondList;
    ValueVector* thirdList;
    ValueVector* fourthList;
    static int compare(Value v1, Value v2);
    void sort(ValueVector *cards);
};

class Ai {
public:
    static const int CARD_TYPE_ILLEGAL = 0;
    static const int CARD_TYPE_NONE	= 1;
    static const int CARD_TYPE_SINGLE = 2;	             // 单张
    static const int CARD_TYPE_PAIR	= 3;		         // 对子
    static const int CARD_TYPE_TRIAD = 4;		         // 三张
    static const int CARD_TYPE_TRIAD_SINGLE	= 5;		 // 三带一
    static const int CARD_TYPE_TRIAD_PAIR = 6;		     // 三带一对
    static const int CARD_TYPE_QUAD_TWO	= 7;			 // 四带两个
    static const int CARD_TYPE_QUAD_TWO_PAIR = 8;	     // 四带两对
    static const int CARD_TYPE_STRAIGHT	= 9;		     // 顺子
    static const int CARD_TYPE_DOUBLE_STRAIGHT = 10;     // 连对
    static const int CARD_TYPE_TRIAD_STRAIGHT = 11;      // 飞机不带翅膀
    static const int CARD_TYPE_PLANE = 12;			     // 飞机带单翅膀
    static const int CARD_TYPE_DOUBLE_PLANE = 13;	     // 飞机带双翅膀
    static const int CARD_TYPE_BOMB = 14;			     // 炸弹
    static const int CARD_TYPE_ROCKET = 15;		         // 火箭
    static const int CARD_TYPE_BOMB_Y = 16;	             // 纯癞子炸弹
    static const int CARD_TYPE_BOMB_SOFT = 17;	         // 软炸弹
    static int choosePoker(Vector<PokerDDZ *> myPokerList, Vector<PokerDDZ *> pokerList, CardType *lastCardType, int lastPlayListSize, int aiLevel, Vector<PokerDDZ *> *tipPokerList);
    static void chooseSingle(Vector<PokerDDZ *> *tipPokerList, int lastNum, Vector<TmpFaceCard *> *list);
    static void choosePair(Vector<PokerDDZ *> *tipPokerList, int lastNum, Vector<TmpFaceCard *> *list);
    static void chooseTriad(Vector<PokerDDZ *> *tipPokerList, int lastNum, Vector<TmpFaceCard *> *list);
    static void chooseTriadSingle(Vector<PokerDDZ *> *tipPokerList, int lastNum, Vector<TmpFaceCard *> *list);
    static void chooseTriadPair(Vector<PokerDDZ *> *tipPokerList, int lastNum, Vector<TmpFaceCard *> *list);
    static void chooseStraight(Vector<PokerDDZ *> *tipPokerList, int lastCardNum, Vector<TmpFaceCard *> *list, int lastPlayListSize, int num);
    static void choosePlane(Vector<PokerDDZ *> *tipPokerList, int lastCardNum, Vector<TmpFaceCard *> *list, int lastNum);
    static void chooseDoublePlane(Vector<PokerDDZ *> *tipPokerList, int lastCardNum, Vector<TmpFaceCard *> *list, int lastNum);
    static void chooseRocket(Vector<PokerDDZ *> *tipPokerList, Vector<TmpFaceCard *> *list);
    static void chooseBomb(Vector<PokerDDZ *> *tipPokerList, int lastNum, Vector<TmpFaceCard *> *list);
    static void copyAndClean(TmpFaceCard *tfc, Vector<PokerDDZ *> *dst, int len);
    static CardType* cardTypeNull();
    static CardType* cardTypeIllegal();
    static CardType* validCardsAndGetCardType(CardType *lastCardType, int lastPlayListSize, Vector<PokerDDZ *> pokerList, int naizi);
    static CardType* validCardsAndGetCardTypeNormal(CardType *lastCardType, int lastPlayListSize, CardList *cardList);
    static CardType* getCardType(Vector<PokerDDZ *> pokerList);
    static CardType* getNormalCardType(CardList *cards);
    static bool checkContinus(ValueVector *list);
    static void copyValueVector(ValueVector *src, ValueVector *dst, int from, int end);
    static void getCardTypeName(char *name, int nameSize, int cardtype);
    static void getCardTypeNumName(char *name, int nameSize, int cardtypenum);
};

#endif /* defined(__DouDiZhu__Ai__) */
