//
//  Hull.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/1.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef Hull_hpp
#define Hull_hpp

#include <stdio.h>
#include "ZLPaintContext.hpp"

class Hull {
public:
    enum DrawMode {
        None = 0,
        Outline = 1,
        Fill = 2,
        Underline = 3,
        Speech = 4,
    };

    virtual void draw(shared_ptr<ZLPaintContext> context, int mode, shared_ptr<ZLColor> color, int noteCount = 0) = 0;
    virtual int distanceTo(int x, int y) = 0;
    virtual bool isBefore(int x, int y) = 0;
};
#endif /* Hull_hpp */
