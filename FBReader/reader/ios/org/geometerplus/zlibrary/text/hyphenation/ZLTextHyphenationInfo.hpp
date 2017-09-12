//
//  ZLTextHyphenationInfo.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/3.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLTextHyphenationInfo_hpp
#define ZLTextHyphenationInfo_hpp

#include <stdio.h>

class ZLTextHyphenationInfo
{
    protected:
    bool *Mask;
    int Length;
    public:
     ZLTextHyphenationInfo(int length) {
        Mask = new bool[length - 1];
         Length = length;
    }
    
    bool isHyphenationPossible(int position) {
        return (position < Length && Mask[position]);
    }
    friend class ZLTextHyphenator;
};
#endif /* ZLTextHyphenationInfo_hpp */
