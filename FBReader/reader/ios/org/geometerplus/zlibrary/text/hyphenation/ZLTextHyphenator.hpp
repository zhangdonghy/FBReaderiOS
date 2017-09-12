//
//  ZLTextHyphenator.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/3.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLTextHyphenator_hpp
#define ZLTextHyphenator_hpp

#include <stdio.h>
#include "shared_ptr.h"

class ZLTextTeXHyphenator;
class ZLTextHyphenationInfo;
class ZLTextWord;
class ZLTextHyphenator
{
    private:
    static shared_ptr<ZLTextHyphenator> ourInstance;
    public:
    static shared_ptr<ZLTextHyphenator> Instance() ;
    shared_ptr<ZLTextHyphenationInfo> getInfo(ZLTextWord& word) ;
    virtual void hyphenate(char* stringToHyphenate, bool* mask, int length)=0;
};
#endif /* ZLTextHyphenator_hpp */
