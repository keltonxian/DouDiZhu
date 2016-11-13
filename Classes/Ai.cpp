//
//  Ai.cpp
//  DouDiZhu
//
//  Created by keltonxian on 1/28/15.
//
//

#include "Ai.h"

#pragma mark CardType start
CardType* CardType::create(int cardType, int num)
{
    CardType *ct = new CardType(cardType, num);
    if (ct)
    {
        ct->autorelease();
        return ct;
    }
    CC_SAFE_DELETE(ct);
    return NULL;
}

CardType::CardType(int cardType, int num):cardType(cardType), num(num)
{
    
}

CardType::~CardType()
{
    
}

bool CardType::isIllegeal()
{
    return this->cardType == Ai::CARD_TYPE_ILLEGAL;
}

bool CardType::isNone()
{
    return this->cardType == Ai::CARD_TYPE_NONE;
}

bool CardType::isRocket()
{
    return this->cardType == Ai::CARD_TYPE_ROCKET;
}

int CardType::getCardType()
{
    return this->cardType;
}

int CardType::getNum()
{
    return this->num;
}

#pragma mark CardType end
#pragma mark TmpFaceCard start
TmpFaceCard* TmpFaceCard::create(PokerDDZ *poker)
{
    TmpFaceCard *tempFaceCard = new TmpFaceCard();
    if (tempFaceCard)
    {
        tempFaceCard->init(poker);
        tempFaceCard->autorelease();
        return tempFaceCard;
    }
    CC_SAFE_DELETE(tempFaceCard);
    return NULL;
}

void TmpFaceCard::init(PokerDDZ *poker)
{
    this->num = poker->getNum();
    (*this->cardList).pushBack(poker);
}

int TmpFaceCard::getNum()
{
    return this->num;
}

Vector<PokerDDZ *>* TmpFaceCard::getCardList()
{
    return this->cardList;
}

TmpFaceCard::TmpFaceCard():num(0)
{
    this->cardList = new Vector<PokerDDZ *>();
}

TmpFaceCard::~TmpFaceCard()
{
    CC_SAFE_DELETE(this->cardList);
}
#pragma mark TmpFaceCard end
#pragma mark CardList start
CardList* CardList::create(Vector<PokerDDZ *> cards, int naizi)
{
    CardList *cardList = new CardList();
    if (cardList)
    {
        cardList->init(cards, naizi);
        cardList->autorelease();
        return cardList;
    }
    CC_SAFE_DELETE(cardList);
    return NULL;
}

void CardList::init(Vector<PokerDDZ *> cards, int naizi)
{
    size_t size = cards.size();
    this->firstList = new ValueVector();
    this->secondList = new ValueVector();
    this->thirdList = new ValueVector();
    this->fourthList = new ValueVector();
    this->naizi = naizi;
    ValueMapIntKey map;
    CCLOG("CardList::init cardNum[%d] size[%zd]", cardNum, size);
    for (int i = 0; i < size; i++) {
        PokerDDZ *card = cards.at(i);
        int cNum = card->getNum();
        this->cardNum++;
        if (cNum == this->naizi) {
            this->naiziNum++;
            continue;
        }
        Value num;
        ValueMapIntKey::iterator iter = map.find(cNum);
        if (iter == map.end()) {
            num = Value(1);
        } else {
            num = Value(map.at(cNum).asInt() + 1);
        }
        map.erase(cNum);
        map.insert(std::make_pair(cNum, num));
    }
    CCLOG("CardList::init cardNum[%d]", cardNum);
    CardList::init(&map, this->firstList);
    CardList::init(&map, this->secondList);
    CardList::init(&map, this->thirdList);
    CardList::init(&map, this->fourthList);
}

void CardList::init(ValueMapIntKey *map, ValueVector *cards)
{
    if (0 == map->size()) {
        return;
    }
    ValueVector keys;
    ValueMapIntKey::iterator iter = map->begin();
    for (; iter != map->end(); iter++) {
        int num = iter->first;   // key
        keys.push_back(Value(num));
    }
    for (int i = 0; i < keys.size(); i++) {
        int num = keys.at(i).asInt();
        int value = map->at(num).asInt();
//        CCLOG("iter[%d][%d] cards size[%zd]", num, value, cards->size());
        if (value > 0) {
            (*cards).push_back(Value(num));
            map->erase(num);
            map->insert(std::make_pair(num, Value(value-1)));
        }
    }
    sort(cards);
}

int CardList::compare(Value v1, Value v2)
{
    return v1.asInt() < v2.asInt();
}

void CardList::sort(ValueVector *cards)
{
    
    std::sort( std::begin(*cards), std::end(*cards), CardList::compare );
}

CardList::CardList():firstList(nullptr),secondList(nullptr),thirdList(nullptr),fourthList(nullptr),cardNum(0),naiziNum(0)
{
    
}

