//
//  ZLIntegerRangeOption.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/14.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLIntegerRangeOption_hpp
#define ZLIntegerRangeOption_hpp

#include <stdio.h>
#include <stdlib.h>

#include "ZLOption.hpp"
#include <string>
#include "IOSUtil.h"
class ZLIntegerRangeOption:public ZLOption
{
private:
    int myValue;
    std::string myStringValue;
    static int valueInRange(int value, int min, int max) {
        return std::min(max, std::max(min, value));
    }
public:
    int MinValue;
    int MaxValue;
    ZLIntegerRangeOption(std::string group, std::string optionName, int minValue, int maxValue, int defaultValue)
    : ZLOption(group,optionName,IOSUtil::IntToString(valueInRange(defaultValue, minValue, maxValue)))
    {
       
        //super(group, optionName, String.valueOf(valueInRange(defaultValue, minValue, maxValue)));
        MinValue = minValue;
        MaxValue = maxValue;
    }
    int getValue() {
        std::string stringValue = getConfigValue();
        if (!(stringValue == myStringValue)) {
            myStringValue = stringValue;
            myValue = valueInRange(atoi(stringValue.c_str()), MinValue, MaxValue);
        }
        return myValue;
    }
    
    void setValue(int value) {
        value = valueInRange(value, MinValue, MaxValue);
        myValue = value;
        myStringValue = IOSUtil::IntToString(value);//String.valueOf(value);
        setConfigValue(myStringValue);
    }
};

/*
 private static int valueInRange(int value, int min, int max) {
 return Math.min(max, Math.max(min, value));
	}
 
	public final int MinValue;
	public final int MaxValue;
 
	private int myValue;
	private String myStringValue;
 
	public ZLIntegerRangeOption(String group, String optionName, int minValue, int maxValue, int defaultValue) {
 super(group, optionName, String.valueOf(valueInRange(defaultValue, minValue, maxValue)));
 MinValue = minValue;
 MaxValue = maxValue;
	}
 
	public int getValue() {
 final String stringValue = getConfigValue();
 if (!stringValue.equals(myStringValue)) {
 myStringValue = stringValue;
 try {
 myValue = valueInRange(Integer.parseInt(stringValue), MinValue, MaxValue);
 } catch (NumberFormatException e) {
 }
 }
 return myValue;
	}
 
	public void setValue(int value) {
 value = valueInRange(value, MinValue, MaxValue);
 myValue = value;
 myStringValue = String.valueOf(value);
 setConfigValue(myStringValue);
	}*/
#endif /* ZLIntegerRangeOption_hpp */
