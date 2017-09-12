//
//  ZLTextLineInfo.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/1.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLTextLineInfo_hpp
#define ZLTextLineInfo_hpp
#include <algorithm>
#include <math.h>
#include <stdio.h>
#include "shared_ptr.h"

class ZLTextParagraphCursor;
class ZLTextStyle;

class ZLTextLineInfo
{
private:
    
protected:
    shared_ptr<ZLTextParagraphCursor> ParagraphCursor;
    
public:
// protected
    int StartElementIndex;
    int StartCharIndex;
    int RealStartElementIndex;
    int RealStartCharIndex;
    
    
    
    
   shared_ptr<ZLTextStyle> StartStyle;

    int ParagraphCursorLength;

// public
    int LeftIndent;
    int Width;
    int Height;
    int Descent;
    int VSpaceBefore;
    int VSpaceAfter;
    bool PreviousInfoUsed;
    int SpaceCounter;
    bool IsVisible;
    int EndElementIndex;
    int EndCharIndex;
    ZLTextLineInfo(shared_ptr<ZLTextParagraphCursor> paragraphCursor, int elementIndex, int charIndex,
                   shared_ptr<ZLTextStyle> style) ;
    
    
    bool isEndOfParagraph() {
        return EndElementIndex == ParagraphCursorLength;
    }
    
    void adjust(shared_ptr<ZLTextLineInfo> previous) {
        if (!PreviousInfoUsed && previous != 0) {
            Height -= std::min(previous->VSpaceAfter, VSpaceBefore);
            PreviousInfoUsed = true;
        }
    }
    
    virtual  bool equals(shared_ptr<ZLTextLineInfo> info) {
        return
        (ParagraphCursor == info->ParagraphCursor) &&
        (StartElementIndex == info->StartElementIndex) &&
        (StartCharIndex == info->StartCharIndex);
    }
    
    virtual int hashCode() ;

    friend class ZLTextView;
};

#endif /* ZLTextLineInfo_hpp */
