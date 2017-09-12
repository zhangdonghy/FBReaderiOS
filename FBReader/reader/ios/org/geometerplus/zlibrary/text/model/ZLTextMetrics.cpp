//
//  ZLTextMetrics.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/31.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ZLTextMetrics.hpp"

ZLTextMetrics::ZLTextMetrics(int dpi, int fullWidth, int fullHeight, int fontSize) {
    DPI = dpi;
    FullWidth = fullWidth;
    FullHeight = fullHeight;
    FontSize = fontSize;
}

 bool ZLTextMetrics::equals(ZLTextMetrics oo) {
     
    return
    DPI == oo.DPI &&
    FullWidth == oo.FullWidth &&
    FullHeight == oo.FullHeight;
}

int ZLTextMetrics::hashCode() {
    return DPI + 13 * (FullHeight + 13 * FullWidth);
}
