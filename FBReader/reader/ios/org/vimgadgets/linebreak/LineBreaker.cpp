//
//  LineBreaker.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/5.
//  Copyright © 2016年 Eagley. All rights reserved.
//
#include "linebreak.h"

#include "LineBreaker.hpp"


void LineBreaker::setLineBreaksForCharArray(short* data, int offset, int length, std::string lang,uint8_t* breaksArray,uint16_t breaks_len)
{
    /*
     jchar* dataArray = env->GetCharArrayElements(data, 0);
     jbyte* breaksArray = env->GetByteArrayElements(breaks, 0);
     const char *langArray = (lang != 0) ? env->GetStringUTFChars(lang, 0) : 0;
     
     set_linebreaks_utf16(dataArray + offset, length, langArray, (char*)breaksArray);
     const jchar* start = dataArray + offset;
     const jchar* end = start + length;
     for (const jchar* ptr = start; ptr < end; ++ptr) {
     if (*ptr == (jchar)0xAD) {
     breaksArray[ptr - start] = LINEBREAK_NOBREAK;
     }
     }
     
     if (lang != 0) {
     env->ReleaseStringUTFChars(lang, langArray);
     }
     env->ReleaseByteArrayElements(breaks, breaksArray, 0);
     env->ReleaseCharArrayElements(data, dataArray, 0);
     */
    set_linebreaks_utf16((const utf16_t *)(data + offset), length, lang.c_str(), (char*)breaksArray);
    const short* start = data + offset;
    const short* end = start + length;
    for (const short* ptr = start; ptr < end; ++ptr) {
        if (*ptr == (short)0xAD) {
            breaksArray[ptr - start] = LINEBREAK_NOBREAK;
        }
    }
    
}
void LineBreaker::setLineBreaksForString(std::string data, std::string lang, uint8_t* breaksArray,uint16_t breaks_len)
{
    /*
     const jchar* dataArray = env->GetStringChars(data, 0);
     jbyte* breaksArray = env->GetByteArrayElements(breaks, 0);
     const size_t len = env->GetStringLength(data);
     const char *langArray = (lang != 0) ? env->GetStringUTFChars(lang, 0) : 0;
     
     set_linebreaks_utf16(dataArray, len, langArray, (char*)breaksArray);
     
     if (lang != 0) {
     env->ReleaseStringUTFChars(lang, langArray);
     }
     env->ReleaseByteArrayElements(breaks, breaksArray, 0);
     env->ReleaseStringChars(data, dataArray);
     */
    set_linebreaks_utf16((const utf16_t *)data.c_str(), data.size(), lang.c_str(), (char*)breaksArray);
}
