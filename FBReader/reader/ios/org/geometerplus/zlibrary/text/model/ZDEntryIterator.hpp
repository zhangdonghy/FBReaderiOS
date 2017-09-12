//
//  ZDEntryIterator.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/24.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZDEntryIterator_hpp
#define ZDEntryIterator_hpp
#include "ZDZLImageEntry.hpp"
#include "ZDZLVideoEntry.hpp"
#include "ZDExtensionEntry.hpp"
#include "ZDZLTextStyleEntry.hpp"

#include <stdio.h>
#include <string>

class ZDEntryIterator{
public:
    virtual unsigned char getType()=0;
    
    virtual short* getTextData()=0;
    virtual int getTextOffset()=0;
    virtual int getTextLength()=0;
    
    virtual unsigned char getControlKind()=0;
    virtual bool getControlIsStart()=0;
    
    virtual unsigned char getHyperlinkType()=0;
    virtual std::string getHyperlinkId()=0;
    
    virtual ZDZLImageEntry getImageEntry()=0;
    virtual ZDZLVideoEntry getVideoEntry()=0;
    virtual  ZDExtensionEntry getExtensionEntry()=0;
    virtual ZDZLTextStyleEntry getStyleEntry()=0;
    
    virtual short getFixedHSpaceLength()=0;
    
    virtual bool next()=0;
};

#endif /* ZDEntryIterator_hpp */
