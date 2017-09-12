//
//  HorizontalConvexHull.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/8.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "HorizontalConvexHull.hpp"

void HorizontalConvexHull::draw(shared_ptr<ZLPaintContext> context, int mode, shared_ptr<ZLColor> color, int noteCount) {
//    printf("HorizontalConvexHull draw\n");
    if (mode == Hull::None) {
        return;
    }
    
    std::vector<shared_ptr<ZLRect>> rectangles;
    for (int i = 0; i < myRectangles.size(); i++) {
        rectangles.insert(rectangles.end(), new ZLRect(myRectangles[i]->Left,
                                                       myRectangles[i]->Top,
                                                       myRectangles[i]->Right,
                                                       myRectangles[i]->Bottom));
    }
    if (rectangles.size() > 0) {
        if (mode == Hull::Underline) {
            context->drawUnderline(rectangles, color, noteCount);
            return;
        }
    }

    while (rectangles.size() > 0) {
        std::vector<shared_ptr<ZLRect>> connected;
        shared_ptr<ZLRect> previous = 0;
        for (std::vector<shared_ptr<ZLRect>>::const_iterator it = rectangles.begin(); it != rectangles.end(); ) {
            shared_ptr<ZLRect> current = *it;
            
            if ((previous != 0) &&
                ((previous->Left > current->Right) || (current->Left > previous->Right))) {
                break;
            }
            it = rectangles.erase(it);
            connected.insert(connected.end(), current);
            previous = current;
        }
        
        std::vector<int> xList;
        std::vector<int> yList;
        int x = 0, xPrev = 0;
        
        shared_ptr<ZLRect> r = connected[0];
        x = r->Right + 2;
        xList.insert(xList.end(), x); yList.insert(yList.end(), r->Top);
        int i = 1;
        while (i<connected.size()) {
            xPrev = x;
            r = connected[i];//iter.next();
            x = r->Right + 2;
            if (x != xPrev) {
                int y = (x < xPrev) ? r->Top + 2 : r->Top;
                xList.insert(xList.end(),xPrev); yList.insert(yList.end(),y);
                xList.insert(xList.end(),x); yList.insert(yList.end(),y);
            }
            i++;
        }
        
        xList.insert(xList.end(),x); yList.insert(yList.end(),r->Bottom + 2);
        
        //
        i = connected.size()-1;
        r = connected[i];//iter.previous();
        x = r->Left - 2;
        xList.insert(xList.end(),x); yList.insert(yList.end(),r->Bottom + 2);
        i--;
        while (i>=0){//(iter.hasPrevious()) {
            xPrev = x;
            r = connected[i--];//iter.previous();
            x = r->Left - 2;
            if (x != xPrev) {
                int y = (x > xPrev) ? r->Bottom : r->Bottom + 2;
                xList.insert(xList.end(),xPrev); yList.insert(yList.end(),y);
                xList.insert(xList.end(),x); yList.insert(yList.end(),y);
            }
        }
        xList.insert(xList.end(),x); yList.insert(yList.end(),r->Top);
        
        int* xs = new int[xList.size()];
        int* ys = new int[yList.size()];
        int count = 0;
        for (int xx : xList) {
            xs[count++] = xx;
        }
        count = 0;
        for (int yy : yList) {
            ys[count++] = yy;
        }
        
        if (mode == Hull::Speech || mode == Hull::Fill) {
            context->fillPolygon(xs, ys, yList.size(), color);
        }
    }
}

