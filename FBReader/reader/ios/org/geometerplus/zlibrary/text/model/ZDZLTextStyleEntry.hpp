//
//  ZDZLTextStyleEntry.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/24.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZDZLTextStyleEntry_hpp
#define ZDZLTextStyleEntry_hpp

#include <stdio.h>
#include <vector>
#include "ZLTextStyleEntry.h"
#include "ZLBoolean3.h"
#include "ZLTextMetrics.hpp"

class ZDFontEntry;

class ZDZLTextStyleEntry{
public:
    struct LengthType{
    public:
        LengthType()
        {
            
        }
        LengthType(ZLTextStyleEntry::SizeUnit  unit,short size)
        {
            Unit = unit;
            Size = size;
        }
        ZLTextStyleEntry::SizeUnit  Unit;
        short Size;
    };
    private:
    uint8_t myFontModifiers;
    uint8_t myAlignmentType;
    uint8_t mySupportedFontModifiers;
    short Depth;
    unsigned short myFeatureMask;
    uint8_t myVerticalAlignCode;
    LengthType myLengths[ZLTextStyleEntry::NUMBER_OF_LENGTHS];
 
    std::vector<shared_ptr<ZDFontEntry>> myFontEntries;
    public:
    enum EntryKind {
        UNKOWN_Entry,
        
        CSS_Entry,
        OTHER_Entry,
    };
    virtual EntryKind entryKind(){
        return UNKOWN_Entry;
    }
   std::vector<shared_ptr<ZDFontEntry>>  getFontEntries() {
        return myFontEntries;
    }
    // virtual
    ZDZLTextStyleEntry(short depth)
    {
        Depth = depth;
        for(int i=0;i<ZLTextStyleEntry::NUMBER_OF_LENGTHS;i++){
            myLengths[i].Size = 0;
            myLengths[i].Unit = ZLTextStyleEntry::SIZE_UNIT_PIXEL;
        }
        myFontModifiers = 0;
        myAlignmentType = 0;;
        mySupportedFontModifiers=0;
    }
    bool hasNonZeroLength(int featureId) {
        return myLengths[featureId].Size != 0;
    }
    void setLength(int featureId, short size, uint8_t unit) {
        myFeatureMask |= 1 << featureId;
        myLengths[featureId].Size = size;
        myLengths[featureId].Unit = (ZLTextStyleEntry::SizeUnit)unit;
    }
    void setAlignmentType(uint8_t alignmentType) {
        myFeatureMask |= 1 << ZLTextStyleEntry::ALIGNMENT_TYPE;
        myAlignmentType = alignmentType;
    }
    void setVerticalAlignCode(uint8_t code) {
        myFeatureMask |= 1 << ZLTextStyleEntry::NON_LENGTH_VERTICAL_ALIGN;
        myVerticalAlignCode = code;
    }
    
    uint8_t getVerticalAlignCode() {
        return myVerticalAlignCode;
    }
    void setFontModifiers(uint8_t supported, uint8_t values) {
        myFeatureMask |= 1 << ZLTextStyleEntry::FONT_STYLE_MODIFIER;
        mySupportedFontModifiers = supported;
        myFontModifiers = values;
    }
    
    void setFontModifier(uint8_t modifier, bool on) {
        myFeatureMask |= 1 << ZLTextStyleEntry::FONT_STYLE_MODIFIER;
        mySupportedFontModifiers |= modifier;
        if (on) {
            myFontModifiers |= modifier;
        } else {
            myFontModifiers &= ~modifier;
        }
    }
    ZLBoolean3 getFontModifier(uint8_t modifier) {
        if ((mySupportedFontModifiers & modifier) == 0) {
            return B3_UNDEFINED;
        }
        return (myFontModifiers & modifier) == 0 ?B3_FALSE : B3_TRUE;
    }
    static bool isFeatureSupported(short mask,int featureId)
    {
        return ((mask&1<<featureId))!=0;
    }
    static int fullSize(shared_ptr<ZLTextMetrics> metrics, int fontSize, int featureId) {
        switch (featureId) {
            default:
            case ZLTextStyleEntry::LENGTH_MARGIN_LEFT:
            case ZLTextStyleEntry::LENGTH_MARGIN_RIGHT:
            case ZLTextStyleEntry::LENGTH_PADDING_LEFT:
            case ZLTextStyleEntry::LENGTH_PADDING_RIGHT:
            case ZLTextStyleEntry::LENGTH_FIRST_LINE_INDENT:
                return metrics->FullWidth;
            case ZLTextStyleEntry::LENGTH_SPACE_BEFORE:
            case ZLTextStyleEntry::LENGTH_SPACE_AFTER:
                return metrics->FullHeight;
            case ZLTextStyleEntry::LENGTH_VERTICAL_ALIGN:
            case ZLTextStyleEntry::LENGTH_FONT_SIZE:
                return fontSize;
        }
    }
 
     static int compute(LengthType& length, shared_ptr<ZLTextMetrics> metrics, int fontSize, int featureId) {
        switch (length.Unit) {
            default:
            case ZLTextStyleEntry::SIZE_UNIT_PIXEL:
                return length.Size;
            case ZLTextStyleEntry::SIZE_UNIT_POINT:
                return length.Size * metrics->DPI / 72;
            case ZLTextStyleEntry::SIZE_UNIT_EM_100:
                return (length.Size * fontSize + 50) / 100;
            case ZLTextStyleEntry::SIZE_UNIT_REM_100:
                return (length.Size * metrics->FontSize + 50) / 100;
            case ZLTextStyleEntry::SIZE_UNIT_EX_100:
                // TODO 0.5 font size => height of x
                return (length.Size * fontSize / 2 + 50) / 100;
            case ZLTextStyleEntry::SIZE_UNIT_PERCENT:
                return (length.Size * fullSize(metrics, fontSize, featureId) + 50) / 100;
        }
    }
    bool isFeatureSupported(int featureId) {
        return isFeatureSupported(myFeatureMask, featureId);
    }
    int getLength(int featureId, shared_ptr<ZLTextMetrics> metrics, int fontSize) {
        return compute(myLengths[featureId], metrics, fontSize, featureId);
    }
    
    friend class ZLTextExplicitlyDecoratedStyle;
};
#endif /* ZDZLTextStyleEntry_hpp */
