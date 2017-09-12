//
//  ZLViewWidget.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/10.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLViewWidget_hpp
#define ZLViewWidget_hpp

#include <stdio.h>
#include "ZLView.hpp"

class ZLViewWidget {
public:
    virtual void reset() = 0;
    virtual void repaint() = 0;
    
    virtual void startManualScrolling(int x, int y, ZLViewEnums_PageIndex::Direction direction) = 0;
    virtual void scrollManuallyTo(int x, int y) = 0;
    virtual void startAnimatedScrolling(int pageIndex, int x, int y, ZLViewEnums_PageIndex::Direction direction, int speed) = 0;
    virtual void startAnimatedScrolling(int pageIndex, ZLViewEnums_PageIndex::Direction direction, int speed) = 0;
    virtual void startAnimatedScrolling(int x, int y, int speed) = 0;
    
    virtual void setScreenBrightness(int percent) = 0;
    virtual int getScreenBrightness() = 0;
    
    virtual shared_ptr<ZLColor> selectingBgColor() = 0;
    virtual shared_ptr<ZLColor> selectingCursorColor() = 0;
    virtual shared_ptr<ZLColor> underlineColor(int pStart, int eStart, int cStart, int pEnd, int eEnd, int cEnd) = 0;

    virtual int countNote(int pStart, int eStart, int cStart, int pEnd, int eEnd, int cEnd) = 0;
    virtual int countOpinionInParagraph(int pIndex) = 0;
};

#endif /* ZLViewWidget_hpp */

