//
//  ZLTextBaseStyle.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/3.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ZLTextBaseStyle.hpp"
#include "ZLTextHyperlink.hpp"
#include "ZLBooleanOption.hpp"
#include "ZDZLibrary.hpp"

std::string ZLTextBaseStyle::GROUP = "GROUP";
std::string ZLTextBaseStyle::OPTIONS = "OPTIONS";

ZLTextBaseStyle::ZLTextBaseStyle(std::string prefix,std::string fontFamily,int fontSize)
:ZLTextStyle(0,ZLTextHyperlink::NO_LINK)
{
    AutoHyphenationOption = new ZLBooleanOption(ZLTextBaseStyle::OPTIONS, "AutoHyphenation", true);
    UseCSSTextAlignmentOption = new ZLBooleanOption("Style", "css:textAlignment", true);
    UseCSSMarginsOption = new ZLBooleanOption("Style", "css:margins", true);
    UseCSSFontSizeOption = new ZLBooleanOption("Style", "css:fontSize", true);
    UseCSSFontFamilyOption = new ZLBooleanOption("Style", "css:fontFamily", true);
    
    
    FontFamilyOption = new ZLStringOption(GROUP, prefix + ":fontFamily", fontFamily);
    fontSize = fontSize * ZDZLibrary::Instance()->getDisplayDPI() / 160;
    FontSizeOption = new ZLIntegerRangeOption(GROUP, prefix + ":fontSize", 5, std::max(144, fontSize * 2), fontSize);
    BoldOption = new ZLBooleanOption(GROUP, prefix + ":bold", false);
    ItalicOption = new ZLBooleanOption(GROUP, prefix + ":italic", false);
    UnderlineOption = new ZLBooleanOption(GROUP, prefix + ":underline", false);
    StrikeThroughOption = new ZLBooleanOption(GROUP, prefix + ":strikeThrough", false);
    AlignmentOption = new ZLIntegerRangeOption(GROUP, prefix + ":alignment", 1, 4, ZLTextAlignmentType::ALIGN_JUSTIFY);
    LineSpaceOption = new ZLIntegerRangeOption(GROUP, prefix + ":lineSpacing", 5, 20, 12);
    
    FontName = "";
    
    //myBase = (ZLTextStyle*)this;
}

std::string ZLTextBaseStyle::getFontName() {
    return FontName;
}

int ZLTextBaseStyle::getFontSize() {
    return FontSizeOption->getValue();
//    return  20; // should read from config file  zhangdong
}

int ZLTextBaseStyle::getFontSize(shared_ptr<ZLTextMetrics> metrics) {
    return getFontSize();
//    return FontSizeOption->getValue();
}

 bool ZLTextBaseStyle::isBold()
{
    return BoldOption->getValue();
}
 bool ZLTextBaseStyle::isItalic()
{
  return ItalicOption->getValue();
}
 bool ZLTextBaseStyle::isUnderline()
{
    return UnderlineOption->getValue();
}
 bool ZLTextBaseStyle::isStrikeThrough()
{
    return StrikeThroughOption->getValue();
}
 int ZLTextBaseStyle::getLeftMargin(shared_ptr<ZLTextMetrics> metrics)
{
    return  0;
}
 int ZLTextBaseStyle::getRightMargin(shared_ptr<ZLTextMetrics> metrics)
{
    return  0;
}
 int ZLTextBaseStyle::getLeftPadding(shared_ptr<ZLTextMetrics> metrics)
{
    return 0;
}
 int ZLTextBaseStyle::getRightPadding(shared_ptr<ZLTextMetrics> metrics)
{
    return  0;
}

 int ZLTextBaseStyle::getFirstLineIndent(shared_ptr<ZLTextMetrics> metrics)
{
    return  0;
}

int ZLTextBaseStyle::getLineSpacePercent() {
    return LineSpaceOption->getValue() * 13;
}

int ZLTextBaseStyle::getVerticalAlign(shared_ptr<ZLTextMetrics> metrics)
{
   return  0;
}
 bool* ZLTextBaseStyle::isVerticallyAligned()
{
    return 0;
}
 int ZLTextBaseStyle::getSpaceBefore(shared_ptr<ZLTextMetrics> metrics)
{
    return  0;
}
 int ZLTextBaseStyle::getSpaceAfter(shared_ptr<ZLTextMetrics> metrics)
{
    return  0;
}
 unsigned char ZLTextBaseStyle::getAlignment()
{
    return (unsigned char)AlignmentOption->getValue();
    //return  0;
}

bool
ZLTextBaseStyle::allowHyphenations()
{
    return true;
}
std::vector<shared_ptr<ZDFontEntry>>
ZLTextBaseStyle::getFontEntries() {
    std::string family = FontFamilyOption->getValue();
    /*if (myFontEntries.size()==0|| !family.equals(myFontFamily)) {
        myFontEntries = Collections.singletonList(FontEntry.systemEntry(family));
    }*/
    return myFontEntries;
}