CardList::~CardList()
{
    CC_SAFE_DELETE(this->firstList);
    CC_SAFE_DELETE(this->secondList);
    CC_SAFE_DELETE(this->thirdList);
    CC_SAFE_DELETE(this->fourthList);
}

int CardList::getNaiziNum()
{
    return this->naiziNum;
}

int CardList::getCardNum()
{
    return this->cardNum;
}

int CardList::getNaizi()
{
    return this->naizi;
}

ValueVector* CardList::getFirstList()
{
    return this->firstList;
}

ValueVector* CardList::getSecondList()
{
    return this->secondList;
}

ValueVector* CardList::getThirdList()
{
    return this->thirdList;
}

ValueVector* CardList::getFourthList()
{
    return this->fourthList;
}

int CardList::getFirstList(int index)
{
    if (firstList == nullptr || 0 == (*firstList).size() || index < 0 || index >= (*firstList).size()) {
        return -1;
    }
    return (*firstList).at(index).asInt();
}

int CardList::getSecondList(int index)
{
    if (secondList == nullptr || 0 == (*secondList).size() || index < 0 || index >= (*secondList).size()) {
        return -1;
    }
    return (*secondList).at(index).asInt();
}

int CardList::getThirdList(int index)
{
    if (thirdList == nullptr || 0 == (*thirdList).size() || index < 0 || index >= (*thirdList).size()) {
        return -1;
    }
    int num = thirdList->at(index).asInt();
//    CCLOG("thirdList index[%d] is [%d]", index, num);
    return num;
}

int CardList::getFourthList(int index)
{
    if (fourthList == nullptr || 0 == (*fourthList).size() || index < 0 || index >= (*fourthList).size()) {
        return -1;
    }
    return (*fourthList).at(index).asInt();
}

bool CardList::hasNum(int num, ValueVector *list)
{
    for (int i = 0; i < (*list).size(); i++) {
        int n = (*list).at(i).asInt();
        if (num == n) {
            return true;
        }
    }
    return false;
}

bool CardList::hasJoker()
{
    if (0 == (*firstList).size()) {
        return false;
    }
    int t = (*firstList).at((*firstList).size()-1).asInt();
    if (t > POKERDDZ_NUM::DDZ_TWO) {
        return true;
    }
    return false;
}
#pragma mark CardList end
#pragma mark Ai start
int Ai::choosePoker(Vector<PokerDDZ *> myPokerList, Vector<PokerDDZ *> pokerList, CardType *lastCardType, int lastPlayListSize, int aiLevel, Vector<PokerDDZ *> *tipPokerList)
{
//    CCLOG("myPokerList[%zd],pokerList[%zd],lastCardType[%d,%d],aiLevel[%d]", myPokerList.size(), pokerList.size(), lastCardType->getCardType(), lastCardType->getNum(), aiLevel);
    if (myPokerList.size() == 0) {
        return 0;
    }
    if (lastCardType->isRocket()) {
        return 0;
    }
    Vector<TmpFaceCard *> list;
    for (int i = 0; i < myPokerList.size(); i++) {
        PokerDDZ *poker = myPokerList.at(i);
        int idx = 0;
        bool exist = false;
        for (int j = 0; j < list.size(); j++) {
            TmpFaceCard *tmp = list.at(j);
            if (tmp->getNum() == poker->getNum()) {
                exist = true;
                (*tmp->getCardList()).pushBack(poker);
                break;
            } else if (tmp->getNum() > poker->getNum()) {
                break;
            } else {
                idx++;
            }
        }
        if (false == exist) {
            list.insert(idx, TmpFaceCard::create(poker));
        }
    }
    if (lastCardType->isIllegeal() || lastCardType->isNone()) {
        Vector<PokerDDZ *> cs = (*(list.at(0))->getCardList());
        for (int i = 0; i < cs.size(); i++) {
            (*tipPokerList).pushBack(cs.at(i));
        }
        return 0;
    }
    tipPokerList->clear();
    bool hasType = true;
    switch (lastCardType->getCardType()) {
        case CARD_TYPE_SINGLE:
            chooseSingle(tipPokerList, lastCardType->getNum(), &list);
            break;
        case CARD_TYPE_PAIR:
            choosePair(tipPokerList, lastCardType->getNum(), &list);
            break;
        case CARD_TYPE_TRIAD:
            chooseTriad(tipPokerList, lastCardType->getNum(), &list);
            break;
        case CARD_TYPE_TRIAD_SINGLE:
            chooseTriadSingle(tipPokerList, lastCardType->getNum(), &list);
            break;
        case CARD_TYPE_TRIAD_PAIR:
            chooseTriadPair(tipPokerList, lastCardType->getNum(), &list);
            break;
        case CARD_TYPE_STRAIGHT:
            chooseStraight(tipPokerList, lastCardType->getNum(), &list, lastPlayListSize, 1);
            break;
        case CARD_TYPE_DOUBLE_STRAIGHT:
            chooseStraight(tipPokerList, lastCardType->getNum(), &list, lastPlayListSize, 2);
            break;
        case CARD_TYPE_TRIAD_STRAIGHT:
            chooseStraight(tipPokerList, lastCardType->getNum(), &list, lastPlayListSize, 3);
            break;
        case CARD_TYPE_PLANE:
            choosePlane(tipPokerList, lastCardType->getNum(), &list, lastPlayListSize);
            break;
        case CARD_TYPE_DOUBLE_PLANE:
            chooseDoublePlane(tipPokerList, lastCardType->getNum(), &list, lastPlayListSize);
            break;
        default:
            hasType = false;
            break;
    }
    if (true == hasType) {
        return 0;
    }
    if (lastCardType->getCardType() == CARD_TYPE_BOMB) {
        Ai::chooseBomb(tipPokerList, lastCardType->getNum(), &list);
    } else if (lastCardType->getCardType() != CARD_TYPE_BOMB_Y) {
        Ai::chooseBomb(tipPokerList, -1, &list);
    }
    if (tipPokerList->size() == 0) {
        Ai::chooseRocket(tipPokerList, &list);
    }
    return 0;
}

