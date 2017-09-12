//
//  ZLSize.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/31.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLSize_hpp
#define ZLSize_hpp

#include <stdio.h>
#include <string>

class ZLSize
{
public:
    int Width;
    int Height;
    
    ZLSize(int w, int h);
    virtual bool equals(ZLSize size);
    
    virtual std::string toString() ;
};
inline ZLSize::ZLSize(int w, int h) {
    Width = w;
    Height = h;
}
inline bool ZLSize::equals(ZLSize size) {
    
    return Width == size.Width && Height == size.Height;
}
inline std::string ZLSize::toString() {
    return "ZLPaintContext.Size[ x   y]";
}

#endif /* ZLSize_hpp */
