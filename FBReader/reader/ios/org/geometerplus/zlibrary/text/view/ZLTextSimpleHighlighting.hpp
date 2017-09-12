//
//  ZLTextSimpleHighlighting.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/20.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLTextSimpleHighlighting_hpp
#define ZLTextSimpleHighlighting_hpp

#include <stdio.h>
#include "ZLTextHighlighting.hpp"

class ZLTextView;
class ZLTextPosition;
class ZLTextSimpleHighlighting : public ZLTextHighlighting {
protected:
    shared_ptr<ZLTextView> View;

private:
    shared_ptr<ZLTextPosition> myStartPosition;
    shared_ptr<ZLTextPosition> myEndPosition;
    
public:
    ZLTextSimpleHighlighting(shared_ptr<ZLTextView> view,shared_ptr<ZLTextPosition> start,shared_ptr<ZLTextPosition> end);
    
    virtual bool isEmpty() {
        return false;
    }
    
    virtual shared_ptr<ZLTextPosition> getStartPosition();
    virtual shared_ptr<ZLTextPosition> getEndPosition();
    virtual shared_ptr<ZLTextElementArea> getStartArea(ZLTextPage& page);
    virtual shared_ptr<ZLTextElementArea> getEndArea(ZLTextPage& page);

    virtual bool isOpinion() = 0;
    virtual bool isSpeech() = 0;
    virtual bool isUnderline() = 0;
};
#endif /* ZLTextSimpleHighlighting_hpp */
