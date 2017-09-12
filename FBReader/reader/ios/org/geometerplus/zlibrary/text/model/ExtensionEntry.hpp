//
//  ExtensionEntry.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/1.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ExtensionEntry_hpp
#define ExtensionEntry_hpp

#include <string>
#include <map>
#include <stdio.h>
#include "shared_ptr.h"

class ExtensionEntry
{
private:
    std::string Type;
    std::map<std::string,std::string> Data;
protected:
public:
    ExtensionEntry(std::string type,std::map<std::string,std::string> data);
    virtual bool equals(shared_ptr<ExtensionEntry> other);
    virtual int hashCode();
};

#endif /* ExtensionEntry_hpp */
