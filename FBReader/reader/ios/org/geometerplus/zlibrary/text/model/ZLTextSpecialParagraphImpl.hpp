//
//  ZLTextSpecialParagraphImpl.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/2.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLTextSpecialParagraphImpl_hpp
#define ZLTextSpecialParagraphImpl_hpp

#include <stdio.h>
#include "ZLTextParagraphImpl.hpp"

class ZDZLTextPlainModel;

class ZLTextSpecialParagraphImpl: public ZLTextParagraphImpl
{
private:
    uint8_t mykind;
public:
    ZLTextSpecialParagraphImpl(uint8_t kind, shared_ptr<ZDZLTextPlainModel> model, int offset)
    :ZLTextParagraphImpl(model, offset)
    {
        mykind = kind;
    }
    
    uint8_t getKind() {
        return mykind;
    }
};
#endif /* ZLTextSpecialParagraphImpl_hpp */
