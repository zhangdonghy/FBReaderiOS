//
//  ZLTextNGStyle.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/18.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ZLTextNGStyle.hpp"
#include "ZLTextNGStyleDescription.hpp"
#include "IOSUtil.h"
#include "ZLStringOption.hpp"


ZLTextNGStyle::ZLTextNGStyle(shared_ptr<ZLTextStyle> parent, shared_ptr<ZLTextNGStyleDescription> description,
              shared_ptr<ZLTextHyperlink> hyperlink)
:ZLTextDecoratedStyle(parent,hyperlink)
{
    myDescription = description;
}

int ZLTextNGStyle::getLineSpacePercentInternal()
{
    std::string lineHeight = myDescription->LineHeightOption->getValue();
    //if (!lineHeight.matches("[1-9][0-9]*%")) {
    if(lineHeight.size() ==0 || !IOSUtil::isDigit(lineHeight)){
        return Parent->getLineSpacePercent();
    }
    return IOSUtil::StringToInt(lineHeight);//(lineHeight.substring(0, lineHeight.length() - 1));
}
bool ZLTextNGStyle::isItalicInternal()
{
   switch (myDescription->isItalic()) {
        case B3_TRUE:
            return true;
        case B3_FALSE:
            return false;
        default:
            return Parent->isItalic();
    }
    return false;
}
bool ZLTextNGStyle::isBoldInternal()
{
    switch (myDescription->isBold()) {
        case B3_TRUE:
            return true;
        case B3_FALSE:
            return false;
        default:
            return Parent->isBold();
    }
}
bool ZLTextNGStyle::isUnderlineInternal()
{
    switch (myDescription->isUnderlined()) {
        case B3_TRUE:
            return true;
        case B3_FALSE:
            return false;
        default:
            return Parent->isUnderline();
    }
}
bool ZLTextNGStyle::isStrikeThroughInternal()
{
    switch (myDescription->isStrikedThrough()) {
        case B3_TRUE:
            return true;
        case B3_FALSE:
            return false;
        default:
            return Parent->isStrikeThrough();
    }
}
std::vector<shared_ptr<ZDFontEntry>> ZLTextNGStyle::getFontEntriesInternal()
{
    std::vector<shared_ptr<ZDFontEntry>> parentEntries = Parent->getFontEntries();
    std::string decoratedValue = myDescription->FontFamilyOption->getValue();
    if (decoratedValue.compare("")==0) {
        return parentEntries;
    }
    return parentEntries;
    /*
    FontEntry e = FontEntry.systemEntry(decoratedValue);
    if (parentEntries.size() > 0 && e.equals(parentEntries.get(0))) {
        return parentEntries;
    }
    final List<FontEntry> entries = new ArrayList<FontEntry>(parentEntries.size() + 1);
    entries.add(e);
    entries.addAll(parentEntries);
    return entries;*/
}

int ZLTextNGStyle::getFontSizeInternal(shared_ptr<ZLTextMetrics> metrics)
{
   return myDescription->getFontSize(metrics, Parent->getFontSize(metrics));
}
int ZLTextNGStyle::getSpaceBeforeInternal(shared_ptr<ZLTextMetrics> metrics, int fontSize)
{
  return myDescription->getSpaceBefore(metrics, Parent->getSpaceBefore(metrics), fontSize);
}
int ZLTextNGStyle::getSpaceAfterInternal(shared_ptr<ZLTextMetrics> metrics, int fontSize)
{
   return myDescription->getSpaceAfter(metrics, Parent->getSpaceAfter(metrics), fontSize);
}
int ZLTextNGStyle::getVerticalAlignInternal(shared_ptr<ZLTextMetrics> metrics, int fontSize)
{
    return myDescription->getVerticalAlign(metrics, Parent->getVerticalAlign(metrics), fontSize);
}
int ZLTextNGStyle::getLeftMarginInternal(shared_ptr<ZLTextMetrics> metrics, int fontSize)
{
    return myDescription->getLeftMargin(metrics, Parent->getLeftMargin(metrics), fontSize);
}
int ZLTextNGStyle::getRightMarginInternal(shared_ptr<ZLTextMetrics> metrics, int fontSize)
{
   return myDescription->getRightMargin(metrics, Parent->getRightMargin(metrics), fontSize);
}
int ZLTextNGStyle::getLeftPaddingInternal(shared_ptr<ZLTextMetrics> metrics, int fontSize)
{
   return myDescription->getLeftPadding(metrics, Parent->getLeftPadding(metrics), fontSize);
}
int ZLTextNGStyle::getRightPaddingInternal(shared_ptr<ZLTextMetrics> metrics, int fontSize)
{
    return myDescription->getRightPadding(metrics, Parent->getRightPadding(metrics), fontSize);
}
int ZLTextNGStyle::getFirstLineIndentInternal(shared_ptr<ZLTextMetrics> metrics, int fontSize)
{
    return myDescription->getFirstLineIndent(metrics, Parent->getFirstLineIndent(metrics), fontSize);
}
bool ZLTextNGStyle::isVerticallyAlignedInternal()
{
   return myDescription->hasNonZeroVerticalAlign();
}