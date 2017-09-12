//
//  ZDHash.c
//  ZDStudent
//
//  Created by zhangdong on 16/9/1.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ZDHash.hpp"

int GetZDHash(std::string str)
{
    int val =0;
    int length = (int)str.length();
    if(length ==0) return 0;
    for(int i=0;i<length;i++){
        val += (str.at(i)*31^(length-i-1));
    }
    return  val;
}