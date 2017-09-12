//
//  ZLTextRegion.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/1.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLTextRegion_hpp
#define ZLTextRegion_hpp

#include <stdio.h>
#include <list>
#include <vector>
#include "shared_ptr.h"
#include "ZLTextElementArea.hpp"
#include "Hull.hpp"


class ZLTextRegion
{
    public:
    class Filter {
        public:
        virtual bool accepts(shared_ptr<ZLTextRegion> region)=0;
    };
    class AnyRegionFilter: public Filter{
        public:
        bool accepts(shared_ptr<ZLTextRegion> region) {
            return true;
        }
    };
    class Soul
    {
        public:
        
        enum SoulKind {
            UNKOWN_SOUL,
            
            SoulKind_ZLTextWordRegionSoul,
            
            SoulKind_ZLTextImageRegionSoul,
            SoulKind_ZLTextHyperlinkRegionSoul
        };
        virtual SoulKind getSoulKind()
        {
            return  UNKOWN_SOUL;
        }
        int ParagraphIndex;
        int StartElementIndex;
        int EndElementIndex;
        Soul(int paragraphIndex, int startElementIndex, int endElementIndex)
        {
            ParagraphIndex = paragraphIndex;
            StartElementIndex = startElementIndex;
            EndElementIndex = endElementIndex;
        }
        bool accepts(shared_ptr<ZLTextElementArea> area) {
            return compareTo(area) == 0;
        }
        bool equals(shared_ptr<Soul> soul) {
            return
            ParagraphIndex == soul->ParagraphIndex &&
            StartElementIndex == soul->StartElementIndex &&
            EndElementIndex == soul->EndElementIndex;
        }
        int compareTo(shared_ptr<Soul>  soul) {
            if (ParagraphIndex != soul->ParagraphIndex) {
                return ParagraphIndex < soul->ParagraphIndex ? -1 : 1;
            }
            if (EndElementIndex < soul->StartElementIndex) {
                return -1;
            }
            if (StartElementIndex > soul->EndElementIndex) {
                return 1;
            }
            return 0;
        }
        int compareTo(shared_ptr<ZLTextElementArea> area) {
            if (ParagraphIndex != area->ParagraphIndex) {
                return ParagraphIndex < area->ParagraphIndex ? -1 : 1;
            }
            if (EndElementIndex < area->ElementIndex) {
                return -1;
            }
            if (StartElementIndex > area->ElementIndex) {
                return 1;
            }
            return 0;
        }
        int compareTo(shared_ptr<ZLTextPosition> position) {
            int ppi = position->getParagraphIndex();
            if (ParagraphIndex != ppi) {
                return ParagraphIndex < ppi ? -1 : 1;
            }
            int pei = position->getElementIndex();
            if (EndElementIndex < pei) {
                return -1;
            }
            if (StartElementIndex > pei) {
                return 1;
            }
            return 0;
        }
    };
private:
    shared_ptr<Soul> mySoul;
    std::vector<shared_ptr<ZLTextElementArea>>* myAreaList;
    shared_ptr<ZLTextElementArea>* myAreas;
    int myAreaLength;
    
    int myFromIndex;
    int myToIndex;
    
    shared_ptr<Hull> myHull;
    shared_ptr<Hull> myHull0;
protected:
public:
   
    
    static AnyRegionFilter* anyFiler;
    
    ZLTextRegion(shared_ptr<Soul> soul, std::vector<shared_ptr<ZLTextElementArea>>* list, int fromIndex) {
        mySoul = soul;
        myAreaList = list;
        myFromIndex = fromIndex;
        myToIndex = fromIndex + 1;
        myAreaLength = 0;
        myAreas = 0;
    }
    shared_ptr<Soul> getSoul() {
        return mySoul;
    }
    void extend() {
        ++myToIndex;
        myHull = 0;
    }
    int distanceTo(int x, int y) {
        return hull()->distanceTo(x, y);
    }
    shared_ptr<ZLTextElementArea>* textAreas() ;
    
    shared_ptr<Hull> hull() ;
    shared_ptr<Hull> hull0() ;
    
    bool isBefore(int x, int y, int columnIndex);
    
    shared_ptr<ZLTextElementArea> getFirstArea() {
        return textAreas()[0];
    }
    
    shared_ptr<ZLTextElementArea> getLastArea() {
        shared_ptr<ZLTextElementArea> * areas = textAreas();
        return areas[myAreaLength - 1];
    }
};
#endif /* ZLTextRegion_hpp */
