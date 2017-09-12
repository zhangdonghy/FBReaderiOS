//
//  SimpleCSSReader.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/13.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef SimpleCSSReader_hpp
#define SimpleCSSReader_hpp

#include <stdio.h>
#include <string>
#include <map>
#include <vector>
class ZLTextNGStyleDescription;

class SimpleCSSReader
{
private:
    enum State {
        EXPECT_SELECTOR,
        EXPECT_OPEN_BRACKET,
        EXPECT_NAME,
        EXPECT_VALUE,
        READ_COMMENT,
    };
    State myState;
    State mySavedState;
    std::string mySelector;
    std::string myName;
    std::map<std::string,std::string>* myCurrentMap;
    std::map<int,ZLTextNGStyleDescription*> myDescriptionMap;
    void processToken(std::string token);
public:
    SimpleCSSReader();
    //void read(std::string content);
    void read(std::string path);
    std::vector<ZLTextNGStyleDescription*> getDescriptionList();
    std::map<int,ZLTextNGStyleDescription*> getDescriptionMap()
    {
        return myDescriptionMap;
    }
   
};
#endif /* SimpleCSSReader_hpp */
