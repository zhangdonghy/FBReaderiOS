//
//  ZLTextControlElement.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/5.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ZLTextControlElement.hpp"


shared_ptr<ZLTextElement> ZLTextControlElement::myStartElements[256] ={0};
shared_ptr<ZLTextElement> ZLTextControlElement::myEndElements[256] ={0};

shared_ptr<ZLTextElement>
ZLTextControlElement::get(unsigned char kind, bool isStart) {
    shared_ptr<ZLTextElement> element =0;
    if(isStart) {
        element = myStartElements[kind & 0xFF];
    } else {
        element = myEndElements[kind & 0xFF];
    }
    if (element == 0) {
        element = new ZLTextControlElement(kind, isStart);
        if(isStart) {
            myStartElements[kind & 0xFF] = element;
        } else {
            myEndElements[kind & 0xFF] = element;
        }
    }
    return element;
}

ZLTextElement::Kind ZLTextControlElement::kind() const
{
    return ZLTextElement::TEXT_CONTROL_ELEMENT;
}

