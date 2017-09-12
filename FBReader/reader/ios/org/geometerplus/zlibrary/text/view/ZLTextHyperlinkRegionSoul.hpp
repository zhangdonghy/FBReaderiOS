//
//  ZLTextHyperlinkRegionSoul.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/3.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLTextHyperlinkRegionSoul_hpp
#define ZLTextHyperlinkRegionSoul_hpp

#include <stdio.h>
#include <vector>
#include "ZLTextRegion.hpp"
#include "ZLTextHyperlink.hpp"

class  ZLTextHyperlinkRegionSoul :public ZLTextRegion::Soul
{
    public:
    shared_ptr<ZLTextHyperlink> Hyperlink;
    
    static int startElementIndex(shared_ptr<ZLTextHyperlink> hyperlink, int fallback) {
        std::vector<int> indexes = hyperlink->elementIndexes();
        return indexes.size()==0 ? fallback : indexes[0];
    }
    
     static int endElementIndex(shared_ptr<ZLTextHyperlink> hyperlink, int fallback) {
        std::vector<int>indexes = hyperlink->elementIndexes();
        return indexes.size()==0 ? fallback : indexes[indexes.size() - 1];
    }
    
    ZLTextHyperlinkRegionSoul(ZLTextPosition& position, shared_ptr<ZLTextHyperlink> hyperlink)
    :ZLTextRegion::Soul(
           position.getParagraphIndex(),
           startElementIndex(hyperlink, position.getElementIndex()),
           endElementIndex(hyperlink, position.getElementIndex())
           )
    {
        
        Hyperlink = hyperlink;
    }
    virtual SoulKind getSoulKind()
    {
        return  SoulKind_ZLTextHyperlinkRegionSoul;
    }
};
#endif /* ZLTextHyperlinkRegionSoul_hpp */
