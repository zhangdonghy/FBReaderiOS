//
//  ZLTextLineInfo.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/1.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ZLTextLineInfo.hpp"
#include "ZLTextParagraphCursor.hpp"
#include "ZLTextParagraphCursor.hpp"

ZLTextLineInfo::ZLTextLineInfo(shared_ptr<ZLTextParagraphCursor> paragraphCursor, int elementIndex, int charIndex,
               shared_ptr<ZLTextStyle> style) {

    LeftIndent = 0;
    Width = 0;
    Height = 0;
    Descent = 0;
    VSpaceBefore=0;
    VSpaceAfter = 0;
    PreviousInfoUsed = false;
    SpaceCounter = 0;
    IsVisible = false;
    
    ParagraphCursor = paragraphCursor;
    ParagraphCursorLength = paragraphCursor->getParagraphLength();
    
    StartElementIndex = elementIndex;
    StartCharIndex = charIndex;
    RealStartElementIndex = elementIndex;
    RealStartCharIndex = charIndex;
    EndElementIndex = elementIndex;
    EndCharIndex = charIndex;
    
    StartStyle = style;
    
    
}
int ZLTextLineInfo::hashCode() {
    return ParagraphCursor->hashCode() + StartElementIndex + 239 * StartCharIndex;
}
