//
//  ZLiOSImageData.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/6.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ZLiOSImageData.h"
#include <vector>
#include <stdio.h>
#include "ZLInputStream.h"
#include "ZLFileImage.h"

ZLiOSImageData::~ZLiOSImageData()
{
    
}
UIImage* getScaledImage(UIImage* old,int neww,int newheight)
{
    UIImage* newres = 0;
    float scaleSize= 0.20;
    UIGraphicsBeginImageContext(CGSizeMake(neww, newheight));
    [old drawInRect:CGRectMake(0, 0, neww, newheight)];
    newres= UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return  newres;
}
UIImage* ZLiOSImageData::getUIImageBy(shared_ptr<ZLFileImage> file)
{
    UIImage* ret = nil;
    ZLFile f2 = file->file();
    shared_ptr<std::string> imageData = new std::string();
    shared_ptr<ZLInputStream>  input = f2.inputStream();
    if(input->open()){
        while(true){
            char* buffer = new char[2048];
            // std::size_t filesize = f2.size();
            std::size_t readed = input->read(buffer, 2048);
            if(readed>0){
                imageData->append(buffer,readed);
            } else {
                break;
            }
        }
        const char* data = imageData->c_str();
        NSData* data2 = [NSData dataWithBytes:data length:imageData->size()];
        ret =  [UIImage imageWithData:data2];
        input->close();
    }
    return ret;
}
UIImage* ZLiOSImageData::getBitmap(int maxWidth,int maxHeight,ZLPaintContext_ScalingType scaling)
{
    if(lastRequestWidth == maxWidth && lastRequestHeight == maxHeight && lastScaling == scaling && cache != 0) {
        return cache;
    }
    lastRequestWidth = maxWidth;
    lastRequestHeight = maxHeight;
    lastScaling = scaling;
    
    cache = getUIImageBy(fileImage);
    
//    const std::string path = fileImage->file().path();
//    ZLFile f2 = fileImage->file();
//    f2.inputStream();
//    shared_ptr<ZLInputStream>  input = f2.inputStream();
//    shared_ptr<std::string> imageData = new std::string();
//    if(input->open()){
//        
//        
//        
//       /* for(std::size_t i=0;i<fileImage->blocks().size();i++){
//            ZLFileImage::Block block = fileImage->blocks().at(i);
//            std::size_t size = block.size;
//            if(size ==0) {
//                if(fileImage->blocks().size() != 1){
//                    continue;
//                } else {
//                    size = input->sizeOfOpened();
//                    if(size ==0) return 0;
//                }
//            }
//            char* buffer = new char[size];
//            std::size_t readed = input->read(buffer, size);
//            imageData->append(buffer,readed);
//            delete[] buffer;
//        }*/
//        while(true){
//            char* buffer = new char[2048];
//           // std::size_t filesize = f2.size();
//            std::size_t readed = input->read(buffer, 2048);
//            if(readed>0){
//                imageData->append(buffer,readed);
//            } else {
//                break;
//            }
//        }
//        
//        input->close();
//    }
//    /*
//    NSString* s =[NSString stringWithUTF8String:path.c_str()];
//    FILE* f = fopen(path.c_str(),"r");
//    char buffer[105];
//    fread(buffer, 1, 100, f);
//    fclose(f);*/
//    const char* data = imageData->c_str();
//    NSData* data2 = [NSData dataWithBytes:data length:imageData->size()];
//    cache =  [UIImage imageWithData:data2];
    if (cache != 0x0) {
        if(scaling == OriginalSize){
            
        } else if (scaling == FitMaximum){
            int bWidth = cache.size.width;
            int bHeight = cache.size.height;
            if (bWidth > 0 && bHeight > 0 &&
                bWidth != maxWidth && bHeight != maxHeight) {
                int w, h;
                if (bWidth * maxHeight > bHeight * maxWidth) {
                    w = maxWidth;
                    h = std::max(1, bHeight * w / bWidth);
                } else {
                    h = maxHeight;
                    w = std::max(1, bWidth * h / bHeight);
                }
                
                /*final Bitmap scaled =
                Bitmap.createScaledBitmap(myBitmap, w, h, false);
                if (scaled != null) {
                    myBitmap = scaled;
                }*/
                cache = getScaledImage(cache,w,h);
                
            }
            
        } else if (IntegerCoefficient == scaling){
             int bWidth = cache.size.width;
             int bHeight = cache.size.height;
            if (bWidth > 0 && bHeight > 0 &&
                (bWidth > maxWidth || bHeight > maxHeight)) {
                 int w, h;
                if (bWidth * maxHeight > bHeight * maxWidth) {
                    w = maxWidth;
                    h = std::max(1, bHeight * w / bWidth);
                } else {
                    h = maxHeight;
                    w = std::max(1, bWidth * h / bHeight);
                }
                cache = getScaledImage(cache, w, h);
                
            }
        }
    }
  /*  float scaleSize= 0.20;
    UIGraphicsBeginImageContext(CGSizeMake(cache.size.width * scaleSize, cache.size.height * scaleSize));
                                [cache drawInRect:CGRectMake(0, 0, cache.size.width * scaleSize, cache.size.height * scaleSize)];
                               cache= UIGraphicsGetImageFromCurrentImageContext();
                                UIGraphicsEndImageContext();*/
    // resize it
    
    return cache;
}
void setFontInternal(/*List<FontEntry> entries,*/ int size, bool bold, bool italic, bool underline, bool strikeThrough)
{
    
}
