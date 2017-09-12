//
//  ZLTextHighlighting.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/8.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLTextHighlighting_hpp
#define ZLTextHighlighting_hpp

#include <stdio.h>
#include "ZLTextPosition.hpp"
#include "ZLTextPage.hpp"
#include "HullUtil.hpp"

class ZLTextHighlighting
{
    
    public:
    virtual bool isEmpty()=0;
    
    virtual shared_ptr<ZLTextPosition> getStartPosition()=0;
    virtual shared_ptr<ZLTextPosition> getEndPosition()=0;
    virtual shared_ptr<ZLTextElementArea> getStartArea(ZLTextPage& page)=0;
    virtual shared_ptr<ZLTextElementArea> getEndArea(ZLTextPage& page)=0;
    
    virtual shared_ptr<ZLColor> getForegroundColor() = 0;
    virtual shared_ptr<ZLColor> getBackgroundColor() = 0;
    virtual shared_ptr<ZLColor> getOutlineColor() = 0;
    virtual shared_ptr<ZLColor> getUnderlineColor() = 0;
    
    virtual int getNoteCount() = 0;
    virtual int getOpinionCount() = 0;
    virtual bool isOpinion() = 0;
    virtual bool isSpeech() = 0;
    virtual bool isUnderline() = 0;

    // function
    bool intersects(ZLTextPage &page) {
        return
        !isEmpty() &&
        !page.StartCursor->isNull() && !page.EndCursor.isNull() &&
        page.StartCursor->compareTo(*getEndPosition()) < 0 &&
        page.EndCursor->compareTo(*getStartPosition()) > 0;
    }
    bool intersects(ZLTextRegion& region) {
        shared_ptr<ZLTextRegion::Soul> soul = region.getSoul();
        return
        !isEmpty() &&
        soul->compareTo(getStartPosition()) >= 0 &&
        soul->compareTo(getEndPosition()) <= 0;
    }
    shared_ptr<Hull> hull(ZLTextPage& page) {
         shared_ptr<ZLTextPosition> startPosition = getStartPosition();
         shared_ptr<ZLTextPosition> endPosition = getEndPosition();
         std::vector<shared_ptr<ZLTextElementArea>> areas = page.TextElementMap->areas();
        int startIndex = 0;
        int endIndex = 0;
        for (int i = 0; i < areas.size(); ++i) {
            shared_ptr<ZLTextElementArea> a = areas[i];//.get(i);
            if (i == startIndex && startPosition->compareTo(( ZLTextPosition&)(*a)) > 0) {
                ++startIndex;
            } else if (endPosition->compareTo((ZLTextPosition&)*a) < 0) {
                break;
            }
            ++endIndex;
        }
        
        if(startIndex>=endIndex) {
            return 0;
        }
        //
       // std::vector<shared_ptr<ZLTextElementArea>> temp;
        shared_ptr<ZLTextElementArea>* temp = new shared_ptr<ZLTextElementArea>[endIndex-startIndex];
        for(int i = 0;i<endIndex-startIndex;i++){
            //temp.insert(temp.end(), areas[i]);
            temp[i] =areas[i+startIndex];
        }
        
        return HullUtil::hull(temp,endIndex-startIndex);//
                             //areas.subList(startIndex, endIndex)
                             
                             //);
    }
    bool checkIn(ZLTextPage& page,int x,int y)
    {
        shared_ptr<ZLTextPosition> startPosition = getStartPosition();
        shared_ptr<ZLTextPosition> endPosition = getEndPosition();
        std::vector<shared_ptr<ZLTextElementArea>> areas = page.TextElementMap->areas();
        int startIndex = 0;
        int endIndex = 0;
        for (int i = 0; i < areas.size(); ++i) {
            shared_ptr<ZLTextElementArea> a = areas[i];//.get(i);
            if (i == startIndex && startPosition->compareTo(( ZLTextPosition&)(*a)) > 0) {
                ++startIndex;
            } else if (endPosition->compareTo((ZLTextPosition&)*a) < 0) {
                break;
            }
            ++endIndex;
        }
        
        if(startIndex>=endIndex) {
            return false;
        }
       // shared_ptr<ZLTextElementArea>* temp = new shared_ptr<ZLTextElementArea>[endIndex-startIndex];
        for(int i = 0;i<endIndex-startIndex;i++){
            shared_ptr<ZLTextElementArea> item =areas[i+startIndex];
            if(x >= item->XStart  && x<=item->XEnd  &&
               y>=item->YStart && y<=item->YEnd) {
                return true;
            }
        }
        return false;
    }
    int compareTo(ZLTextHighlighting& highlighting) {
         int cmp = getStartPosition()->compareTo(*highlighting.getStartPosition());
        return cmp != 0 ? cmp : getEndPosition()->compareTo(*highlighting.getEndPosition());
    }
};
#endif /* ZLTextHighlighting_hpp */
