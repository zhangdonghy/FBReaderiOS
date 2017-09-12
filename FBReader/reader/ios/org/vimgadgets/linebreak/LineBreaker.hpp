//
//  LineBreaker.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/5.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef LineBreaker_hpp
#define LineBreaker_hpp

#include <stdio.h>
#include <string>
class LineBreaker
{
    /*static {
        System.loadLibrary("LineBreak-v2");
        init();
    }*/
    public:
    static const char MUSTBREAK = 0;
    static const  char ALLOWBREAK = 1;
    static const  char NOBREAK = 2;
    static const  char INSIDEACHAR = 3;
    
    //private static native void init();
    void setLineBreaksForCharArray(short* data, int offset, int length, std::string lang,uint8_t* breaks,uint16_t breaks_len);
    void setLineBreaksForString(std::string data, std::string lang, uint8_t* breaks,uint16_t breaks_len);
    
    std::string myLanguage;
    
    LineBreaker(std::string lang) {
        myLanguage = lang;
    }
    
    void setLineBreaks(short* data, int offset, int length, uint8_t* breaks,uint16_t breaks_len) {
        setLineBreaksForCharArray(data, offset, length, myLanguage, breaks,breaks_len);
    }
    
    void setLineBreaks(std::string data, uint8_t* breaks,uint16_t breaks_len) {
        setLineBreaksForString(data, myLanguage, breaks,breaks_len);
    }
};
#endif /* LineBreaker_hpp */
