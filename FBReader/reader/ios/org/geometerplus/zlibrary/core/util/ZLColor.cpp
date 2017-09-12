//
//  ZLColor.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/31.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ZLColor.hpp"

ZLColor::ZLColor(int r, int g, int b) {
    Red = (short)(r & 0xFF);
    Green = (short)(g & 0xFF);
    Blue = (short)(b & 0xFF);
}

ZLColor::ZLColor(int intValue) {
    Red = (short)((intValue >> 16) & 0xFF);
    Green = (short)((intValue >> 8) & 0xFF);
    Blue = (short)(intValue & 0xFF);
}

int ZLColor::intValue() {
    return (Red << 16) + (Green << 8) + Blue;
}


 bool ZLColor::equals(shared_ptr<ZLColor> o) {
    if (o == this) {
        return true;
    }
    
    return o->Red == Red && o->Green == Green && o->Blue == Blue;
}


 int ZLColor::hashCode() {
    return intValue();
}

std::string ZLColor::toString() {
    /*return new StringBuilder("ZLColor(")
    .append(String.valueOf(Red)).append(", ")
    .append(String.valueOf(Green)).append(", ")
    .append(String.valueOf(Blue)).append(")")
    .toString();*/
    return "Empty";
    
}
