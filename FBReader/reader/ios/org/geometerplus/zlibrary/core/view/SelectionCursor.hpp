//
//  SelectionCursor.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/9.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef SelectionCursor_hpp
#define SelectionCursor_hpp

#include <stdio.h>
#include "ZLPaintContext.hpp"
#include "ZLTextElementArea.hpp"

class SelectionCursor {
    public:
    enum Which {
        None,
        Left,
        Right
    };
    
    static void draw(shared_ptr<ZLPaintContext> context,
                     Which which,
                     shared_ptr<ZLTextElementArea> area,
                     shared_ptr<ZLColor> color);
};

#endif /* SelectionCursor_hpp */
