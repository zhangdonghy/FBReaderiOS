//
//  ZLTextPosition.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/31.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLTextPosition_hpp
#define ZLTextPosition_hpp

#include <string>
#include "shared_ptr.h"

class ZLTextPosition
{
private:
protected:
public:
    virtual int getParagraphIndex()const=0 ;
    virtual int getElementIndex()const=0;
    virtual int getCharIndex()const=0;
    bool samePositionAs(const ZLTextPosition& position);
    int compareTo(ZLTextPosition& position);
    int compareToIgnoreChar(shared_ptr<ZLTextPosition> position);
    int hashCode();
    bool equals(shared_ptr<ZLTextPosition> position);
    virtual std::string toString();
    
};
#endif /* ZLTextPosition_hpp */
