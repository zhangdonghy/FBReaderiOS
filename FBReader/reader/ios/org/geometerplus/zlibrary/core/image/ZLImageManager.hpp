//
//  ZLImageManager.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/6.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLImageManager_hpp
#define ZLImageManager_hpp

#include <stdio.h>
#include "ZLFileImage.h"

class ZLImageData;
class ZLImageManager
{
    protected:
    static ZLImageManager *ourInstance;
    public:
    ZLImageManager(){
        ourInstance = this;
    }
    ~ZLImageManager()
    {
        
    }
    static const ZLImageManager& Instance(){
        return *ourInstance;
    }
    virtual shared_ptr<ZLImageData> getImageData(shared_ptr<ZLFileImage> image) const=0;
    
};
#endif /* ZLImageManager_hpp */
