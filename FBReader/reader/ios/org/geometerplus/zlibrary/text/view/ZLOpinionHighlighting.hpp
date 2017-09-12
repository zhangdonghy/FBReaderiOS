//
//  ZLOpinionHighlighting.hpp
//  ZDLibrary
//
//  Created by 蒋晨霄 on 2017/3/7.
//  Copyright © 2017年 蒋晨霄. All rights reserved.
//

#ifndef ZLOpinionHighlighting_hpp
#define ZLOpinionHighlighting_hpp

#include <stdio.h>
#include "ZLTextManualHighlighting.hpp"

class ZLOpinionHighlighting : public ZLTextManualHighlighting {
public:
    ZLOpinionHighlighting(shared_ptr<ZLTextView> view, shared_ptr<ZLTextPosition> start,shared_ptr<ZLTextPosition> end);
    
    bool isOpinion();
};

#endif /* ZLOpinionHighlighting_hpp */
