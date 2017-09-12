//
//  ZDZLTextMark.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/16.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZDZLTextMark_hpp
#define ZDZLTextMark_hpp

#include <string>

class ZDZLTextMark {
public:
    int ParagraphIndex;
    int Offset;
    int Length;

public:
    ZDZLTextMark(int paragraphIndex,int offset,int length);
    ZDZLTextMark(ZDZLTextMark* mark);
    int compareTo(ZDZLTextMark& mark);
    std::string toString();
};

#endif /* ZDZLTextMark_hpp */
