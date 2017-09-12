//
//  ZLTextRegion.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/1.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ZLTextRegion.hpp"
#include "HullUtil.hpp"
#include "ZLTextElementArea.hpp"


ZLTextRegion::AnyRegionFilter* ZLTextRegion::anyFiler = new ZLTextRegion::AnyRegionFilter();
shared_ptr<Hull>
ZLTextRegion::hull() {
    if (myHull == 0) {
        myHull = HullUtil::hull(textAreas(),myAreaLength);
    }
    return myHull;
}
shared_ptr<ZLTextElementArea>*
ZLTextRegion::textAreas() {
    if (myAreas == 0 || myAreaLength != myToIndex - myFromIndex) {
        //synchronized (myAreaList)
        {
            if(myAreas != 0) {
                delete myAreas;
            }
            myAreas = new shared_ptr<ZLTextElementArea>[myToIndex - myFromIndex];
            myAreaLength = myToIndex - myFromIndex;
            for (int i = 0; i < (myToIndex - myFromIndex); ++i) {
                myAreas[i] = (*myAreaList)[i + myFromIndex];
            }
        }
    }
    return myAreas;
}
bool
ZLTextRegion::isBefore(int x, int y, int columnIndex) {
    switch (columnIndex) {
        default:
        case -1:
        return hull()->isBefore(x, y);
        case 0:
        {
            int count0 = 0;
            int count1 = 0;
            //for (ZLTextElementArea area : textAreas())
            shared_ptr<ZLTextElementArea>* list = textAreas();
            for(int i=0;i<myAreaLength;i++)
            {
                shared_ptr<ZLTextElementArea> area = list[i];
                if (area->ColumnIndex == 0) {
                    ++count0;
                } else {
                    ++count1;
                }
            }
            if (count0 == 0) {
                return false;
            } else if (count1 == 0) {
                return hull()->isBefore(x, y);
            } else {
                return hull0()->isBefore(x, y);
            }
        }
        case 1:
        {
            //for (ZLTextElementArea area : textAreas())
            shared_ptr<ZLTextElementArea>* list = textAreas();
            for(int i=0;i<myAreaLength;i++)
            {
                shared_ptr<ZLTextElementArea> area = list[i];
            if (area->ColumnIndex == 0) {
                return true;
            }
        }
        }
        return hull()->isBefore(x, y);
    }
}

shared_ptr<Hull>
ZLTextRegion::hull0() {
    if (myHull0 == 0) {
        //List<ZLTextElementArea> column0 = new ArrayList<ZLTextElementArea>();
        shared_ptr<ZLTextElementArea>* list = textAreas();
        std::vector<shared_ptr<ZLTextElementArea>> column0;
        for(int i=0;i<myAreaLength;i++){
        //for (ZLTextElementArea a : textAreas()) {
            shared_ptr<ZLTextElementArea> a = list[i];
            if (a->ColumnIndex == 0) {
                //column0.add(a);
                column0.insert(column0.end(), a);
            }
        }
        myHull0 = HullUtil::hull(column0);
    }
    return myHull0;
}