//
//  ZLiOSImageManager.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/6.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLiOSImageManager_hpp
#define ZLiOSImageManager_hpp

#include <stdio.h>
#include "ZLImageManager.hpp"

class ZLiOSImageManager : public ZLImageManager
{
    public:
    virtual shared_ptr<ZLImageData> getImageData(shared_ptr<ZLFileImage> image) const;
};
#endif /* ZLiOSImageManager_hpp */