void Ai::chooseSingle(Vector<PokerDDZ *> *tipPokerList, int lastNum, Vector<TmpFaceCard *> *list)
{
    int jokerNum = 0;
    int cardTypeNum = 0;
    TmpFaceCard *tfc = NULL;
    for (int i = 0; i < list->size(); i++) {
        TmpFaceCard *card = list->at(i);
        if (card->getNum() >= POKERDDZ_NUM::DDZ_J_B) {
            jokerNum++;
        }
        if (1 == card->getCardList()->size() && card->getNum() > lastNum) {
            copyAndClean(card, tipPokerList, 1);
            return;
        } else if (card->getNum() > lastNum && card->getCardList()->size() < 4 && tfc == NULL) {
            tfc = card;
        }
        cardTypeNum++;
    }
    if (jokerNum == 2 && cardTypeNum == 1) {
        Ai::chooseRocket(tipPokerList, list);
        return;
    }
    if (NULL != tfc) {
        Ai::copyAndClean(tfc, tipPokerList, 1);
        return;
    }
}

void Ai::choosePair(Vector<PokerDDZ *> *tipPokerList, int lastNum, Vector<TmpFaceCard *> *list)
{
    TmpFaceCard *tfc = NULL;
    for (int i = 0; i < list->size(); i++) {
        TmpFaceCard *card = list->at(i);
        if (card->getNum() > lastNum) {
            if (card->getCardList()->size() == 2) {
                Ai::copyAndClean(card, tipPokerList, 2);
                return;
            } else if (card->getCardList()->size() > 2 && NULL == tfc) {
                tfc = card;
            }
        }
    }
    if (NULL != tfc) {
        Ai::copyAndClean(tfc, tipPokerList, 2);
        return;
    }
}

void Ai::chooseTriad(Vector<PokerDDZ *> *tipPokerList, int lastNum, Vector<TmpFaceCard *> *list)
{
    TmpFaceCard *tfc = NULL;
    for (int i = 0; i < list->size(); i++) {
        TmpFaceCard *card = list->at(i);
        if (card->getNum() > lastNum) {
            if (card->getCardList()->size() == 3) {
                Ai::copyAndClean(card, tipPokerList, 3);
                return;
            } else if (card->getCardList()->size() == 4 && NULL == tfc) {
                tfc = card;
            }
        }
    }
    if (NULL != tfc) {
        Ai::copyAndClean(tfc, tipPokerList, 3);
        return;
    }
}

void Ai::chooseTriadSingle(Vector<PokerDDZ *> *tipPokerList, int lastNum, Vector<TmpFaceCard *> *list)
{
    Vector<PokerDDZ *> list1;
    Ai::chooseTriad(&list1, lastNum, list);
    if (0 == list1.size()) {
        return;
    }
    Vector<PokerDDZ *> list2;
    Ai::chooseSingle(&list2, -1, list);
    if (0 == list2.size()) {
        return;
    }
    tipPokerList->pushBack(list1);
    tipPokerList->pushBack(list2);
}

void Ai::chooseTriadPair(Vector<PokerDDZ *> *tipPokerList, int lastNum, Vector<TmpFaceCard *> *list)
{
    Vector<PokerDDZ *> list1;
    Ai::chooseTriad(&list1, lastNum, list);
    if (0 == list1.size()) {
        return;
    }
    Vector<PokerDDZ *> list2;
    Ai::choosePair(&list2, -1, list);
    if (0 == list2.size()) {
        return;
    }
    tipPokerList->pushBack(list1);
    tipPokerList->pushBack(list2);
}

