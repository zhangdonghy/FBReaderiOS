//
//  ZLTextControlElement.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/5.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLTextControlElement_hpp
#define ZLTextControlElement_hpp

#include <stdio.h>
#include "ZLTextElement.hpp"

class ZLTextControlElement : public ZLTextElement
{
    private:
    //static ZLTextControlElement* myStartElements[256];
    //static ZLTextControlElement* myEndElements[256];
    
    static shared_ptr<ZLTextElement> myStartElements[256];
    static shared_ptr<ZLTextElement> myEndElements[256];
    public:
    static shared_ptr<ZLTextElement> get(unsigned char kind, bool isStart) ;
    
    unsigned char Kind;
    bool IsStart;
    
    ZLTextControlElement(unsigned char kind, bool isStart) {
        Kind = kind;
        IsStart = isStart;
    }
    virtual ZLTextElement::Kind kind() const;
};
#endif /* ZLTextControlElement_hpp */
