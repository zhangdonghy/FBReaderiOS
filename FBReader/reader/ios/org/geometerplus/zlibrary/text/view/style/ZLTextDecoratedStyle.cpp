//
//  ZLTextDecoratedStyle.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/14.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ZLTextDecoratedStyle.hpp"
#include "ZLTextStyle.hpp"


ZLTextDecoratedStyle::ZLTextDecoratedStyle(shared_ptr<ZLTextStyle> base, shared_ptr<ZLTextHyperlink> hyperLink)
:ZLTextStyle(base,hyperLink!=0?hyperLink:base->Hyperlink)
{
   // BaseStyle = base instanceof ZLTextBaseStyle
   // ? (ZLTextBaseStyle)base
   // : ((ZLTextDecoratedStyle)base).BaseStyle;
    myIsNotCached = true;
    if(base->isBaseStyle()) {
        BaseStyle = base;
    } else {
        BaseStyle =  ((ZLTextDecoratedStyle&)*base).BaseStyle ;//base;
    }
    
}

void ZLTextDecoratedStyle::initMetricsCache(shared_ptr<ZLTextMetrics> metrics) {
    myMetrics = metrics;
    myFontSize = getFontSizeInternal(metrics);
    mySpaceBefore = getSpaceBeforeInternal(metrics, myFontSize);
    mySpaceAfter = getSpaceAfterInternal(metrics, myFontSize);
    myVerticalAlign = getVerticalAlignInternal(metrics, myFontSize);
    myLeftMargin = getLeftMarginInternal(metrics, myFontSize);
    myRightMargin = getRightMarginInternal(metrics, myFontSize);
    myLeftPadding = getLeftPaddingInternal(metrics, myFontSize);
    myRightPadding = getRightPaddingInternal(metrics, myFontSize);
    myFirstLineIndent = getFirstLineIndentInternal(metrics, myFontSize);
}

std::vector<shared_ptr<ZDFontEntry>>
ZLTextDecoratedStyle::getFontEntries()
{
    if (myIsNotCached) {
        initCache();
    }
    return myFontEntries;
}

std::string ZLTextDecoratedStyle::getFontName() {
    return Parent->getFontName();
}

int ZLTextDecoratedStyle::getFontSize(shared_ptr<ZLTextMetrics> metrics)
{
    if (!(metrics == myMetrics)) {
        initMetricsCache(metrics);
    }
    return myFontSize;
}

bool ZLTextDecoratedStyle::isBold()
{
    if (myIsNotCached) {
        initCache();
    }
    return myIsBold;
}
bool ZLTextDecoratedStyle::isItalic()
{
    if (myIsNotCached) {
        initCache();
    }
    return myIsItalic;
}
bool ZLTextDecoratedStyle::isUnderline()
{
    if (myIsNotCached) {
        initCache();
    }
    return myIsUnderline;
}
bool ZLTextDecoratedStyle::isStrikeThrough()
{
    if (myIsNotCached) {
        initCache();
    }
    return myIsStrikeThrough;
}
int ZLTextDecoratedStyle::getLeftMargin(shared_ptr<ZLTextMetrics> metrics)
{
    if (!(metrics == myMetrics)) {
        initMetricsCache(metrics);
    }
    return myLeftMargin;
}
int ZLTextDecoratedStyle::getRightMargin(shared_ptr<ZLTextMetrics> metrics)
{
    if (!(metrics == myMetrics)) {
        initMetricsCache(metrics);
    }
    return myRightMargin;
}
int ZLTextDecoratedStyle::getLeftPadding(shared_ptr<ZLTextMetrics> metrics)
{
    if (!(metrics == myMetrics)) {
        initMetricsCache(metrics);
    }
    return myLeftPadding;
}
int ZLTextDecoratedStyle::getRightPadding(shared_ptr<ZLTextMetrics> metrics)
{
    if (!(metrics == myMetrics)) {
        initMetricsCache(metrics);
    }
    return myRightPadding;
}

int ZLTextDecoratedStyle::getFirstLineIndent(shared_ptr<ZLTextMetrics> metrics)
{
    if (!(metrics == myMetrics)) {
        initMetricsCache(metrics);
    }
    return myFirstLineIndent;
}
int ZLTextDecoratedStyle::getLineSpacePercent()
{
    if (myIsNotCached) {
        initCache();
    }
    return myLineSpacePercent;
}
int ZLTextDecoratedStyle::getVerticalAlign(shared_ptr<ZLTextMetrics> metrics)
{
    if (!(metrics == myMetrics)) {
        initMetricsCache(metrics);
    }
    return myVerticalAlign;
}
bool* ZLTextDecoratedStyle::isVerticallyAligned()
{
    if (myIsVerticallyAligned == 0) {
        bool * ret = Parent->isVerticallyAligned();
        bool restval = false;
        if(ret != 0x0) {
            restval = restval || *ret;
        }
        restval = restval || isVerticallyAlignedInternal();
        myIsVerticallyAligned =  new bool[1];
        *myIsVerticallyAligned = restval;
    }
    return myIsVerticallyAligned;
}
int ZLTextDecoratedStyle::getSpaceBefore(shared_ptr<ZLTextMetrics> metrics)
{
    if (!(metrics == myMetrics)) {
        initMetricsCache(metrics);
    }
    return mySpaceBefore;
}
int ZLTextDecoratedStyle::getSpaceAfter(shared_ptr<ZLTextMetrics> metrics)
{
    if (!(metrics == myMetrics)) {
        initMetricsCache(metrics);
    }
    return mySpaceAfter;
}
/*
 unsigned char ZLTextDecoratedStyle::getAlignment()
{
    return  0;
}
 bool ZLTextDecoratedStyle::allowHyphenations()
{
    return  false;
}*/