void Ai::chooseStraight(Vector<PokerDDZ *> *tipPokerList, int lastCardNum, Vector<TmpFaceCard *> *list, int lastPlayListSize, int num)
{
    if (0 >= lastCardNum) {
        return;
    }
    int snum = lastPlayListSize / num;
    if (snum > list->size()) {
        return;
    }
    int i = 0;
    int length = (int)list->size();
    while (i < length) {
        bool hasStraight = true;
        TmpFaceCard *card = list->at(i);
        if (card->getCardList()->size() == 4 || card->getNum() >= POKERDDZ_NUM::DDZ_TWO || card->getNum() <= lastCardNum || card->getCardList()->size() < num) {
            i += 1;
            continue;
        }
        for (int j = 0; j < snum - 1; j++) {
            if (i + j + 1 >= length) {
                hasStraight = false;
                break;
            }
            TmpFaceCard *tfc = list->at(i + j + 1);
            if (tfc->getNum() < POKERDDZ_NUM::DDZ_TWO && tfc->getCardList()->size() != 4 && tfc->getCardList()->size() >= num) {
                if (tfc->getNum() == list->at(i + j)->getNum() + 1) {
                    continue;
                }
            }
            i = i + j;
            hasStraight = false;
            break;
        }
        if (hasStraight) {
            break;
        }
        i += 1;
    }
    if (i < length && (i + snum -1 < length)) {
        for (int j = 0; j < snum; j++) {
            for (int k = 0; k < num; k++) {
                auto l = list->at(i+j)->getCardList();
                auto poker = l->at(0);
                tipPokerList->pushBack(poker);
                l->erase(0);
            }
        }
    }
}

void Ai::choosePlane(Vector<PokerDDZ *> *tipPokerList, int lastCardNum, Vector<TmpFaceCard *> *list, int lastNum)
{
    int triadNum = lastNum / 4;
    Vector<PokerDDZ *> list1;
    Ai::chooseStraight(&list1, lastCardNum, list, triadNum * 3, 3);
    if (list1.size() == 0) {
        return;
    }
    for (int i = 0; i < triadNum; i++) {
        Vector<PokerDDZ *> list2;
        Ai::chooseSingle(&list2, -1, list);
        if (list2.size() == 0) {
            return;
        }
        tipPokerList->pushBack(list1);
        tipPokerList->pushBack(list2);
    }
}

void Ai::chooseDoublePlane(Vector<PokerDDZ *> *tipPokerList, int lastCardNum, Vector<TmpFaceCard *> *list, int lastNum)
{
    int triadNum = lastNum / 5;
    Vector<PokerDDZ *> list1;
    Ai::chooseStraight(&list1, lastCardNum, list, triadNum * 3, 3);
    if (list1.size() == 0) {
        return;
    }
    for (int i = 0; i < triadNum; i++) {
        Vector<PokerDDZ *> list2;
        Ai::chooseSingle(&list2, -1, list);
        if (list2.size() == 0) {
            return;
        }
        tipPokerList->pushBack(list1);
        tipPokerList->pushBack(list2);
    }
}

void Ai::chooseRocket(Vector<PokerDDZ *> *tipPokerList, Vector<TmpFaceCard *> *list)
{
    if (list->size() < 2) {
        return;
    }
    TmpFaceCard *card = list->at(list->size()-2);
    if (card->getNum() == POKERDDZ_NUM::DDZ_J_B) {
        auto clist1 = card->getCardList();
        auto poker1 = clist1->at(0);
        tipPokerList->pushBack(poker1);
        clist1->erase(0);
        auto clist2 = list->at(list->size()-1)->getCardList();
        auto poker2 = clist2->at(0);
        tipPokerList->pushBack(poker2);
        clist2->erase(0);
    }
}

void Ai::chooseBomb(Vector<PokerDDZ *> *tipPokerList, int lastNum, Vector<TmpFaceCard *> *list)
{
    for (int i = 0; i < list->size(); i++) {
        auto tfc = list->at(i);
        if (tfc->getNum() > lastNum && tfc->getCardList()->size() == 4) {
            Ai::copyAndClean(tfc, tipPokerList, (int)tfc->getCardList()->size());
        }
    }
}

void Ai::copyAndClean(TmpFaceCard *tfc, Vector<PokerDDZ *> *dst, int len)
{
    for (int i = 0; i < len; i++) {
        auto src = tfc->getCardList();
        if (0 == src->size()) {
            return;
        }
        auto poker = src->at(0);
        dst->pushBack(poker);
        src->erase(0);
    }
}

CardType* Ai::cardTypeNull()
{
    CardType *ct = CardType::create(Ai::CARD_TYPE_NONE, -1);
    return ct;
}

CardType* Ai::cardTypeIllegal()
{
    CardType *ct = CardType::create(Ai::CARD_TYPE_ILLEGAL, -1);
    return ct;
}

