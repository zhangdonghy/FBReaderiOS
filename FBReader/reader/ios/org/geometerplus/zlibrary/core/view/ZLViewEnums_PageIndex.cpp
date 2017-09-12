//
//  ZLViewEnums_PageIndex.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/31.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ZLViewEnums_PageIndex.hpp"

ZLViewEnums_PageIndex::ZLViewEnums_PageIndex(int v)
{
    value = v;
}
int ZLViewEnums_PageIndex::getNext()
{
    switch (value) {
        case previous:
            return current;
        case current:
            return next;
        default:
            return invalid;
    }
}
int ZLViewEnums_PageIndex::getPrevious(){
    switch (value) {
        case next:
            return current;
        case current:
            return previous;
        default:
            return -1;
    }
}
int ZLViewEnums_PageIndex::getValue()
{
    return value;
}