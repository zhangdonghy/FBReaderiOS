//
//  ZLRect.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/8.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLRect_hpp
#define ZLRect_hpp

#include <stdio.h>

class ZLRect
{
    public:
    int Left;
    int Top;
    int Right;
    int Bottom;
    ZLRect(int left,int top,int right,int bottom){
        Left = left;
        Right = right;
        Top = top;
        Bottom = bottom;
    }
    ZLRect(ZLRect &r){
        Left = r.Left;
        Right = r.Right;
        Top = r.Top;
        Bottom = r.Bottom;
    }
};
#endif /* ZLRect_hpp */