CardType* Ai::validCardsAndGetCardType(CardType *lastCardType, int lastPlayListSize, Vector<PokerDDZ *> pokerList, int naizi)
{
    if (0 == pokerList.size()) {
        CCLOG("valid 0 == pokerList.size()");
        return Ai::cardTypeNull();
    }
    CardList *cardList = CardList::create(pokerList, naizi);
//    if (naizi < 0 || cardList->getNaiziNum() == 0) {
//
//    }
    return Ai::validCardsAndGetCardTypeNormal(lastCardType, lastPlayListSize, cardList);
}

CardType* Ai::validCardsAndGetCardTypeNormal(CardType *lastCardType, int lastPlayListSize, CardList *cardList)
{
    if (lastCardType->getCardType() <= CARD_TYPE_NONE || lastPlayListSize <= 0) {
        CCLOG("valid lastCardType->getCardType()[%d] <= CARD_TYPE_NONE || lastPlayListSize[%d] <= 0", lastCardType->getCardType(), lastPlayListSize);
        return Ai::getNormalCardType(cardList);
    }
    if (lastCardType->isRocket()) {
        CCLOG("valid lastCardType->isRocket()");
        return Ai::cardTypeIllegal();
    }
    CCLOG("valid lastCardType->getCardType()[%d]", lastCardType->getCardType());
    if (lastCardType->getCardType() == CARD_TYPE_PLANE || lastCardType->getCardType() == CARD_TYPE_DOUBLE_PLANE) {
        ValueVector *firstList = cardList->getFirstList();
        ValueVector *secondList = cardList->getSecondList();
        ValueVector *thirdList = cardList->getThirdList();
        ValueVector *fourthList = cardList->getFourthList();
        int firstSize = (int)firstList->size();
        int secondSize = (int)secondList->size();
        int thirdSize = (int)thirdList->size();
        int fourthSize = (int)fourthList->size();
        CCLOG("valid size first[%d],second[%d],third[%d],fourth[%d]", firstSize, secondSize, thirdSize, fourthSize);
        if (firstSize == fourthSize && fourthSize == 1) {
            return CardType::create(CARD_TYPE_BOMB, cardList->getFourthList(0));
        }
        if (secondSize + thirdSize + fourthSize == 0) {
            if (firstSize == 2) {
                if (cardList->getFirstList(0) >= POKERDDZ_NUM::DDZ_J_B) {
                    return CardType::create(CARD_TYPE_ROCKET, POKERDDZ_NUM::DDZ_J_B);
                }
            }
            return Ai::cardTypeIllegal();
        }
        if (lastPlayListSize != cardList->getCardNum()) {
            return Ai::cardTypeIllegal();
        }
        if (lastCardType->getCardType() == CARD_TYPE_PLANE) {
            int thirdNum = 0;
            thirdNum = lastPlayListSize / 4;
            if(thirdSize < thirdNum){
                return Ai::cardTypeIllegal();
            }
            if(thirdSize == thirdNum) {
                if(Ai::checkContinus(thirdList)) {
                    int num = cardList->getThirdList(0);
                    if(num > lastCardType->getNum()) {
                        return CardType::create(lastCardType->getCardType(), num);
                    }
                }
                return Ai::cardTypeIllegal();
            }
            
            if(thirdSize > thirdNum+ 1) {
                return Ai::cardTypeIllegal();
            }
            
            ValueVector tmp;
            Ai::copyValueVector(thirdList, &tmp, 0, thirdNum);
            if (Ai::checkContinus(&tmp) && tmp.at(0).asInt() > lastCardType->getNum()) {
                return CardType::create(lastCardType->getCardType(), tmp.at(0).asInt());
            }
            tmp.clear();
            Ai::copyValueVector(thirdList, &tmp, 1, thirdNum);
            if (Ai::checkContinus(&tmp) && tmp.at(0).asInt() > lastCardType->getNum()) {
                return CardType::create(CARD_TYPE_PLANE, tmp.at(0).asInt());
            }
            
            return Ai::cardTypeIllegal();
        }
        if(lastCardType->getCardType() == CARD_TYPE_PLANE){
            int thirdNum = 0;
            thirdNum = lastPlayListSize / 5;
            if(firstSize > secondSize) {
                return Ai::cardTypeIllegal();
            }
            if(thirdNum == thirdSize) {
                if(Ai::checkContinus(thirdList) && cardList->getThirdList(0) > lastCardType->getNum()) {
                    return CardType::create(lastCardType->getCardType(), cardList->getThirdList(0));
                }
                return Ai::cardTypeIllegal();
            }
            
            ValueVector tmp;
            for (int i = 0; i < thirdSize; i++) {
                int n3 = thirdList->at(i).asInt();
                if(cardList->hasNum(n3, fourthList)){
                    continue;
                }
                /*
                 bool flag = false;
                 for (int j = 0; j < fourthList->count(); j++) {
                 int n4 = ((CCInteger*)fourthList->objectAtIndex(i))->getValue();
                 if (n3 == n4) {
                 flag = true;
                 break;
                 }
                 }
                 if (true == flag) {
                 continue;
                 }
                 */
                tmp.push_back(Value(n3));
            }
            if (tmp.size() != thirdNum) {
                return Ai::cardTypeIllegal();
            }
            if (Ai::checkContinus(&tmp)) {
                if(tmp.at(0).asInt() > lastCardType->getNum()) {
                    return CardType::create(lastCardType->getCardType(), tmp.at(0).asInt());
                }
                
            }
            
            return Ai::cardTypeIllegal();
        }
        return Ai::cardTypeIllegal();
    }
    CardType *ct = Ai::getNormalCardType(cardList);
    CCLOG("valid ct->getCardType()[%d]", ct->getCardType());
    if(ct->getCardType() == lastCardType->getCardType()) {
        CCLOG("valid ct->getNum()[%d] > lastCardType->getNum()[%d] && cardList->getCardNum()[%d] == lastPlayListSize[%d]", ct->getNum(), lastCardType->getNum(), cardList->getCardNum(), lastPlayListSize);
        if(ct->getNum() > lastCardType->getNum() && cardList->getCardNum() == lastPlayListSize){
            return ct;
        }
        return Ai::cardTypeIllegal();
    }
    if(lastCardType->getCardType() == CARD_TYPE_BOMB_Y) {
        if(ct->getCardType() == CARD_TYPE_ROCKET) {
            return ct;
        }
        return Ai::cardTypeIllegal();
    }
    if(ct->getCardType() == CARD_TYPE_ROCKET || ct->getCardType() == CARD_TYPE_BOMB) {
        return ct;
    }
    return Ai::cardTypeIllegal();
}

