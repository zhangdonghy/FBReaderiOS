//
//  ZLTextExplicitlyDecoratedStyle.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/14.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ZLTextExplicitlyDecoratedStyle.hpp"
#include "ZLTextBaseStyle.hpp"
#include "ZDZLTextStyleEntry.hpp"
#include "ZLTextStyleEntry.h"


shared_ptr<ZLTextStyle>
ZLTextExplicitlyDecoratedStyle::computeTreeParent() {
    if (myEntry->Depth == 0) {
        return Parent->Parent;
    }
    int count = 0;
    shared_ptr<ZLTextStyle> p = Parent;
    for (; p != p->Parent; p = p->Parent) {
        //if (p instanceof ZLTextExplicitlyDecoratedStyle) {
        if (p->styleKind() == KIND_Explicit) {
            
            if (((ZLTextExplicitlyDecoratedStyle&)*p).myEntry->Depth != myEntry->Depth) {
                return p;
            }
            
        } else {
            if (++count > 1) {
                return p;
            }
        }
    }
    return p;
}
unsigned char
ZLTextExplicitlyDecoratedStyle::getAlignment()
{
    return 0;
   /* if (myEntry instanceof ZLTextCSSStyleEntry && !BaseStyle.UseCSSTextAlignmentOption.getValue()) {
        return Parent.getAlignment();
    }
    return
    myEntry.isFeatureSupported(ALIGNMENT_TYPE)
				? myEntry.getAlignmentType()
				: Parent.getAlignment();*/
}
bool
ZLTextExplicitlyDecoratedStyle::allowHyphenations()
{
    return  Parent->allowHyphenations();
}

int
ZLTextExplicitlyDecoratedStyle::getLineSpacePercentInternal()
{
    return Parent->getLineSpacePercent();
}
bool
ZLTextExplicitlyDecoratedStyle::isItalicInternal()
{
    switch (myEntry->getFontModifier(ZLTextStyleEntry::FONT_MODIFIER_ITALIC)) {
        case B3_TRUE:
            return true;
        case B3_FALSE:
            return false;
        default:
            return Parent->isItalic();
    }
}
bool
ZLTextExplicitlyDecoratedStyle::isBoldInternal()
{
    switch (myEntry->getFontModifier(ZLTextStyleEntry::FONT_MODIFIER_BOLD)) {
        case B3_TRUE:
            return true;
        case B3_FALSE:
            return false;
        default:
            return Parent->isBold();
    }
}
bool
ZLTextExplicitlyDecoratedStyle::isUnderlineInternal()
{
    switch (myEntry->getFontModifier(ZLTextStyleEntry::FONT_MODIFIER_UNDERLINED)) {
        case B3_TRUE:
            return true;
        case B3_FALSE:
            return false;
        default:
            return Parent->isUnderline();
    }
}
bool
ZLTextExplicitlyDecoratedStyle::isStrikeThroughInternal()
{
    switch (myEntry->getFontModifier(ZLTextStyleEntry::FONT_MODIFIER_STRIKEDTHROUGH)) {
        case B3_TRUE:
            return true;
        case B3_FALSE:
            return false;
        default:
            return Parent->isStrikeThrough();
    }
}

