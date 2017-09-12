//
//  ZLTextNGStyleDescription.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/13.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ZLTextNGStyleDescription.hpp"
#include "ZLStringOption.hpp"
#include "IOSUtil.h"


shared_ptr<ZDZLTextStyleEntry::LengthType> ZLTextNGStyleDescription::ourNullObject
= new ZDZLTextStyleEntry::LengthType();

std::map<std::string,shared_ptr<ZDZLTextStyleEntry::LengthType>>  ZLTextNGStyleDescription::ourCache;
ZLTextNGStyleDescription::ZLTextNGStyleDescription(std::string selector,std::map<std::string,std::string>* valueMap)
{
    std::map<std::string,std::string>::const_iterator it =  valueMap->find("fbreader-name");
    Name = it->second;
    
    FontFamilyOption = createOption(selector, "font-family", valueMap);
    FontSizeOption = createOption(selector, "font-size", valueMap);
    FontWeightOption = createOption(selector, "font-weight", valueMap);
    FontStyleOption = createOption(selector, "font-style", valueMap);
    TextDecorationOption = createOption(selector, "text-decoration", valueMap);
    HyphenationOption = createOption(selector, "hyphens", valueMap);
    MarginTopOption = createOption(selector, "margin-top", valueMap);
    MarginBottomOption = createOption(selector, "margin-bottom", valueMap);
    MarginLeftOption = createOption(selector, "margin-left", valueMap);
    MarginRightOption = createOption(selector, "margin-right", valueMap);
    TextIndentOption = createOption(selector, "text-indent", valueMap);
    AlignmentOption = createOption(selector, "text-align", valueMap);
    VerticalAlignOption = createOption(selector, "vertical-align", valueMap);
    LineHeightOption = createOption(selector, "line-height", valueMap);
}
shared_ptr<ZLStringOption>
ZLTextNGStyleDescription::createOption(std::string selector,std::string name,std::map<std::string,std::string>* valueMap)
{
    std::map<std::string,std::string>::const_iterator it =  valueMap->find(name);
    if(it != valueMap->end()) {
        std::string optionname="";
        optionname += selector;
        optionname +="::";
        optionname += name;
        return  new ZLStringOption(
                                   "Style",
                                   optionname,
                                   it->second
                                   );
    } else
    {
        std::string optionname="";
        optionname += selector;
        optionname +="::";
        optionname += name;
        return  new ZLStringOption(
                                   "Style",
                                   optionname,
                                   ""
                                   );
    }
    
}

shared_ptr<ZDZLTextStyleEntry::LengthType>
ZLTextNGStyleDescription::parseLength(std::string value) {
    if (value.size() == 0) {
        return 0;
    }
    
    std::map<std::string,shared_ptr<ZDZLTextStyleEntry::LengthType>>::const_iterator  cacheditem = ourCache.find(value);
    if(cacheditem != ourCache.end()) {
       //return cached == ourNullObject ? 0 : (ZLTextStyleEntry.Length)cached;
        if(cacheditem->second == ourNullObject) {
            return  0;
        }
        return cacheditem->second;
    }
    
    
   shared_ptr<ZDZLTextStyleEntry::LengthType> length = 0;
    //try {
        
        if (
            //value.endsWith("%")
            IOSUtil::endWithStr(value, "%")
            ) {
            
            length = new ZDZLTextStyleEntry::LengthType(ZLTextStyleEntry::SIZE_UNIT_PERCENT,IOSUtil::StringToInt(value.substr(0,value.size() - 1)));
                                                 //Short.valueOf(value.substring(0, value.length() - 1)),
                                                // ZLTextStyleEntry.SizeUnit.PERCENT
                                                // );
        } else if (
                   IOSUtil::endWithStr(value, "rem")
                   //value.endsWith("rem")
                   ) {
            length = new ZDZLTextStyleEntry::LengthType(ZLTextStyleEntry::SIZE_UNIT_REM_100,
                                                        (short)(100 *IOSUtil::StringToDouble(value.substr(0,value.size()-2)))
                                                        
                                                        );
                                                // (short)(100 * Double.valueOf(value.substring(0, value.length() - 2))),
                                                // ZLTextStyleEntry.SizeUnit.REM_100
                                                // );
        } else if (
                   //value.endsWith("em")
                   IOSUtil::endWithStr(value, "em")
                   ) {
            length = new ZDZLTextStyleEntry::LengthType(
                                                        ZLTextStyleEntry::SIZE_UNIT_EM_100,
                                                 (short)(100 *IOSUtil::StringToDouble(value.substr(0,value.size()-2)))
                                                // ZLTextStyleEntry.SizeUnit.EM_100
                                                 );
        } else if (
                   IOSUtil::endWithStr(value, "ex")
                   //value.endsWith("ex")
                   ) {
            length = new ZDZLTextStyleEntry::LengthType(
                                                      ZLTextStyleEntry::SIZE_UNIT_EX_100,
                                                 (short)(100 *IOSUtil::StringToDouble(value.substr(0,value.size()-2)))
                                                 //ZLTextStyleEntry.SizeUnit.EX_100
                                                 );
        } else if (
                   IOSUtil::endWithStr(value, "px")
                   //value.endsWith("px")
                   ) {
            length = new ZDZLTextStyleEntry::LengthType(
                                                   ZLTextStyleEntry::SIZE_UNIT_PIXEL,
                                                        IOSUtil::StringToInt(value.substr(0,value.size() - 2))
                                                 //Short.valueOf(value.substring(0, value.length() - 2)),
                                               //  ZLTextStyleEntry.SizeUnit.PIXEL
                                                 );
        } else if (
                   IOSUtil::endWithStr(value, "pt")
                   //value.endsWith("pt")
                   ) {
            length = new ZDZLTextStyleEntry::LengthType(
                                                        ZLTextStyleEntry::SIZE_UNIT_POINT,
                                                        IOSUtil::StringToInt(value.substr(0,value.size() - 2))
                                                 //Short.valueOf(value.substring(0, value.length() - 2)),
                                                 //ZLTextStyleEntry.SizeUnit.POINT
                                                 );
        }
    //} catch (Exception e) {
        // ignore
    //}
   // ourCache.put(value, length != null ? length : ourNullObject);
     ourCache.insert(std::make_pair(value, length != 0 ? length : ourNullObject));
    
    return length;
}