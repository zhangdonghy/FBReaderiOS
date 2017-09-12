//
//  ZLiOSWidget.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/10.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLiOSWidget_hpp
#define ZLiOSWidget_hpp

#include <stdio.h>
#include "ZLViewWidget.hpp"

#import "ZDFBReaderInterface.h"

class ZLiOSWidget: public ZLViewWidget {
private:
    ZDFBReaderInterface* myModel;

public:
    ZLiOSWidget(ZDFBReaderInterface* model);
    ~ZLiOSWidget();

    virtual void reset();
    virtual void repaint();
    
    virtual void startManualScrolling(int x, int y, ZLViewEnums_PageIndex::Direction direction);
    virtual void scrollManuallyTo(int x, int y);
    virtual void startAnimatedScrolling(int pageIndex, int x, int y, ZLViewEnums_PageIndex::Direction direction, int speed);
    virtual void startAnimatedScrolling(int pageIndex, ZLViewEnums_PageIndex::Direction direction, int speed);
    virtual void startAnimatedScrolling(int x, int y, int speed);
    
    virtual void setScreenBrightness(int percent);
    virtual int getScreenBrightness();
    
    virtual shared_ptr<ZLColor> selectingBgColor();
    virtual shared_ptr<ZLColor> selectingCursorColor();
    virtual shared_ptr<ZLColor> underlineColor(int pStart, int eStart, int cStart, int pEnd, int eEnd, int cEnd);

    virtual int countNote(int pStart, int eStart, int cStart, int pEnd, int eEnd, int cEnd);
    virtual int countOpinionInParagraph(int pIndex);
};

#endif /* ZLiOSWidget_hpp */

