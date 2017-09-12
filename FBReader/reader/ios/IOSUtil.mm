//
//  IOSUtil.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/29.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "IOSUtil.h"
#include <stdlib.h>
#include "ZDBookModel.hpp"
#include "ZLFileImage.h"
#import <Foundation/Foundation.h>


double IOSUtil::StringToDouble(std::string string)
{
    const char* b = string.c_str();
    return  atof(b);
}
int IOSUtil::StringToInt(std::string string)
{
    const char* b = string.c_str();
    return  atoi(b);
}
bool IOSUtil::endWithStr(std::string src, std::string match)
{
    if(match.size() > src.size()) {
        return false;
    }
    const char* srcb = src.c_str();
    const char* matchb = match.c_str();
    
    for(int i=0;i<match.size();i++)
    {
        if(srcb[src.size()-match.size()+i] != match[i]){
            return false;
        }
    }
    return true;
}

bool
IOSUtil::isDigit(std::string maydigit)
{
    const char* b = maydigit.c_str();
    for(int i=0;i<maydigit.size();i++){
        if(!( b[i]>='0' && b[i]<='9' )) {
            return  false;
        }
    }
    return true;
}
ZDZLImage* IOSUtil::createZDZLImage(const ZLImage& image)
{
    ZDZLImage* newimage = new ZDZLImage();
    
    const ZLFileImage& image2 = (ZLFileImage&)image;
    std::string path = image2.file().path();
    const ZLFileImage::Blocks &blocks = image2.blocks();
    return newimage;
}

void IOSUtil::Method_BookModel_addImage(ZDBookModel* bookModel,const std::string &id,const ZLFileImage& image)
{
    //model-
    bookModel->addImage(id,  image);
}


std::string IOSUtil::GetFullPathForResource(std::string name,std::string group,std::string type)
{
    NSString* bookFullPath = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:name.c_str()]
                                                             ofType:[NSString stringWithUTF8String:type.c_str()]
                                                             inDirectory:nil];
   /* NSString *htmlString = [NSString stringWithContentsOfFile:bookFullPath encoding:NSUTF8StringEncoding error:nil];*/
    return [bookFullPath UTF8String];
}
std::string IOSUtil::GetContentForResource(std::string name,std::string type)
{
    NSString* bookFullPath = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:name.c_str()]
                                                             ofType:[NSString stringWithUTF8String:type.c_str()]
                                                        inDirectory:nil];
     NSString *htmlString = [NSString stringWithContentsOfFile:bookFullPath encoding:NSUTF8StringEncoding error:nil];
    return [htmlString UTF8String];
}
std::vector<std::string> IOSUtil::EvaluateString(std::string line)
{
    NSString *searchText = [NSString stringWithUTF8String:line.c_str()];
   /* NSError *error = NULL;
    NSRegularExpression *regex = [NSRegularExpression regularExpressionWithPattern:@"^[0-9]+$"
                                    options:NSRegularExpressionCaseInsensitive error:&error];
    NSTextCheckingResult *result = [regex firstMatchInString:searchText options:0
                        range:NSMakeRange(0, [searchText length])];
    if (result) {
        NSLog(@"%@", [searchText substringWithRange:result.range]);
    }*/
    /*NSString *searchText = @"// Do any additional setup after loading the view, typically from a nib., typically from a nib., typically from a nib., typically from a nib., typically from a nib., typically from a nib.";
    NSError *error = NULL;
    NSRegularExpression *regex = [NSRegularExpression regularExpressionWithPattern:@"(?:[^,])*\\." options:NSRegularExpressionCaseInsensitive error:&error];
    NSTextCheckingResult *result = [regex firstMatchInString:searchText options:0 range:NSMakeRange(0, [searchText length])];
    if (result) {
        NSLog(@"%@\n", [searchText substringWithRange:result.range]);
    }
    
    NSString *urlString = @"sfdshttp://www.baidu.comsfdshttp://www.baidu.comsfdshttp://www.baidu.comsfdshttp://www.baidu.comsfdshttp://www.baidu.com";
   */
    NSError *error = NULL;
    std::vector<std::string> list;
   NSRegularExpression* regex = [NSRegularExpression regularExpressionWithPattern:@"([^\"\\s:;]+|\".+?\")" options:0 error:&error];
    
    if (regex != nil) {
        NSArray<NSTextCheckingResult *> * res = [regex matchesInString:searchText options:0 range:NSMakeRange(0, [searchText length])];
        NSUInteger ff = [regex numberOfMatchesInString:searchText options:0 range:NSMakeRange(0, [searchText length])];
        for(int i=0;i<[res count];i++){
            NSTextCheckingResult * item = res[i];
            NSRange resultRange = [item rangeAtIndex:0];
            NSString *result = [searchText substringWithRange:resultRange];
            //
            NSString *resultnew = [result stringByReplacingOccurrencesOfString:@"\"" withString:@""];
            std::string itemstr = [resultnew UTF8String];
            //NSLog(@"%@",result);
            list.insert(list.end(), itemstr);
        }
        
    }
    return list;
}
void
IOSUtil::StoreKeyValue(std::string key,std::string value)
{
    
}
std::string
IOSUtil::ReadKeyValue(std::string key)
{
    return  "";
}
std::string IOSUtil::IntToString(int number)
{
    NSString *resultnew = [NSString stringWithFormat:@"%d",number];
    return  [resultnew UTF8String];
}

void IOSUtil::arraycopy(short* src, int srcPos, short*  dest, int destPos, int length)
{
    for(int i=0;i<length;i++){
        dest[destPos+i] = src[srcPos+i];
    }
}



