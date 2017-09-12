//
//  ZLTextNGStyleDescription.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/13.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLTextNGStyleDescription_hpp
#define ZLTextNGStyleDescription_hpp

#include <stdio.h>

#include <map>
#include <string>
#include "shared_ptr.h"
#include "ZDZLTextStyleEntry.hpp"
#include "ZLTextStyleEntry.h"
#include "ZLStringOption.hpp"
#include "ZLTextAlignmentType.h"

class ZLStringOption;
class ZLTextNGStyleDescription
{
private:
    shared_ptr<ZLStringOption> createOption(std::string selector,std::string name,
                                            std::map<std::string,std::string>* valueMap);
    
    static shared_ptr<ZDZLTextStyleEntry::LengthType> parseLength(std::string value) ;
    
    // cache
    //static final Map<String,Object> ourCache = new HashMap<String,Object>();
    static std::map<std::string,shared_ptr<ZDZLTextStyleEntry::LengthType>> ourCache;
    static shared_ptr<ZDZLTextStyleEntry::LengthType>  ourNullObject;
public:
    std::string Name;
    shared_ptr<ZLStringOption> FontFamilyOption;
    shared_ptr<ZLStringOption> FontSizeOption;
    shared_ptr<ZLStringOption> FontWeightOption;
    shared_ptr<ZLStringOption> FontStyleOption;
    shared_ptr<ZLStringOption> TextDecorationOption;
    shared_ptr<ZLStringOption> HyphenationOption;
    shared_ptr<ZLStringOption> MarginTopOption;
    shared_ptr<ZLStringOption> MarginBottomOption;
    shared_ptr<ZLStringOption> MarginLeftOption;
    shared_ptr<ZLStringOption> MarginRightOption;
    shared_ptr<ZLStringOption> TextIndentOption;
    shared_ptr<ZLStringOption> AlignmentOption;
    shared_ptr<ZLStringOption> VerticalAlignOption;
    shared_ptr<ZLStringOption> LineHeightOption;
    
    ZLTextNGStyleDescription(std::string selector,std::map<std::string,std::string>* valueMap);
    
    int getFontSize(shared_ptr<ZLTextMetrics> metrics, int parentFontSize) {
        shared_ptr<ZDZLTextStyleEntry::LengthType> length = parseLength(FontSizeOption->getValue());
        if (length == 0) {
            return parentFontSize;
        }
        return ZDZLTextStyleEntry::compute(
                                           *length, metrics, parentFontSize, ZLTextStyleEntry::LENGTH_FONT_SIZE
                                        );
    }
    
    ZLBoolean3 allowHyphenations() {
        std::string hyphen = HyphenationOption->getValue();
        if (hyphen.compare("auto")==0) {
            return B3_TRUE;
        } else if (hyphen.compare("none")==0) {
            return B3_FALSE;
        } else {
            return B3_UNDEFINED;
        }
    }
    unsigned char getAlignment() {
        std::string alignment = AlignmentOption->getValue();
        if (alignment.length() == 0) {
            return ALIGN_UNDEFINED;
        } else if (alignment.compare("center")==0) {
            return ALIGN_CENTER;
        } else if (alignment.compare("left")==0) {
            return ALIGN_LEFT;
        } else if (alignment.compare("right")==0) {
            return ALIGN_RIGHT;
        } else if (alignment.compare("justify")==0) {
            return ALIGN_JUSTIFY;
        } else {
            return ALIGN_UNDEFINED;
        }
    }
    ZLBoolean3 isItalic() {
        std::string fontStyle = FontStyleOption->getValue();
        if (fontStyle.compare("italic")==0 || fontStyle.compare("oblique")==0) {
            return B3_TRUE;
        } else if (fontStyle.compare("normal")==0) {
            return B3_FALSE;
        } else {
            return B3_UNDEFINED;
        }
    }
    ZLBoolean3 isBold() {
        std::string fontWeight = FontWeightOption->getValue();
        if (fontWeight.compare("bold")==0) {
            return B3_TRUE;
        } else if (fontWeight.compare("normal")==0) {
            return B3_FALSE;
        } else {
            return B3_UNDEFINED;
        }
    }
    ZLBoolean3 isUnderlined() {
        std::string textDecoration = TextDecorationOption->getValue();
        if (textDecoration.compare("underline")==0) {
            return B3_TRUE;
        } else if (textDecoration.compare("")==0 || textDecoration.compare("inherit")==0) {
            return B3_UNDEFINED;
        } else {
            return B3_FALSE;
        }
    }
    ZLBoolean3 isStrikedThrough() {
        std::string textDecoration = TextDecorationOption->getValue();
        if (textDecoration.compare("line-through")==0) {
            return B3_TRUE;
        } else if (textDecoration.compare("")==0
                   || textDecoration.compare("inherit")==0) {
            return B3_UNDEFINED;
        } else {
            return B3_FALSE;
        }
    }
    