CardType* Ai::getCardType(Vector<PokerDDZ *> pokerList)
{
    if (0 == pokerList.size()) {
        return Ai::cardTypeNull();
    }
    CardList *list = CardList::create(pokerList, -1);
    CardType *ct = getNormalCardType(list);
    return ct;
}

CardType* Ai::getNormalCardType(CardList *cards)
{
    CCLOG("getNormalCardType cards->getCardNum()[%d]", cards->getCardNum());
    if (1 == cards->getCardNum()) {
        return CardType::create(CARD_TYPE_SINGLE, cards->getFirstList(0));
    }
    ValueVector *firstList = cards->getFirstList();
    ValueVector *secondList = cards->getSecondList();
    ValueVector *thirdList = cards->getThirdList();
    ValueVector *fourthList = cards->getFourthList();
    int firstSize = (int)firstList->size();
    int secondSize = (int)secondList->size();
    int thirdSize = (int)thirdList->size();
    int fourthSize = (int)fourthList->size();
    CCLOG("getNormalCardType first[%d]second[%d]third[%d]fourth[%d]", firstSize, secondSize, thirdSize, fourthSize);
    
    if (fourthSize + thirdSize == 0) {
        if (secondSize == 0) {
            if (firstSize == 1) {
                return CardType::create(CARD_TYPE_SINGLE, cards->getFirstList(0));
            }
            if (firstSize == 2) {
                if (cards->getFirstList(0) >= POKERDDZ_NUM::DDZ_J_B) {
                    return CardType::create(CARD_TYPE_ROCKET, POKERDDZ_NUM::DDZ_J_B);
                }
            }
            if (firstSize >= 5) {
                if (Ai::checkContinus(firstList)) {
                    //keltonTODO : get last num?? cards->getFirstList(firstSize)
                    return CardType::create(CARD_TYPE_STRAIGHT, cards->getFirstList(0));
                }
            }
        } else if (firstSize == secondSize) {
            if (secondSize == 1) {
                return CardType::create(CARD_TYPE_PAIR, cards->getSecondList(0));
            }
            if (secondSize >= 3) {
                if (Ai::checkContinus(secondList)) {
                    return CardType::create(CARD_TYPE_DOUBLE_STRAIGHT, cards->getSecondList(0));
                }
            }
        }
        return Ai::cardTypeIllegal();
    }
    
    if (fourthSize == 0) {
        if (thirdSize == firstSize) {
            if (thirdSize == 1) {
                return CardType::create(CARD_TYPE_TRIAD, cards->getThirdList(0));
            }
            if (Ai::checkContinus(thirdList)) {
                return CardType::create(CARD_TYPE_TRIAD_STRAIGHT, cards->getThirdList(0));
            }
            if (thirdSize == 4) {
                ValueVector tmp;
                Ai::copyValueVector(thirdList, &tmp, 0, thirdSize-1);
                if (Ai::checkContinus(&tmp)) {
                    return CardType::create(CARD_TYPE_PLANE, tmp.at(0).asInt());
                }
                tmp.clear();
                Ai::copyValueVector(thirdList, &tmp, 1, thirdSize-1);
                if (Ai::checkContinus(&tmp)) {
                    return CardType::create(CARD_TYPE_PLANE, tmp.at(0).asInt());
                }
            }
        } else {
            if (thirdSize == 1) {
                if (firstSize == secondSize) {
                    if (secondSize == 2) {
                        return CardType::create(CARD_TYPE_TRIAD_PAIR, cards->getThirdList(0));
                    }
                } else if (firstSize == 2) {
                    return CardType::create(CARD_TYPE_TRIAD_SINGLE, cards->getThirdList(0));
                }
            } else if (Ai::checkContinus(thirdList)) {
                if(firstSize - thirdSize + secondSize - thirdSize == thirdSize) {
                    return CardType::create(CARD_TYPE_PLANE, cards->getThirdList(0));
                }
                if(firstSize == secondSize && secondSize - thirdSize == thirdSize) {
                    return CardType::create(CARD_TYPE_DOUBLE_PLANE, cards->getThirdList(0));
                }
            } else {
                // 自己拆,最多可以一个3张不用,把他当单张
                if(thirdSize >= 12) {
                    ValueVector tmp;
                    Ai::copyValueVector(thirdList, &tmp, 0, thirdSize-1);
                    if (Ai::checkContinus(&tmp)) {
                        if(3 + firstSize - thirdSize + secondSize - thirdSize == thirdSize - 1){
                            // 只能是拆除一个3张
                            return CardType::create(CARD_TYPE_PLANE, cards->getThirdList(0));
                        }
                    }
                    tmp.clear();
                    Ai::copyValueVector(thirdList, &tmp, 1, thirdSize-1);
                    if (Ai::checkContinus(&tmp)) {
                        if(3 + firstSize - thirdSize + secondSize - thirdSize == thirdSize - 1){
                            // 只能是拆除一个3张
                            return CardType::create(CARD_TYPE_PLANE, cards->getThirdList(1));
                        }
                    }
                }
            }
        }
        return Ai::cardTypeIllegal();
    }
    
    if(firstSize == fourthSize) {
        // 没有独立的1张 2张和3张
        if(fourthSize == 1) {
            return CardType::create(CARD_TYPE_BOMB, cards->getFourthList(0));
        }
        
        if(fourthSize == 2){
            return CardType::create(CARD_TYPE_QUAD_TWO_PAIR, cards->getFourthList(1));
        }
        
        if(Ai::checkContinus(fourthList)) {
            return CardType::create(CARD_TYPE_PLANE, cards->getFourthList(0));
        }
        
        return Ai::cardTypeIllegal();
    }
    
    if(thirdSize == fourthSize) {
        //有独立的2张和1张
        if(fourthSize == 1){
            if(firstSize - fourthSize + secondSize - fourthSize == 2) {
                return CardType::create(CARD_TYPE_QUAD_TWO, cards->getFourthList(0));
            }
            if(firstSize == secondSize && secondSize == 3) {
                return CardType::create(CARD_TYPE_QUAD_TWO_PAIR, cards->getFourthList(0));
            }
        }
        return Ai::cardTypeIllegal();
    }
    
    // 有独立的3张,只能是飞机了
    // 一种是4张是飞机的一部分,一种是4张只是飞机的附属
    // 无论怎样,只能有
    if (cards->getCardNum() % 5 == 0) {
        if ((firstSize == thirdSize && (thirdSize - fourthSize == fourthSize * 2)) ||
            (firstSize == secondSize && (thirdSize - fourthSize == fourthSize * 2 + secondSize - thirdSize))) {
            ValueVector tmp;
            for (int i = 0; i < thirdSize; i++) {
                int n3 = thirdList->at(i).asInt();
                if(cards->hasNum(n3, fourthList)){
                    continue;
                }
                /*
                 bool flag = false;
                 for (int j = 0; j < fourthList->count(); j++) {
                 int n4 = ((CCInteger *)fourthList->objectAtIndex(i))->getValue();
                 if (n3 == n4) {
                 flag = true;
                 break;
                 }
                 }
                 if (true == flag) {
                 continue;
                 }
                 */
                tmp.push_back(Value(n3));
            }
            if (tmp.size() >= 2) {
                if (Ai::checkContinus(&tmp)) {
                    return CardType::create(CARD_TYPE_DOUBLE_PLANE, tmp.at(0).asInt());
                }
            }
        }
        return Ai::cardTypeIllegal();
    }
    if (cards->getCardNum() % 4 == 0) {
        if (Ai::checkContinus(thirdList)) {
            if(firstSize - thirdSize + secondSize - thirdSize + fourthSize == thirdSize) {
                return CardType::create(CARD_TYPE_PLANE, cards->getThirdList(0));
            }
        }
        if (cards->getCardNum() == 16) {
            ValueVector tmp;
            Ai::copyValueVector(thirdList, &tmp, 0, thirdSize-1);
            if (Ai::checkContinus(&tmp)) {
                return CardType::create(CARD_TYPE_PLANE, tmp.at(0).asInt());
            }
            tmp.clear();
            Ai::copyValueVector(thirdList, &tmp, 1, thirdSize-1);
            if (Ai::checkContinus(&tmp)) {
                return CardType::create(CARD_TYPE_PLANE, tmp.at(0).asInt());
            }
        }
        return Ai::cardTypeIllegal();
    }
    //飞机双翅膀
    
    return Ai::cardTypeIllegal();
}

