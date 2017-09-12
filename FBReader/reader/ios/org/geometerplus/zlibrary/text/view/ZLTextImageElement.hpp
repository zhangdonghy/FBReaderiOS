//
//  ZLTextImageElement.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/1.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLTextImageElement_hpp
#define ZLTextImageElement_hpp

#include <stdio.h>
#include <string>

#include "ZLTextElement.hpp"

class ZLImageData;
class ZLFileImage;
class ZLTextImageElement : public ZLTextElement
{
public:
    std::string Id;
    shared_ptr<ZLImageData> ImageData;
    std::string URL;
     bool IsCover;
    shared_ptr<ZLFileImage> ImageFile;
    ZLTextImageElement(std::string id, shared_ptr<ZLImageData> imageData, std::string url, bool isCover,shared_ptr<ZLFileImage> imageFile) {
        Id = id;
        ImageData = imageData;
        URL = url;
        IsCover = isCover;
        ImageFile = imageFile;
    }
    shared_ptr<ZLFileImage>  getFileImage()
    {
        return  ImageFile;
    }
   /* ZLTextImageElement(std::string id, shared_ptr<ZLFileImage> fileImage, std::string url, bool isCover) {
        Id = id;
        ImageFile = fileImage;
        URL = url;
        IsCover = isCover;
    }*/
    virtual Kind kind() const;
    friend class ZLTextViewBase;
};
#endif /* ZLTextImageElement_hpp */
