//
//  ZLTextHyphenator.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/3.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ZLTextHyphenator.hpp"
#include "ZLTextTeXHyphenator.hpp"
#include "ZLTextWord.hpp"
#include "ZLUnicodeUtil.h"
#include "ZLTextHyphenationInfo.hpp"


shared_ptr<ZLTextHyphenator>  ZLTextHyphenator::ourInstance = 0;

shared_ptr<ZLTextHyphenator> ZLTextHyphenator::Instance() {
    if (ourInstance == 0) {
        ourInstance = new ZLTextTeXHyphenator();
    }
    return ourInstance;
}

shared_ptr<ZLTextHyphenationInfo>
ZLTextHyphenator::getInfo(ZLTextWord& word) {
    int len = word.Length;
  bool* isLetter = new bool[len];
    char* pattern = new char[len + 2];
    const short* data = word.Data;
    pattern[0] = ' ';
    for (int i = 0, j = word.Offset; i < len; ++i, ++j) {
        short character = data[j];
        if (character == '\'' || character == '^' || ZLUnicodeUtil::isLetter(character)) {
            isLetter[i] = true;
            pattern[i + 1] = std::tolower(character);//Character.toLowerCase(character);
        } else {
            pattern[i + 1] = ' ';
        }
    }
    pattern[len + 1] = ' ';
    
    shared_ptr<ZLTextHyphenationInfo> info = new ZLTextHyphenationInfo(len + 2);
    bool* mask = info->Mask;
    hyphenate(pattern, mask, len + 2);
    for (int i = 0, j = word.Offset - 1; i <= len; ++i, ++j) {
        if ((i < 2) || (i > len - 2)) {
            mask[i] = false;
        } else {
            switch (data[j]) {
                case (char)0xAD: // soft hyphen
                    mask[i] = true;
                    break;
                case '-':
                    mask[i] = (i >= 3)
                    && isLetter[i - 3]
                    && isLetter[i - 2]
                    && isLetter[i]
                    && isLetter[i + 1];
                    break;
                default:
                    mask[i] = mask[i]
                    && isLetter[i - 2]
                    && isLetter[i - 1]
                    && isLetter[i]
                    && isLetter[i + 1];
                    break;
            }
        }
    }
   
    return info;
   // return 0;
}
