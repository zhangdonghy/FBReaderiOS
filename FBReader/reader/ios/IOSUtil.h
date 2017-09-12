//
//  IOSUtil.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/29.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef IOSUtil_hpp
#define IOSUtil_hpp

//#include <stdio.h>
#include <vector>
#include "ZLFileImage.h"
#include "ZDZLImage.hpp"
//#include "ZDZLFileImage.hpp"
#include "ZDBookModel.hpp"

class IOSUtil {
    
public:
    static ZDZLImage* createZDZLImage(const ZLImage& image);
    
    
    /// method
   static void Method_BookModel_addImage(ZDBookModel* bookModel,const std::string& id,const  ZLFileImage& image);
    static std::string GetFullPathForResource(std::string name,std::string group,std::string type);
    static std::string GetContentForResource(std::string name,std::string type);
    static std::vector<std::string> EvaluateString(std::string line);
    static void StoreKeyValue(std::string key,std::string value);
    static std::string ReadKeyValue(std::string key);
    static std::string IntToString(int number);
    static int StringToInt(std::string string);
    static double StringToDouble(std::string string);
    static bool isDigit(std::string maydigit);
    static bool endWithStr(std::string src, std::string match);
    
    static void arraycopy(short* src, int srcPos, short*  dest, int destPos, int length);
  //  static std::string subString(std::string src,);
};
#endif /* IOSUtil_hpp */