bool Ai::checkContinus(ValueVector *list)
{
    int size = (int)list->size();
    if (size < 2) {
        return false;
    }
    int num = list->at(size - 1).asInt();
    if (num >= POKERDDZ_NUM::DDZ_TWO) {
        return false;
    }
    for (int i = 0; i < size - 1; i++) {
        int num1 = list->at(i).asInt();
        int num2 = list->at(i+1).asInt();
        if (num1 + 1 != num2) {
            return false;
        }
    }
    return true;
}

void Ai::copyValueVector(ValueVector *src, ValueVector *dst, int from, int end)
{
    if (from < 0 || from < end || end > src->size()-1) {
        return;
    }
    for (int i = from; i < end; i++) {
        int v = src->at(i).asInt();
        dst->push_back(Value(v));
    }
}

void Ai::getCardTypeName(char *name, int nameSize, int cardtype)
{
    memset(name, 0, nameSize);
    switch (cardtype) {
        case CARD_TYPE_ILLEGAL:
            sprintf(name, "%s", "非法");
            break;
        case CARD_TYPE_NONE:
            sprintf(name, "%s", "空");
            break;
        case CARD_TYPE_SINGLE:
            sprintf(name, "%s", "单张");
            break;
        case CARD_TYPE_PAIR:
            sprintf(name, "%s", "对子");
            break;
        case CARD_TYPE_TRIAD:
            sprintf(name, "%s", "三张");
            break;
        case CARD_TYPE_TRIAD_SINGLE:
            sprintf(name, "%s", "三带一");
            break;
        case CARD_TYPE_TRIAD_PAIR:
            sprintf(name, "%s", "三带一对");
            break;
        case CARD_TYPE_QUAD_TWO:
            sprintf(name, "%s", "四带两个");
            break;
        case CARD_TYPE_QUAD_TWO_PAIR:
            sprintf(name, "%s", "四带两对");
            break;
        case CARD_TYPE_STRAIGHT:
            sprintf(name, "%s", "顺子");
            break;
        case CARD_TYPE_DOUBLE_STRAIGHT:
            sprintf(name, "%s", "连对");
            break;
        case CARD_TYPE_TRIAD_STRAIGHT:
            sprintf(name, "%s", "飞机不带翅膀");
            break;
        case CARD_TYPE_PLANE:
            sprintf(name, "%s", "飞机带单翅膀");
            break;
        case CARD_TYPE_DOUBLE_PLANE:
            sprintf(name, "%s", "飞机带双翅膀");
            break;
        case CARD_TYPE_BOMB:
            sprintf(name, "%s", "炸弹");
            break;
        case CARD_TYPE_ROCKET:
            sprintf(name, "%s", "火箭");
            break;
        case CARD_TYPE_BOMB_Y:
            sprintf(name, "%s", "纯癞子炸弹");
            break;
        case CARD_TYPE_BOMB_SOFT:
            sprintf(name, "%s", "软炸弹");
            break;
            
        default:
            sprintf(name, "%s", "未知牌型");
            break;
    }
}

