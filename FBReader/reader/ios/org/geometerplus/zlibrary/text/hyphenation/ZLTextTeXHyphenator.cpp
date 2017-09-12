//
//  ZLTextTeXHyphenator.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/3.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ZLTextTeXHyphenator.hpp"


void
ZLTextTeXHyphenator::hyphenate(char* stringToHyphenate, bool* mask, int length)
{
    //if (myPatternTable.isEmpty())
    {
        for (int i = 0; i < length - 1; i++) {
            mask[i] = false;
        }
        return;
    }
   /*
    byte[] values = new byte[length + 1];
    
    final HashMap<ZLTextTeXHyphenationPattern,ZLTextTeXHyphenationPattern> table = myPatternTable;
    ZLTextTeXHyphenationPattern pattern =
    new ZLTextTeXHyphenationPattern(stringToHyphenate, 0, length, false);
    for (int offset = 0; offset < length - 1; offset++) {
        int len = Math.min(length - offset, myMaxPatternLength) + 1;
        pattern.update(stringToHyphenate, offset, len - 1);
        while (--len > 0) {
            pattern.reset(len);
            ZLTextTeXHyphenationPattern toApply =
            (ZLTextTeXHyphenationPattern)table.get(pattern);
            if (toApply != null) {
                toApply.apply(values, offset);
            }
        }
    }
    
    for (int i = 0; i < length - 1; i++) {
        mask[i] = (values[i + 1] % 2) == 1;
    }*/
}