std::vector<shared_ptr<ZDFontEntry>>
ZLTextExplicitlyDecoratedStyle::getFontEntriesInternal()
{
    std::vector<shared_ptr<ZDFontEntry>> parentEntries = Parent->getFontEntries();//getFontEntries();
    if (myEntry->entryKind() == ZDZLTextStyleEntry::CSS_Entry//myEntry instanceof ZLTextCSSStyleEntry
        //&& !BaseStyle.UseCSSFontFamilyOption.getValue()
        &&  ((ZLTextBaseStyle&)(*BaseStyle)).UseCSSFontFamilyOption->getValue()
        ) {
        return parentEntries;
    }
    
    if (!myEntry->isFeatureSupported(ZLTextStyleEntry::FONT_FAMILY)) {
        return parentEntries;
    }
    
    std::vector<shared_ptr<ZDFontEntry>>  entries = myEntry->getFontEntries();
    long lSize = entries.size();
    if (lSize == 0) {
        return parentEntries;
    }
    
    long pSize = parentEntries.size();
    if (pSize > lSize /*&& entries.equals(parentEntries.subList(0, lSize))*/) {
        bool equal = true;
        for(int i=0;i<lSize;i++){
            if(!parentEntries[i]->equals(entries[i])) {
                equal = false;
                break;
            }
        }
        if(equal)
        return parentEntries;
    }
    
    std::vector<shared_ptr<ZDFontEntry>> allEntries ;//= new ArrayList<FontEntry>(pSize + lSize);
   
     //allEntries.addAll(entries);
    for(int i=0;i<entries.size();i++){
        allEntries.insert(allEntries.end(), entries[i]);
    }
   
    //allEntries.addAll(parentEntries);
    for(int i=0;i<parentEntries.size();i++){
        allEntries.insert(allEntries.end(), parentEntries[i]);
    }
    return allEntries;
}

