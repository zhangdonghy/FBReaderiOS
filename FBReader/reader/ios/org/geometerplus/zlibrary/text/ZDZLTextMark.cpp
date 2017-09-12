//
//  ZDZLTextMark.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/16.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ZDZLTextMark.hpp"


ZDZLTextMark::ZDZLTextMark(int paragraphIndex,int offset,int length)
{
    ParagraphIndex = paragraphIndex;
    Offset = offset;
    Length = length;
}
ZDZLTextMark::ZDZLTextMark(ZDZLTextMark* mark)
{
    ParagraphIndex = mark->ParagraphIndex;
    Offset = mark->Offset;
    Length = mark->Length;
}
int ZDZLTextMark::compareTo( ZDZLTextMark& mark)
{
    int diff = ParagraphIndex - mark.ParagraphIndex;
    return diff !=0?diff:Offset-mark.Offset;
}

std::string ZDZLTextMark::toString() {
    char chars[20];
    sprintf(chars, "%d %d %d", ParagraphIndex, Offset, Length);
    std::string str = chars;
    return str;
}

