//
//  BitmapManager.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/31.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef BitmapManager_hpp
#define BitmapManager_hpp

#import <UIKit/UIKit.h>
#include "ZLViewEnums_PageIndex.hpp"

class  BitmapManager {
    
public:
    virtual UIImage* getBitmap(ZLViewEnums_PageIndex* index)=0;
    virtual void drawBitmap(/*Canvas canvas, */int x, int y, ZLViewEnums_PageIndex index/*, Paint paint*/)=0;
};

#endif /* BitmapManager_hpp */