int
ZLTextExplicitlyDecoratedStyle::getFontSizeInternal(shared_ptr<ZLTextMetrics> metrics)
{
    //if (myEntry->entryKind() ==  ZLTextCSSStyleEntry && !BaseStyle.UseCSSFontSizeOption.getValue()) {
    if (myEntry->entryKind() == ZDZLTextStyleEntry::CSS_Entry
        &&  !((ZLTextBaseStyle&)(*BaseStyle)).UseCSSFontSizeOption->getValue()
            //&& !BaseStyle->UseCSSFontSizeOption.getValue()
        
            ) {
        return Parent->getFontSize(metrics);
    }
    
    int baseFontSize = getTreeParent()->getFontSize(metrics);
    if (myEntry->isFeatureSupported(ZLTextStyleEntry::FONT_STYLE_MODIFIER)) {
        if (myEntry->getFontModifier(ZLTextStyleEntry::FONT_MODIFIER_INHERIT) == B3_TRUE) {
            return baseFontSize;
        }
        if (myEntry->getFontModifier(ZLTextStyleEntry::FONT_MODIFIER_LARGER) == B3_TRUE) {
            return baseFontSize * 120 / 100;
        }
        if (myEntry->getFontModifier(ZLTextStyleEntry::FONT_MODIFIER_SMALLER) == B3_TRUE) {
            return baseFontSize * 100 / 120;
        }
    }
    if (myEntry->isFeatureSupported(ZLTextStyleEntry::LENGTH_FONT_SIZE)) {
        return myEntry->getLength(ZLTextStyleEntry::LENGTH_FONT_SIZE, metrics, baseFontSize);
    }
    return Parent->getFontSize(metrics);
}
int
ZLTextExplicitlyDecoratedStyle::getSpaceBeforeInternal(shared_ptr<ZLTextMetrics> metrics, int fontSize)
{
    if (myEntry->entryKind() ==  ZDZLTextStyleEntry::CSS_Entry// instanceof ZLTextCSSStyleEntry
        //    && !BaseStyle->UseCSSMarginsOption.getValue()
         &&((ZLTextBaseStyle&)(*BaseStyle)).UseCSSMarginsOption->getValue()
        ) {
        return Parent->getSpaceBefore(metrics);
    }
    
    if (!myEntry->isFeatureSupported(ZLTextStyleEntry::LENGTH_SPACE_BEFORE)) {
        return Parent->getSpaceBefore(metrics);
    }
    return myEntry->getLength(ZLTextStyleEntry::LENGTH_SPACE_BEFORE, metrics, fontSize);
}
int
ZLTextExplicitlyDecoratedStyle::getSpaceAfterInternal(shared_ptr<ZLTextMetrics> metrics, int fontSize)
{
    if (myEntry->entryKind() ==  ZDZLTextStyleEntry::CSS_Entry//myEntry instanceof ZLTextCSSStyleEntry
        &&((ZLTextBaseStyle&)(*BaseStyle)).UseCSSMarginsOption->getValue()
        //&& !BaseStyle.UseCSSMarginsOption.getValue()
        
        ) {
        return Parent->getSpaceAfter(metrics);
    }
    
    if (!myEntry->isFeatureSupported(ZLTextStyleEntry::LENGTH_SPACE_AFTER)) {
        return Parent->getSpaceAfter(metrics);
    }
    return myEntry->getLength(ZLTextStyleEntry::LENGTH_SPACE_AFTER, metrics, fontSize);
}
int
ZLTextExplicitlyDecoratedStyle::getVerticalAlignInternal(shared_ptr<ZLTextMetrics> metrics, int fontSize)
{
    // TODO: implement
    if (myEntry->isFeatureSupported(ZLTextStyleEntry::LENGTH_VERTICAL_ALIGN)) {
        return myEntry->getLength(ZLTextStyleEntry::LENGTH_VERTICAL_ALIGN, metrics, fontSize);
    } else if (myEntry->isFeatureSupported(ZLTextStyleEntry::NON_LENGTH_VERTICAL_ALIGN)) {
        switch (myEntry->getVerticalAlignCode()) {
            default:
                return Parent->getVerticalAlign(metrics);
            case 0: // sub
            {
                ZDZLTextStyleEntry::LengthType xx;//(ZLTextStyleEntry::SIZE_UNIT_EM_100,(short)50);
                xx.Unit =ZLTextStyleEntry::SIZE_UNIT_EM_100;
                xx.Size = (short)-50;
                return ZDZLTextStyleEntry::compute(
                                                   xx,
                                                metrics, fontSize, ZLTextStyleEntry::LENGTH_VERTICAL_ALIGN
                                                );
            }
            case 1: // super
            {
                ZDZLTextStyleEntry::LengthType xx;//(ZLTextStyleEntry::SIZE_UNIT_EM_100,(short)50);
                xx.Unit =ZLTextStyleEntry::SIZE_UNIT_EM_100;
                xx.Size = 50;
                return ZDZLTextStyleEntry::compute(
                                                   xx,
                                                   metrics, fontSize, ZLTextStyleEntry::LENGTH_VERTICAL_ALIGN
                                                   );
            }
                
                /*
                 case 2: // top
                 return 0;
                 case 3: // text-top
                 return 0;
                 case 4: // middle
                 return 0;
                 case 5: // bottom
                 return 0;
                 case 6: // text-bottom
                 return 0;
                 case 7: // initial
                 return 0;
                 case 8: // inherit
                 return 0;
                 */
        }
    } else {
        return Parent->getVerticalAlign(metrics);
    }
}
int
ZLTextExplicitlyDecoratedStyle::getLeftMarginInternal(shared_ptr<ZLTextMetrics> metrics, int fontSize)
{
    if (myEntry->entryKind() ==  ZDZLTextStyleEntry::CSS_Entry//myEntry instanceof ZLTextCSSStyleEntry
         &&((ZLTextBaseStyle&)(*BaseStyle)).UseCSSMarginsOption->getValue()
        //&& !BaseStyle.UseCSSMarginsOption.getValue()
        ) {
        return Parent->getLeftMargin(metrics);
    }
    
    if (!myEntry->isFeatureSupported(ZLTextStyleEntry::LENGTH_MARGIN_LEFT)) {
        return Parent->getLeftMargin(metrics);
    }
    return getTreeParent()->getLeftMargin(metrics) + myEntry->getLength(ZLTextStyleEntry::LENGTH_MARGIN_LEFT, metrics, fontSize);
}
int
ZLTextExplicitlyDecoratedStyle::getRightMarginInternal(shared_ptr<ZLTextMetrics> metrics, int fontSize)
{
    if (//myEntry instanceof ZLTextCSSStyleEntry
        myEntry->entryKind() ==  ZDZLTextStyleEntry::CSS_Entry
        &&((ZLTextBaseStyle&)(*BaseStyle)).UseCSSMarginsOption->getValue()
        //&& !BaseStyle.UseCSSMarginsOption.getValue()
        
        ) {
        return Parent->getRightMargin(metrics);
    }
    
    if (!myEntry->isFeatureSupported(ZLTextStyleEntry::LENGTH_MARGIN_RIGHT)) {
        return Parent->getRightMargin(metrics);
    }
    return getTreeParent()->getRightMargin(metrics) + myEntry->getLength(ZLTextStyleEntry::LENGTH_MARGIN_RIGHT, metrics, fontSize);
}
int
ZLTextExplicitlyDecoratedStyle::getLeftPaddingInternal(shared_ptr<ZLTextMetrics> metrics, int fontSize)
{
    if (//myEntry instanceof ZLTextCSSStyleEntry
        myEntry->entryKind() ==  ZDZLTextStyleEntry::CSS_Entry
        &&((ZLTextBaseStyle&)(*BaseStyle)).UseCSSMarginsOption->getValue()
        //&& !BaseStyle.UseCSSMarginsOption.getValue()
        
        ) {
        return Parent->getLeftPadding(metrics);
    }
    
    if (!myEntry->isFeatureSupported(ZLTextStyleEntry::LENGTH_PADDING_LEFT)) {
        return Parent->getLeftPadding(metrics);
    }
    return getTreeParent()->getLeftPadding(metrics) + myEntry->getLength(ZLTextStyleEntry::LENGTH_PADDING_LEFT, metrics, fontSize);
}
int
ZLTextExplicitlyDecoratedStyle::getRightPaddingInternal(shared_ptr<ZLTextMetrics> metrics, int fontSize)
{
    if (
        myEntry->entryKind() ==  ZDZLTextStyleEntry::CSS_Entry
        //myEntry instanceof ZLTextCSSStyleEntry
         &&((ZLTextBaseStyle&)(*BaseStyle)).UseCSSMarginsOption->getValue()
        //&& !BaseStyle.UseCSSMarginsOption.getValue()
        
        ) {
        return Parent->getRightPadding(metrics);
    }
    
    if (!myEntry->isFeatureSupported(ZLTextStyleEntry::LENGTH_PADDING_RIGHT)) {
        return Parent->getRightPadding(metrics);
    }
    return getTreeParent()->getRightPadding(metrics) + myEntry->getLength(ZLTextStyleEntry::LENGTH_PADDING_RIGHT, metrics, fontSize);
}
int
ZLTextExplicitlyDecoratedStyle::getFirstLineIndentInternal(shared_ptr<ZLTextMetrics> metrics, int fontSize)
{
    if (myEntry->entryKind() ==  ZDZLTextStyleEntry::CSS_Entry//myEntry instanceof ZLTextCSSStyleEntry &&
       // !BaseStyle.UseCSSMarginsOption.getValue()
        &&!((ZLTextBaseStyle&)(*BaseStyle)).UseCSSMarginsOption->getValue()
        ) {
        return Parent->getFirstLineIndent(metrics);
    }
    
    if (!myEntry->isFeatureSupported(ZLTextStyleEntry::LENGTH_FIRST_LINE_INDENT)) {
        return Parent->getFirstLineIndent(metrics);
    }
    return myEntry->getLength(ZLTextStyleEntry::LENGTH_FIRST_LINE_INDENT, metrics, fontSize);
}
bool
ZLTextExplicitlyDecoratedStyle::isVerticallyAlignedInternal()
{
    if (myEntry->isFeatureSupported(ZLTextStyleEntry::LENGTH_VERTICAL_ALIGN)) {
        return myEntry->hasNonZeroLength(ZLTextStyleEntry::LENGTH_VERTICAL_ALIGN);
    } else if (myEntry->isFeatureSupported(ZLTextStyleEntry::NON_LENGTH_VERTICAL_ALIGN)) {
        switch (myEntry->getVerticalAlignCode()) {
            default:
                return false;
            case 0: // sub
            case 1: // super
                return true;
        }
    } else {
        return false;
    }
}




