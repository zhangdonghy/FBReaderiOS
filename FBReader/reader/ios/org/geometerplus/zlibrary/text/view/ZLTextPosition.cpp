//
//  ZLTextPosition.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/31.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ZLTextPosition.hpp"

bool ZLTextPosition::samePositionAs(const ZLTextPosition& position) {
    return
    getParagraphIndex() == position.getParagraphIndex() &&
    getElementIndex() == position.getElementIndex() &&
    getCharIndex() == position.getCharIndex();
}

int ZLTextPosition::compareTo(ZLTextPosition& position) {
    int p0 = getParagraphIndex();
    int p1 = position.getParagraphIndex();
    if (p0 != p1) {
        return p0 < p1 ? -1 : 1;
    }
    
    int e0 = getElementIndex();
    int e1 = position.getElementIndex();
    if (e0 != e1) {
        return e0 < e1 ? -1 : 1;
    }
    
    return getCharIndex() - position.getCharIndex();
}

int ZLTextPosition::compareToIgnoreChar(shared_ptr<ZLTextPosition> position)
{
    int p0 = getParagraphIndex();
    int p1 = position->getParagraphIndex();
    if (p0 != p1) {
        return p0 < p1 ? -1 : 1;
    }
    
    return getElementIndex() - position->getElementIndex();
}
int ZLTextPosition::hashCode()
{
    return (getParagraphIndex() << 16) + (getElementIndex() << 8) + getCharIndex();
}
bool
ZLTextPosition::equals(shared_ptr<ZLTextPosition> position)
{
    return
    getParagraphIndex() == position->getParagraphIndex() &&
    getElementIndex() == position->getElementIndex() &&
    getCharIndex() == position->getCharIndex();
}

std::string
ZLTextPosition::toString()
{
    return "ZLTextPosition";
   // return "ZLTextPosition" + " [" + getParagraphIndex() + "," + getElementIndex() + "," + getCharIndex() + "]";
}
