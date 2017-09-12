//
//  SelectionCursor.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/9.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "SelectionCursor.hpp"
#include "ZDZLibrary.hpp"

void SelectionCursor::draw(shared_ptr<ZLPaintContext> context,
                           Which which,
                           shared_ptr<ZLTextElementArea> area,
                           shared_ptr<ZLColor> color) {
    context->setFillColor(color);
    int dpi = ZDZLibrary::Instance()->getDisplayDPI();

//    printf("Selection area: xStart: %d, xEnd: %d, yStart: %d, yEnd: %d, which: %d\n",
//           area->XStart, area->XEnd, area->YStart, area->YEnd, which);
    int lineHalfWidth = dpi / 120;
    int lineHeight = area->YEnd - area->YStart + dpi / 20;
    int radius = lineHalfWidth * 4;

    if (which == Left) {
//        printf("Left rectangle: x0: %d, y0: %d, x1: %d, y1: %d\n",
//               area->XStart - lineHalfWidth * 2, area->YEnd - lineHeight, area->XStart, area->YEnd);
        context->fillRectangle(area->XStart - lineHalfWidth * 2, area->YEnd - lineHeight, area->XStart, area->YEnd);
        context->fillCircle(area->XStart - lineHalfWidth, area->YEnd - lineHeight, radius);        
    } else {
//        printf("Right rectangle: x0: %d, y0: %d, x1: %d, y1: %d\n",
//               area->XEnd, area->YStart, area->XEnd + lineHalfWidth * 2, area->YStart + lineHeight);
        context->fillRectangle(area->XEnd, area->YStart, area->XEnd + lineHalfWidth * 2, area->YStart + lineHeight);
        context->fillCircle(area->XEnd + lineHalfWidth, area->YStart + lineHeight, radius);
    }
}
