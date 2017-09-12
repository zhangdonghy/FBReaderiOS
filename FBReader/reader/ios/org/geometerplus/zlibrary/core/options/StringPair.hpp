//
//  StringPair.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/3.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef StringPair_hpp
#define StringPair_hpp

#include <string>

#include <stdio.h>
class StringPair
{
    public:
    std::string Group;
    std::string Name;
    StringPair(std::string group, std::string name) {
        Group = group;//group.intern();
        Name = name;//name.intern();
    }
    
    virtual  bool equals(StringPair pair) {
        return Group == (pair.Group) && Name == (pair.Name);
    }
    
    virtual int hashCode() ;
};
#endif /* StringPair_hpp */
