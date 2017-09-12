//
//  ZLTextManualHighlighting.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/20.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLTextManualHighlighting_hpp
#define ZLTextManualHighlighting_hpp

#include <stdio.h>
#include "ZLTextSimpleHighlighting.hpp"

class ZLTextView;
class ZLTextPosition;
class ZLTextManualHighlighting : public ZLTextSimpleHighlighting {
public:
    ZLTextManualHighlighting(shared_ptr<ZLTextView> view, shared_ptr<ZLTextPosition> start,shared_ptr<ZLTextPosition> end);
    
    virtual shared_ptr<ZLColor> getForegroundColor();
    virtual shared_ptr<ZLColor> getBackgroundColor();
    virtual shared_ptr<ZLColor> getOutlineColor();
    virtual shared_ptr<ZLColor> getUnderlineColor();

    virtual int getNoteCount();
    virtual int getOpinionCount();

    virtual bool isOpinion() {
//        printf("ZLTextManualHighlighting isOpinion\n");
        return false;
    };
    virtual bool isSpeech() {
//        printf("ZLTextManualHighlighting isSpeech\n");
        return false;
    };
    virtual bool isUnderline() {
//        printf("ZLTextManualHighlighting isUnderline\n");
        return false;
    };
};
#endif /* ZLTextManualHighlighting_hpp */
