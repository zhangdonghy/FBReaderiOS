//
//  ZLTextExplicitlyDecoratedStyle.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/14.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLTextExplicitlyDecoratedStyle_hpp
#define ZLTextExplicitlyDecoratedStyle_hpp

#include <stdio.h>
#include "ZLTextDecoratedStyle.hpp"
#include "ZDZLTextStyleEntry.hpp"

class ZLTextExplicitlyDecoratedStyle:public ZLTextDecoratedStyle
{
    shared_ptr<ZDZLTextStyleEntry> myEntry;
    shared_ptr<ZLTextStyle> myTreeParent;
private:
     shared_ptr<ZLTextStyle> getTreeParent() {
        if (myTreeParent == 0) {
            myTreeParent = computeTreeParent();
        }
        return myTreeParent;
    }
    shared_ptr<ZLTextStyle>  computeTreeParent();
    
public:
    ZLTextExplicitlyDecoratedStyle(shared_ptr<ZLTextStyle> parent, shared_ptr<ZDZLTextStyleEntry> entry)
    :ZLTextDecoratedStyle(parent,parent->Hyperlink)
    {
        myEntry = entry;
    }
    virtual StyleKind styleKind()
    {
        return KIND_Explicit;
    }
    virtual unsigned char getAlignment();
    
    virtual bool allowHyphenations();
    
    
    
    virtual int getLineSpacePercentInternal();
    virtual bool isItalicInternal();
    virtual bool isBoldInternal();
    virtual bool isUnderlineInternal();
    virtual bool isStrikeThroughInternal();
    virtual std::vector<shared_ptr<ZDFontEntry>> getFontEntriesInternal();
    
    virtual int getFontSizeInternal(shared_ptr<ZLTextMetrics> metrics);
    virtual int getSpaceBeforeInternal(shared_ptr<ZLTextMetrics> metrics, int fontSize);
    virtual int getSpaceAfterInternal(shared_ptr<ZLTextMetrics> metrics, int fontSize);
    virtual int getVerticalAlignInternal(shared_ptr<ZLTextMetrics> metrics, int fontSize);
    virtual int getLeftMarginInternal(shared_ptr<ZLTextMetrics> metrics, int fontSize);
    virtual int getRightMarginInternal(shared_ptr<ZLTextMetrics> metrics, int fontSize);
    virtual int getLeftPaddingInternal(shared_ptr<ZLTextMetrics> metrics, int fontSize);
    virtual int getRightPaddingInternal(shared_ptr<ZLTextMetrics> metrics, int fontSize);
    virtual int getFirstLineIndentInternal(shared_ptr<ZLTextMetrics> metrics, int fontSize);
    virtual bool isVerticallyAlignedInternal();
    
};
#endif /* ZLTextExplicitlyDecoratedStyle_hpp */
