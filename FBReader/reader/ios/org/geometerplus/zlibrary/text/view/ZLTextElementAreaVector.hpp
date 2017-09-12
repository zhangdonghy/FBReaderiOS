//
//  ZLTextElementAreaVector.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/1.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLTextElementAreaVector_hpp
#define ZLTextElementAreaVector_hpp

#include <stdio.h>
#include <list>
#include <vector>
#include "shared_ptr.h"
#include "ZLTextElementArea.hpp"
#include "ZLTextRegion.hpp"

class ZLTextElementAreaVector
{
private:
    std::vector<shared_ptr<ZLTextElementArea>> myAreas;// =
    //Collections.synchronizedList(new ArrayList<ZLTextElementArea>());
    std::vector<shared_ptr<ZLTextRegion>> myElementRegions ;//= new ArrayList<ZLTextRegion>();
    shared_ptr<ZLTextRegion> myCurrentElementRegion;
    
protected:
public:
    class RegionPair {
        public:
        shared_ptr<ZLTextRegion> Before;
        shared_ptr<ZLTextRegion> After;
    };
    void clear();
    int size() {
        return (int)myAreas.size();
    }
    bool add(shared_ptr<ZLTextElementArea> area) ;
    std::vector<shared_ptr<ZLTextElementArea>> areas() {
        //synchronized (myAreas)
        {
            return myAreas;
            //return new ArrayList<ZLTextElementArea>(myAreas);
        }
    }
    shared_ptr<ZLTextRegion> findRegion(int x, int y, int maxDistance, ZLTextRegion::Filter& filter) ;
    shared_ptr< RegionPair> findRegionsPair(int x, int y, int columnIndex, ZLTextRegion::Filter& filter) ;
    shared_ptr<ZLTextElementArea> getFirstArea() {
        //synchronized (myAreas)
        {
            return myAreas.size()==0 ? 0 : myAreas[0];
        }
    }
     shared_ptr<ZLTextElementArea> getLastArea() {
        //synchronized (myAreas)
        {
            return myAreas.size()==0 ? 0 : myAreas[myAreas.size()-1];//myAreas.get(myAreas.size() - 1);
        }
    }
    shared_ptr<ZLTextRegion> getRegion(shared_ptr<ZLTextRegion::Soul> soul) {
        if (soul == 0) {
            return 0;
        }
        //synchronized (myAreas)
        {
           // for (ZLTextRegion region : myElementRegions) {
            for(int i=0;i<myElementRegions.size();i++){
                if (soul->equals(myElementRegions[i]->getSoul())) {
                    return myElementRegions[i];//region;
                }
            }
        }
        return 0;
    }
    
    shared_ptr<ZLTextElementArea> getFirstAfter(shared_ptr<ZLTextPosition> position) {
        if (position == 0) {
            return 0;
        }
        //synchronized (myAreas)
        {
            //for (ZLTextElementArea area : myAreas) {
            for (int i = 0; i <myAreas.size(); ++i) {
                shared_ptr<ZLTextElementArea> area = myAreas[i];
                if (position->compareTo(*area) <= 0) {
                    return area;
                }
            }
        }
        return 0;
    }
    
    shared_ptr<ZLTextElementArea> getLastBefore(shared_ptr<ZLTextPosition> position) {
        if (position == 0) {
            return 0;
        }
        //synchronized (myAreas)
        {
            for (long i = myAreas.size() - 1; i >= 0; --i) {
                shared_ptr<ZLTextElementArea> area = myAreas[i];
                if (position->compareTo(*area) > 0) {
                    return area;
                }
            }
        }
        return 0;
    }
};
#endif /* ZLTextElementAreaVector_hpp */
