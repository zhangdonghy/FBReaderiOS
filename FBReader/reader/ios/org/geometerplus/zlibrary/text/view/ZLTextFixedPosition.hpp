//
//  ZLTextFixedPosition.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/1.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLTextFixedPosition_hpp
#define ZLTextFixedPosition_hpp

#include <stdio.h>
#include "ZLTextPosition.hpp"

class ZLTextFixedPosition : public ZLTextPosition{
public:
    int ParagraphIndex;
    int ElementIndex;
    int CharIndex;
    
    ZLTextFixedPosition(int paragraphIndex, int elementIndex, int charIndex) {
        ParagraphIndex = paragraphIndex;
        ElementIndex = elementIndex;
        CharIndex = charIndex;
    }
    
    ZLTextFixedPosition( shared_ptr<ZLTextPosition> position) {
        ParagraphIndex = position->getParagraphIndex();
        ElementIndex = position->getElementIndex();
        CharIndex = position->getCharIndex();
    }
    ZLTextFixedPosition( ZLTextPosition& position) {
        ParagraphIndex = position.getParagraphIndex();
        ElementIndex = position.getElementIndex();
        CharIndex = position.getCharIndex();
    }
    int getParagraphIndex() const {
        return ParagraphIndex;
    }
    
    int getElementIndex()const {
        return ElementIndex;
    }
    
    int getCharIndex()const {
        return CharIndex;
    }
    
};
class WithTimestamp :public ZLTextFixedPosition {
public:
    long Timestamp;
    
    WithTimestamp(int paragraphIndex, int elementIndex, int charIndex, long stamp)
    :ZLTextFixedPosition(paragraphIndex, elementIndex, charIndex)
    {
        
        Timestamp = stamp;//stamp != null ? stamp : -1;
    }
    
    virtual std::string toString() {
        std::string res = ZLTextFixedPosition::toString();
        res += "; timestamp = ";
        res += Timestamp;
        return res;
        //return toString() + "; timestamp = " + Timestamp;
    }
};
#endif /* ZLTextFixedPosition_hpp */
