//
//  HullUtil.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/8.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "HullUtil.hpp"

#include "HorizontalConvexHull.hpp"
#include "UnionHull.hpp"

shared_ptr<Hull> HullUtil::hull(std::vector<shared_ptr<ZLTextElementArea>>& list)
{
    std::vector<shared_ptr<ZLRect>> rectangles0 ;//= new ArrayList<Rect>(areas.size());
    std::vector<shared_ptr<ZLRect>>  rectangles1 ;//= new ArrayList<Rect>(areas.size());
    for (int i=0;i<list.size();i++) {
        shared_ptr<ZLTextElementArea> area = list[i];
        shared_ptr<ZLRect> rect = new ZLRect(area->XStart, area->YStart, area->XEnd, area->YEnd);
        if (area->ColumnIndex == 0) {
            rectangles0.insert(rectangles0.end(), rect);//.add(rect);
        } else {
            rectangles1.insert(rectangles1.end(), rect);
            //rectangles1.add(rect);
        }
    }
    if (rectangles0.size() == 0) {
        return new HorizontalConvexHull(rectangles1);
    } else if (rectangles1.size() == 0) {
        return new HorizontalConvexHull(rectangles0);
    } else {
        std::vector<shared_ptr<Hull>> array;
        array.insert(array.end(), new HorizontalConvexHull(rectangles0));
        array.insert(array.end(), new HorizontalConvexHull(rectangles1));
        return new UnionHull(array);
    }
}
 shared_ptr<Hull> HullUtil::hull(shared_ptr<ZLTextElementArea>*list,int length)
{
    //return hull();'
    std::vector<shared_ptr<ZLRect>> rectangles0 ;//= new ArrayList<Rect>(areas.size());
    std::vector<shared_ptr<ZLRect>>  rectangles1 ;//= new ArrayList<Rect>(areas.size());
    for (int i=0;i<length;i++) {
        shared_ptr<ZLTextElementArea> area = list[i];
        shared_ptr<ZLRect> rect = new ZLRect(area->XStart, area->YStart, area->XEnd, area->YEnd);
        if (area->ColumnIndex == 0) {
            rectangles0.insert(rectangles0.end(), rect);//.add(rect);
        } else {
            rectangles1.insert(rectangles1.end(), rect);
            //rectangles1.add(rect);
        }
    }
    if (rectangles0.size() == 0) {
        return new HorizontalConvexHull(rectangles1);
    } else if (rectangles1.size() == 0) {
        return new HorizontalConvexHull(rectangles0);
    } else {
        std::vector<shared_ptr<Hull>> array;
        array.insert(array.end(), new HorizontalConvexHull(rectangles0));
        array.insert(array.end(), new HorizontalConvexHull(rectangles1));
        return new UnionHull(array);
    }
}
