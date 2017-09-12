//
//  ProcessHyperLinkAction.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/23.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ProcessHyperLinkAction.h"

#include "FBReaderApp.h"
#include "ZLTextImageRegionSoul.hpp"
#include "ZLiOSImageData.h"

#import "ZDFBReaderInterface.h"

void ProcessHyperLinkAction::run(void* data) {
    shared_ptr<ZLTextRegion> region = myApp->BookTextView->getOutlinedRegion();
    if (region == 0) {
        return;
    }
    shared_ptr<ZLTextRegion::Soul> soul = region->getSoul();
    if (soul->getSoulKind() == ZLTextRegion::Soul::SoulKind_ZLTextImageRegionSoul) {
        // image
        myApp->BookTextView->hideOutline();
        std::string url = ((ZLTextImageRegionSoul&)*soul).ImageElement->URL;
        printf("image url: %s\n",url.c_str());
        shared_ptr<ZLFileImage> fileimage = ((ZLTextImageRegionSoul&)*soul).ImageElement->getFileImage();
        UIImage* image = ZLiOSImageData::getUIImageBy(fileimage);

        if ([_interface respondsToSelector:@selector(showFullScreenImage:)]) {
            [_interface showFullScreenImage:image];
        }
    }
}

