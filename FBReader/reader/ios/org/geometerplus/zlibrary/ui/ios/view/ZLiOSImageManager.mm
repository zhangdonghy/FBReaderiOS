//
//  ZLiOSImageManager.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/6.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ZLiOSImageManager.h"
#include "ZLiOSImageData.h"


shared_ptr<ZLImageData>
ZLiOSImageManager::getImageData(shared_ptr<ZLFileImage> image) const
{
    
    ZLiOSImageData* m = new ZLiOSImageData(image);
    return (ZLImageData*)m;
}
