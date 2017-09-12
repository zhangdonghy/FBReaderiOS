//
//  ZLSpeechHighlighting.hpp
//  ZDLibrary
//
//  Created by 蒋晨霄 on 2017/3/3.
//  Copyright © 2017年 蒋晨霄. All rights reserved.
//

#ifndef ZLSpeechHighlighting_hpp
#define ZLSpeechHighlighting_hpp


#include <stdio.h>
#include "ZLTextManualHighlighting.hpp"

class ZLSpeechHighlighting : public ZLTextManualHighlighting {
public:
    ZLSpeechHighlighting(shared_ptr<ZLTextView> view, shared_ptr<ZLTextPosition> start,shared_ptr<ZLTextPosition> end);
    
    virtual shared_ptr<ZLColor> getForegroundColor();
    virtual shared_ptr<ZLColor> getBackgroundColor();
    virtual shared_ptr<ZLColor> getOutlineColor();
    virtual shared_ptr<ZLColor> getUnderlineColor();

    virtual bool isSpeech();
};

#endif /* ZLSpeechHighlighting_hpp */
