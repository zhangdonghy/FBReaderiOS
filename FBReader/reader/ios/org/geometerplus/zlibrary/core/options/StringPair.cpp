//
//  StringPair.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/3.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "StringPair.hpp"
#include "ZDHash.hpp"

int StringPair::hashCode() {
    return  GetZDHash(Group) + 37 *  GetZDHash(Name);
}
