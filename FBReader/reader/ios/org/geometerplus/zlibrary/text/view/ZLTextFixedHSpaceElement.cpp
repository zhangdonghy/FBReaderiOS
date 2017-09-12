//
//  ZLTextFixedHSpaceElement.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/3.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ZLTextFixedHSpaceElement.hpp"

shared_ptr<ZLTextElement> ZLTextFixedHSpaceElement::ourCollection[20]={0};


ZLTextElement::Kind ZLTextFixedHSpaceElement::kind() const
{
    return ZLTextElement::FIXED_HSPACE_ELEMENT;
}