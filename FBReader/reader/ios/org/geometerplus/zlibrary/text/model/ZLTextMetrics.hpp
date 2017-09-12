//
//  ZLTextMetrics.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/31.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLTextMetrics_hpp
#define ZLTextMetrics_hpp

#include <stdio.h>

class ZLTextMetrics
{
private:
protected:
public:
  int DPI;
    int FullWidth;
   int FullHeight;
   int FontSize;
    
    
    ZLTextMetrics(int dpi, int fullWidth, int fullHeight, int fontSize);
    
    virtual bool equals(ZLTextMetrics o);
    
    virtual int hashCode();
    //friend class ZDZLTextStyleEntry;
    friend class ZDZLTextStyleEntry;
};
#endif /* ZLTextMetrics_hpp */
