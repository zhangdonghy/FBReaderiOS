//
//  ZLTextBaseStyle.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/3.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLTextBaseStyle_hpp
#define ZLTextBaseStyle_hpp

#include <stdio.h>
#include <string>
#include "ZLTextStyle.hpp"
#include "ZLBooleanOption.hpp"
#include "ZLIntegerRangeOption.hpp"
#include "ZLStringOption.hpp"
#include "ZDFontEntry.hpp"


class ZLTextBaseStyle : public ZLTextStyle
{
    private:
    
    static  std::string GROUP ;//= "Style";
    static  std::string OPTIONS ;//= "Options";
    
    std::string myFontFamily;
    std::vector<shared_ptr<ZDFontEntry>> myFontEntries;

public:
    
    virtual bool isBaseStyle(){
        return true;
    }
    std::vector<shared_ptr<ZDFontEntry>> getFontEntries() ;
    
    ZLBooleanOption* UseCSSTextAlignmentOption;// =
    //new ZLBooleanOption("Style", "css:textAlignment", true);
    ZLBooleanOption* UseCSSMarginsOption ;//=
   // new ZLBooleanOption("Style", "css:margins", true);
    ZLBooleanOption* UseCSSFontSizeOption ;//=
    //new ZLBooleanOption("Style", "css:fontSize", true);
    ZLBooleanOption* UseCSSFontFamilyOption ;//=
    //new ZLBooleanOption("Style", "css:fontFamily", true);
    
    ZLBooleanOption* AutoHyphenationOption ;//=
    //new ZLBooleanOption(OPTIONS, "AutoHyphenation", true);
    
    ZLBooleanOption* BoldOption;
    ZLBooleanOption* ItalicOption;
    ZLBooleanOption* UnderlineOption;
    ZLBooleanOption* StrikeThroughOption;
    ZLIntegerRangeOption* AlignmentOption;
    ZLIntegerRangeOption* LineSpaceOption;
    
    ZLStringOption* FontFamilyOption;
    ZLIntegerRangeOption* FontSizeOption;
    std::string FontName;
    
    
    ZLTextBaseStyle(std::string prefix,std::string fontFamily,int fontSize);
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
    virtual unsigned char getAlignment();
    int getFontSize();
    virtual bool allowHyphenations();
  //  virtual std::vector<shared_ptr<ZDFontEntry>> getFontEntries();
    /*shared_ptr<ZLTextStyle> base(){
        return myBase;
    }
    shared_ptr<ZLTextStyle> myBase;*/
};

#endif /* ZLTextBaseStyle_hpp */
