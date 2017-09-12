//
//  ZLTextDecoratedStyle.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/14.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLTextDecoratedStyle_hpp
#define ZLTextDecoratedStyle_hpp

#include <stdio.h>
#include <vector>
#include <string>
#include "ZLTextStyle.hpp"


class ZDFontEntry;
class ZLTextBaseStyle;
class ZLTextDecoratedStyle : public ZLTextStyle
{
protected:
    //protected final ZLTextBaseStyle BaseStyle;
    shared_ptr<ZLTextStyle> BaseStyle;
private:
    std::vector<shared_ptr<ZDFontEntry>> myFontEntries;
    bool myIsItalic;
    bool myIsBold;
    bool myIsUnderline;
    bool myIsStrikeThrough;
    int myLineSpacePercent;
    
    bool myIsNotCached;
    
    int myFontSize;
    int mySpaceBefore;
    int mySpaceAfter;
    int myVerticalAlign;
    bool* myIsVerticallyAligned;//Boolean myIsVerticallyAligned
    int myLeftMargin;
    int myRightMargin;
    int myLeftPadding;
    int myRightPadding;
    int myFirstLineIndent;
    shared_ptr<ZLTextMetrics> myMetrics;
    
    void initCache() {
        myFontEntries = getFontEntriesInternal();
        myIsItalic = isItalicInternal();
        myIsBold = isBoldInternal();
        myIsUnderline = isUnderlineInternal();
        myIsStrikeThrough = isStrikeThroughInternal();
        myLineSpacePercent = getLineSpacePercentInternal();
        
        myIsNotCached = false;
    }
    void initMetricsCache(shared_ptr<ZLTextMetrics> metrics);

protected:
    virtual int getLineSpacePercentInternal()=0;
    virtual bool isItalicInternal()=0;
    virtual bool isBoldInternal()=0;
    virtual bool isUnderlineInternal()=0;
    virtual bool isStrikeThroughInternal()=0;
    virtual std::vector<shared_ptr<ZDFontEntry>> getFontEntriesInternal()=0;
    
    virtual int getFontSizeInternal(shared_ptr<ZLTextMetrics> metrics)=0;
    virtual int getSpaceBeforeInternal(shared_ptr<ZLTextMetrics> metrics, int fontSize)=0;
    virtual int getSpaceAfterInternal(shared_ptr<ZLTextMetrics> metrics, int fontSize)=0;
    virtual int getVerticalAlignInternal(shared_ptr<ZLTextMetrics> metrics, int fontSize)=0;
    virtual int getLeftMarginInternal(shared_ptr<ZLTextMetrics> metrics, int fontSize)=0;
    virtual int getRightMarginInternal(shared_ptr<ZLTextMetrics> metrics, int fontSize)=0;
    virtual int getLeftPaddingInternal(shared_ptr<ZLTextMetrics> metrics, int fontSize)=0;
    virtual int getRightPaddingInternal(shared_ptr<ZLTextMetrics> metrics, int fontSize)=0;
    virtual int getFirstLineIndentInternal(shared_ptr<ZLTextMetrics> metrics, int fontSize)=0;
    virtual bool isVerticallyAlignedInternal()=0;
public:

    ZLTextDecoratedStyle(shared_ptr<ZLTextStyle> base, shared_ptr<ZLTextHyperlink> hyperLink);
    
    virtual std::vector<shared_ptr<ZDFontEntry>> getFontEntries() ;
    
    virtual std::string getFontName();
    virtual int getFontSize(shared_ptr<ZLTextMetrics> metrics);
    
    virtual bool isBold();
    virtual bool isItalic();
    virtual bool isUnderline();
    virtual bool isStrikeThrough();
    virtual int getLeftMargin(shared_ptr<ZLTextMetrics> metrics);
    virtual int getRightMargin(shared_ptr<ZLTextMetrics> metrics);
    virtual int getLeftPadding(shared_ptr<ZLTextMetrics> metrics);
    virtual int getRightPadding(shared_ptr<ZLTextMetrics> metrics);
    
    virtual int getFirstLineIndent(shared_ptr<ZLTextMetrics> metrics);
    virtual int getLineSpacePercent();
    virtual int getVerticalAlign(shared_ptr<ZLTextMetrics> metrics);
    virtual bool* isVerticallyAligned();
    virtual int getSpaceBefore(shared_ptr<ZLTextMetrics> metrics);
    virtual int getSpaceAfter(shared_ptr<ZLTextMetrics> metrics);
    //virtual unsigned char getAlignment();
    
   // virtual bool allowHyphenations();
    
};
#endif /* ZLTextDecoratedStyle_hpp */
