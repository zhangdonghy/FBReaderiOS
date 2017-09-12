//
//  ZDZLSearchUtil.hpp
//  ZDStudent
//
//  Created by 蒋晨霄 on 2016/12/26.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZDZLSearchUtil_hpp
#define ZDZLSearchUtil_hpp

#include <string>
#include "shared_ptr.h"

class ZDZLSearchPattern {
    bool IgnoreCase;
    std::string LowerCasePattern;
    std::string UpperCasePattern;
    long patternLength;
    
public:
    ZDZLSearchPattern(std::string pattern, long length, bool ignoreCase);
    
    int getLength() {
        return (int)patternLength;
    };
    
public:
    bool ignoreCase() {
        return IgnoreCase;
    };
    std::string lowerCasePattern() const {
        return LowerCasePattern;
    };
    std::string upperCasePattern() const {
        return UpperCasePattern;
    };

    friend class ZLSearchUtil;
};

class ZDZLSearchUtil {
    ZDZLSearchUtil() {};

public:
    class Result {
    public:
        int Start;
        int Length;
        
        Result(int start, int length) {
            Start = start;
            Length = length;
        }
    };
    
    static shared_ptr<Result> find(short* textData, int offset, int length, shared_ptr<ZDZLSearchPattern> pattern, int pos = 0);
};

#endif /* ZDZLSearchUtil_hpp */
