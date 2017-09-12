//
//  ZLTextElementAreaVector.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/1.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ZLTextWord.hpp"
#include "ZLTextElementAreaVector.hpp"
#include "ZLTextHyperlinkRegionSoul.hpp"
#include "ZLTextImageRegionSoul.hpp"
#include "ZLTextWordRegionSoul.hpp"
#include "ExtensionRegionSoul.hpp"
#include "ExtensionElement.hpp"



void ZLTextElementAreaVector::clear()
{
    myElementRegions.clear();
    myCurrentElementRegion = 0;
    myAreas.clear();
}

bool ZLTextElementAreaVector::add(shared_ptr<ZLTextElementArea> area) {
    //synchronized (myAreas)
    {
        if (myCurrentElementRegion != 0
            && myCurrentElementRegion->getSoul()->accepts(area)) {
            myCurrentElementRegion->extend();
        } else {
            shared_ptr<ZLTextRegion::Soul> soul = 0;
            shared_ptr<ZLTextHyperlink> hyperlink = area->Style->Hyperlink;
            if (hyperlink->Id.size() >0) {
                soul = new ZLTextHyperlinkRegionSoul(*area, hyperlink);
            } else if (area->Element->kind() == ZLTextElement::IMAGE_ELEMENT) {
                soul = new ZLTextImageRegionSoul(*area, (ZLTextImageElement&)*area->Element);
            } else if (area->Element->kind() == ZLTextElement::VIDEO_ELEMENT) {
                //soul = new ZLTextVideoRegionSoul(area, (ZLTextVideoElement)area.Element);
            } else if (area->Element->kind() == ZLTextElement::WORD_ELEMENT && !((ZLTextWord&)(*area->Element) ).isASpace()) {
                soul = new ZLTextWordRegionSoul(*area, (ZLTextWord&)*(area->Element));
            } else if (area->Element->kind() == ZLTextElement::EXTENSION_ELEMENT) {
                soul = new ExtensionRegionSoul(*area, (ExtensionElement&)(*area->Element));
            }
            if (soul != 0) {
                myCurrentElementRegion = new ZLTextRegion(soul, &myAreas, (int)myAreas.size());
                myElementRegions.insert(myElementRegions.end(), myCurrentElementRegion);
            } else {
                myCurrentElementRegion = 0;
            }
        }
        myAreas.insert(myAreas.end(), area);
        return true;
    }
    return true;
}
shared_ptr<ZLTextRegion>
ZLTextElementAreaVector::findRegion(int x, int y, int maxDistance, ZLTextRegion::Filter& filter) {
    shared_ptr<ZLTextRegion> bestRegion = 0;
    int distance = maxDistance + 1;
    //synchronized (myAreas)
    {
       // for (shared_ptr<ZLTextRegion> region : myElementRegions) {
        for(int i=0;i<myElementRegions.size();i++){
            if (filter.accepts(myElementRegions[i])) {
                int d = myElementRegions[i]->distanceTo(x, y);
                if (d < distance) {
                    bestRegion = myElementRegions[i];
                    distance = d;
                }
            }
        }
    }
    return bestRegion;
}
shared_ptr<ZLTextElementAreaVector::RegionPair>
ZLTextElementAreaVector::findRegionsPair(int x, int y, int columnIndex, ZLTextRegion::Filter& filter) {
    RegionPair* pair = new RegionPair();
    //synchronized (myAreas)
    {
       // for (ZLTextRegion region : myElementRegions) {
        for(int i=0;i<myElementRegions.size();i++){
            if (filter.accepts(myElementRegions[i])) {
                if (myElementRegions[i]->isBefore(x, y, columnIndex)) {
                    pair->Before = myElementRegions[i];
                } else {
                    pair->After = myElementRegions[i];
                    break;
                }
            }
        }
    }
    return pair;
}