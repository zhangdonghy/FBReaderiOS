//
//  ZLTextElementArea.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/1.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLTextElementArea_hpp
#define ZLTextElementArea_hpp

#include <stdio.h>
#include "ZLTextFixedPosition.hpp"
#include "ZLTextElement.hpp"
#include "ZLTextStyle.hpp"

class ZLTextElementArea : public ZLTextFixedPosition
{
private:
    bool myIsLastInElement;
protected:
    int Length;
    bool AddHyphenationSign;
    bool ChangeStyle;
   shared_ptr<ZLTextStyle> Style;
    shared_ptr<ZLTextElement> Element;
public:
    int XStart;
    int XEnd;
    int YStart;
    int YEnd;
    int ColumnIndex;
    ZLTextElementArea(int paragraphIndex, int elementIndex, int charIndex, int length,
                      bool lastInElement, bool addHyphenationSign, bool changeStyle,
                      shared_ptr<ZLTextStyle> style, shared_ptr<ZLTextElement> element,
                      int xStart, int xEnd, int yStart, int yEnd, int columnIndex)
    : ZLTextFixedPosition(paragraphIndex, elementIndex, charIndex) {
        
        
        XStart = xStart;
        XEnd = xEnd;
        YStart = yStart;
        YEnd = yEnd;
        ColumnIndex = columnIndex;
        
        Length = length;
        myIsLastInElement = lastInElement;
        
        AddHyphenationSign = addHyphenationSign;
        ChangeStyle = changeStyle;
        Style = style;
        Element = element;
    }
    
    bool contains(int x, int y) {
        return (y >= YStart) && (y <= YEnd) && (x >= XStart) && (x <= XEnd);
    }
    
    bool isFirstInElement() {
        return CharIndex == 0;
    }
    
    bool isLastInElement() {
        return myIsLastInElement;
    }
    friend class ZLTextElementAreaVector;
    friend class ZLTextView;
};
#endif /* ZLTextElementArea_hpp */
