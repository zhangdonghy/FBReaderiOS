//
//  ZLTextStyle.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/31.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLTextStyle_hpp
#define ZLTextStyle_hpp

#include <stdio.h>
#include "shared_ptr.h"
#include <vector>

class ZLTextHyperlink;
class ZLTextMetrics;
class ZDFontEntry;
class ZLTextStyle{
public:
    enum StyleKind
    {
       KIND_Unknown,
       KIND_Explicit
    };
    shared_ptr<ZLTextStyle> Parent;
    shared_ptr<ZLTextHyperlink> Hyperlink;
    
    ZLTextStyle(shared_ptr<ZLTextStyle> parent, shared_ptr<ZLTextHyperlink> hyperlink) {
        if(hyperlink == 0) {
            Parent = parent != 0 ? parent : this;
            Hyperlink = hyperlink;
        } else {
            Parent = parent != 0 ? parent : this;
            Hyperlink = hyperlink;
        }
        
    }
    
   // virtual
    virtual StyleKind styleKind()
    {
        return KIND_Unknown;
    }
    virtual bool isBaseStyle(){
        return false;
    }
    
    virtual std::vector<shared_ptr<ZDFontEntry>> getFontEntries()=0;
    virtual std::string getFontName() = 0;
    virtual int getFontSize(shared_ptr<ZLTextMetrics> metrics)=0;
    
    virtual bool isBold()=0;
    virtual bool isItalic()=0;
    virtual bool isUnderline()=0;
    virtual bool isStrikeThrough()=0;
    
    int getLeftIndent(shared_ptr<ZLTextMetrics> metrics) {
        return getLeftMargin(metrics) + getLeftPadding(metrics);
    }
    int getRightIndent(shared_ptr<ZLTextMetrics> metrics) {
        return getRightMargin(metrics) + getRightPadding(metrics);
    }
    virtual int getLeftMargin(shared_ptr<ZLTextMetrics> metrics)=0;
    virtual int getRightMargin(shared_ptr<ZLTextMetrics> metrics)=0;
    virtual int getLeftPadding(shared_ptr<ZLTextMetrics> metrics)=0;
    virtual int getRightPadding(shared_ptr<ZLTextMetrics> metrics)=0;
    
    virtual int getFirstLineIndent(shared_ptr<ZLTextMetrics> metrics)=0;
    virtual int getLineSpacePercent()=0;
    virtual int getVerticalAlign(shared_ptr<ZLTextMetrics> metrics)=0;
    virtual bool* isVerticallyAligned()=0;
    virtual int getSpaceBefore(shared_ptr<ZLTextMetrics> metrics)=0;
    virtual int getSpaceAfter(shared_ptr<ZLTextMetrics> metrics)=0;
    virtual unsigned char getAlignment()=0;
    
    virtual bool allowHyphenations()=0;
};
#endif /* ZLTextStyle_hpp */
