//
//  ZLiOSImageData.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/6.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLiOSImageData_hpp
#define ZLiOSImageData_hpp

#include <stdio.h>
#include "ZLFileImage.h"
#import <UIKit/UIKit.h>
#include "ZLPaintContext.hpp"

class ZLiOSImageData
{
    private:
    int lastRequestWidth;
    int lastRequestHeight;
    ZLPaintContext_ScalingType lastScaling;
    shared_ptr<ZLFileImage> fileImage;
    UIImage* cache;
    public:
    ZLiOSImageData(shared_ptr<ZLFileImage> image)
    {
        fileImage = image;
    }
    ~ZLiOSImageData();
    UIImage* getBitmap(int maxWidth,int maxHeight,ZLPaintContext_ScalingType scaling);
    
    static UIImage* getUIImageBy(shared_ptr<ZLFileImage> file);
};
#endif /* ZLiOSImageData_hpp */
