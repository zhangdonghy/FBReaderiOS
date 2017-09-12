//
//  ZDZLibrary.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/31.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ZDZLibrary.hpp"

ZDZLibrary* ZDZLibrary::ourImplementation = 0;

ZDZLibrary* ZDZLibrary::Instance() {
    return ourImplementation;
}


ZDZLibrary::ZDZLibrary() {
    ourImplementation = this;
}