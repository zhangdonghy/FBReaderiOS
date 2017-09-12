//
//  ZDEntryIteratorImpl.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/25.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZDEntryIteratorImpl_hpp
#define ZDEntryIteratorImpl_hpp

#include "ZDEntryIterator.hpp"
#include <stdio.h>

class ZDEntryIteratorImpl :public ZDEntryIterator
{
private:
     short* myTextData;
    ZDZLImageEntry myImageEntry;
    ZDZLVideoEntry myVideoEntry;
    ZDExtensionEntry myExtensionEntry;
    ZDZLTextStyleEntry myStyleEntry;
    short myFixedHSpaceLength;
public:
    virtual unsigned char getType();
    
    virtual short* getTextData();
    virtual int getTextOffset();
    virtual int getTextLength();
    
    virtual unsigned char getControlKind();
    virtual bool getControlIsStart();
    
    virtual unsigned char getHyperlinkType();
    virtual std::string getHyperlinkId();
    
    virtual ZDZLImageEntry getImageEntry();
    virtual ZDZLVideoEntry getVideoEntry();
    virtual  ZDExtensionEntry getExtensionEntry();
    virtual ZDZLTextStyleEntry getStyleEntry();
    
    virtual short getFixedHSpaceLength();
    
    virtual bool next();
};
#endif /* ZDEntryIteratorImpl_hpp */