    int getVerticalAlign(shared_ptr<ZLTextMetrics> metrics, int base, int fontSize) {
        shared_ptr<ZDZLTextStyleEntry::LengthType> length = parseLength(
                                        VerticalAlignOption->getValue());
        if (length == 0) {
            return base;
        }
        return ZDZLTextStyleEntry::compute(
                                        // TODO: add new length for vertical alignment
                                        *length, metrics, fontSize,
                                           ZLTextStyleEntry::LENGTH_FONT_SIZE
                                        );
    }
    
    bool hasNonZeroVerticalAlign() {
       shared_ptr<ZDZLTextStyleEntry::LengthType> length = parseLength(VerticalAlignOption->getValue());
        return length != 0 && length->Size != 0;
    }
    
    int getLeftMargin(shared_ptr<ZLTextMetrics> metrics, int base, int fontSize) {
        shared_ptr<ZDZLTextStyleEntry::LengthType> length = parseLength(MarginLeftOption->getValue());
        if (length == 0) {
            return base;
        }
        return base + ZDZLTextStyleEntry::compute(
                                                  *length, metrics, fontSize,
                                                  ZLTextStyleEntry::LENGTH_MARGIN_LEFT
                                               );
    }
    
    int getRightMargin(shared_ptr<ZLTextMetrics> metrics, int base, int fontSize) {
        shared_ptr<ZDZLTextStyleEntry::LengthType> length = parseLength(MarginRightOption->getValue());
        if (length == 0) {
            return base;
        }
        return base + ZDZLTextStyleEntry::compute(
                                               *length, metrics, fontSize,
                                                ZLTextStyleEntry::LENGTH_MARGIN_RIGHT
                                               );
    }
    
    int getLeftPadding(shared_ptr<ZLTextMetrics> metrics, int base, int fontSize) {
        return base;
    }
    
    int getRightPadding(shared_ptr<ZLTextMetrics> metrics, int base, int fontSize) {
        return base;
    }
    
    int getFirstLineIndent(shared_ptr<ZLTextMetrics> metrics, int base, int fontSize) {
       shared_ptr<ZDZLTextStyleEntry::LengthType> length = parseLength(TextIndentOption->getValue());
        if (length == 0) {
            return base;
        }
        return ZDZLTextStyleEntry::compute(
                                        *length, metrics, fontSize,  ZLTextStyleEntry::LENGTH_FIRST_LINE_INDENT
                                        );
    }
    
    int getSpaceBefore(shared_ptr<ZLTextMetrics> metrics, int base, int fontSize) {
        shared_ptr<ZDZLTextStyleEntry::LengthType> length = parseLength(MarginTopOption->getValue());
        if (length == 0) {
            return base;
        }
        return ZDZLTextStyleEntry::compute(
                                        *length, metrics, fontSize,  ZLTextStyleEntry::LENGTH_SPACE_BEFORE
                                        );
    }
    
    int getSpaceAfter(shared_ptr<ZLTextMetrics> metrics, int base, int fontSize) {
        shared_ptr<ZDZLTextStyleEntry::LengthType> length = parseLength(MarginBottomOption->getValue());
        if (length == 0) {
            return base;
        }
        return ZDZLTextStyleEntry::compute(
                                        *length, metrics, fontSize,
                                        ZLTextStyleEntry::LENGTH_SPACE_AFTER
                                        );
    }
};
#endif /* ZLTextNGStyleDescription_hpp */
