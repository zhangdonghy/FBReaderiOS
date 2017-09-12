//
//  ZDZLImageEntry.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/24.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZDZLImageEntry_hpp
#define ZDZLImageEntry_hpp
#include "ZLFileImage.h"
#include "shared_ptr.h"

#include <stdio.h>
#include <map>
#include <string>

//using namespace std;

class ZDZLImageEntry {
private:
    std::map<std::string,shared_ptr<ZLFileImage>> myImageMap;
   public:
      std::string Id;
      short VOffset;
      bool IsCover;
    ZDZLImageEntry(std::map<std::string, shared_ptr<ZLFileImage>> imageMap, std::string id, short vOffset, bool isCover) ;
    shared_ptr<ZLFileImage> getImage();
};

#endif /* ZDZLImageEntry_hpp */
