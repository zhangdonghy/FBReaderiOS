//
//  ZLUnderlineHighlighting.hpp
//  ZDLibrary
//
//  Created by 蒋晨霄 on 2017/3/3.
//  Copyright © 2017年 蒋晨霄. All rights reserved.
//

#ifndef ZLUnderlineHighlighting_hpp
#define ZLUnderlineHighlighting_hpp


#include <stdio.h>
#include "ZLTextManualHighlighting.hpp"

class ZLUnderlineHighlighting : public ZLTextManualHighlighting {
public:
    ZLUnderlineHighlighting(shared_ptr<ZLTextView> view, shared_ptr<ZLTextPosition> start,shared_ptr<ZLTextPosition> end);
    
    virtual shared_ptr<ZLColor> getForegroundColor();
    virtual shared_ptr<ZLColor> getBackgroundColor();
    virtual shared_ptr<ZLColor> getOutlineColor();
    virtual shared_ptr<ZLColor> getUnderlineColor();
    
    virtual int getNoteCount();

    virtual bool isUnderline();
};


#endif /* ZLUnderlineHighlighting_hpp */
