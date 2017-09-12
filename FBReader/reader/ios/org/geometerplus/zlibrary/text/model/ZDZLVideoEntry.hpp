//
//  ZDZLVideoEntry.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/24.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZDZLVideoEntry_hpp
#define ZDZLVideoEntry_hpp

#include <stdio.h>
#include <string>
#include <map>


class ZDZLVideoEntry {
private:
    std::map<std::string,std::string> mySources;
public:
    ZDZLVideoEntry();
    void addSource(std::string mime, std::string path);
    std::map<std::string,std::string> sources();
};
#endif /* ZDZLVideoEntry_hpp */