void Ai::getCardTypeNumName(char *name, int nameSize, int cardtypenum)
{
    CCLOG("cardtypenum[%d]", cardtypenum);
    memset(name, 0, nameSize);
    if (POKERDDZ_NUM::DDZ_THREE <= cardtypenum && cardtypenum <= POKERDDZ_NUM::DDZ_TEN) {
        sprintf(name, "%d", cardtypenum+3);
        return;
    }
    if (cardtypenum == POKERDDZ_NUM::DDZ_JACK) {
        sprintf(name, "%s", "J");
        return;
    }
    if (cardtypenum == POKERDDZ_NUM::DDZ_QUEEN) {
        sprintf(name, "%s", "Q");
        return;
    }
    if (cardtypenum == POKERDDZ_NUM::DDZ_KING) {
        sprintf(name, "%s", "K");
        return;
    }
    if (cardtypenum == POKERDDZ_NUM::DDZ_ACE) {
        sprintf(name, "%s", "A");
        return;
    }
    if (cardtypenum == POKERDDZ_NUM::DDZ_TWO) {
        sprintf(name, "%s", "2");
        return;
    }
    if (cardtypenum == POKERDDZ_NUM::DDZ_J_R) {
        sprintf(name, "%s", "红鬼");
        return;
    }
    if (cardtypenum == POKERDDZ_NUM::DDZ_J_B) {
        sprintf(name, "%s", "黑鬼");
        return;
    }
    sprintf(name, "%s", "未知");
}
#pragma mark Ai end





