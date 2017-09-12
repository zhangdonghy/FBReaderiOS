//
//  ZDEntryIteratorImpl.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/25.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ZDEntryIteratorImpl.hpp"

 unsigned char ZDEntryIteratorImpl::getType()
{
    return 0;
}

 short* ZDEntryIteratorImpl::getTextData()
{
    return myTextData;
}
 int ZDEntryIteratorImpl::getTextOffset()
{
    return 0;
}
 int ZDEntryIteratorImpl::getTextLength()
{
    return 0;
}

 unsigned char ZDEntryIteratorImpl::getControlKind()
{
    return 0;
}
 bool ZDEntryIteratorImpl::getControlIsStart()
{
    return true;
}

 unsigned char ZDEntryIteratorImpl::getHyperlinkType()
{
    return 0;
}
 std::string ZDEntryIteratorImpl::getHyperlinkId()
{
    return "";
}

 ZDZLImageEntry ZDEntryIteratorImpl::getImageEntry()
{
    return myImageEntry;
}
 ZDZLVideoEntry ZDEntryIteratorImpl::getVideoEntry()
{
    return  myVideoEntry;
}
  ZDExtensionEntry ZDEntryIteratorImpl::getExtensionEntry()
{
    return myExtensionEntry;
}
 ZDZLTextStyleEntry ZDEntryIteratorImpl::getStyleEntry()
{
    return  myStyleEntry;
}

 short ZDEntryIteratorImpl::getFixedHSpaceLength()
{
    return myFixedHSpaceLength;
}

 bool ZDEntryIteratorImpl::next()
{
    return true;
}
