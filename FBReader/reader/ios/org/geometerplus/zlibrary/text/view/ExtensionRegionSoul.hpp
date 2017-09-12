//
//  ExtensionRegionSoul.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/3.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ExtensionRegionSoul_hpp
#define ExtensionRegionSoul_hpp

#include <stdio.h>
#include "ExtensionElement.hpp"
#include "ZLTextRegion.hpp"

class ExtensionRegionSoul: public ZLTextRegion::Soul
{
    public:
    shared_ptr<ExtensionElement> Element;
    
    ExtensionRegionSoul(ZLTextPosition& position, ExtensionElement& element)
        :ZLTextRegion::Soul(position.getParagraphIndex(), position.getElementIndex(), position.getElementIndex())
    {
       // super(position.getParagraphIndex(), position.getElementIndex(), position.getElementIndex());
        Element = &element;
    }
};
#endif /* ExtensionRegionSoul_hpp */
