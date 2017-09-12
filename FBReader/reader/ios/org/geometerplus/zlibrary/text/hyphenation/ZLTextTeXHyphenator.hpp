//
//  ZLTextTeXHyphenator.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/3.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLTextTeXHyphenator_hpp
#define ZLTextTeXHyphenator_hpp

#include <stdio.h>
#include "ZLTextHyphenator.hpp"

class ZLTextTeXHyphenator : public ZLTextHyphenator
{
    private:
    //ZLTextTeXHyphenator
public:
    virtual void hyphenate(char* stringToHyphenate, bool* mask, int length);
};
#endif /* ZLTextTeXHyphenator_hpp */
