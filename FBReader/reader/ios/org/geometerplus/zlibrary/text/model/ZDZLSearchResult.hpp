//
//  ZDZLSearchResult.hpp
//  ZDStudent
//
//  Created by 蒋晨霄 on 2016/12/27.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZDZLSearchResult_hpp
#define ZDZLSearchResult_hpp

#include <vector>
#include <string>
#include "shared_ptr.h"

class ZDZLSearchParagraph {
public:
    int pIndex;
    std::string pText;
    
    ZDZLSearchParagraph(int paragraphIndex, std::string paragraphText);
};

class ZDZLSearchResult {
public:
    int count;
    std::vector<shared_ptr<ZDZLSearchParagraph>> paragraphs;
    
    bool isParagraphExisted(int pIndex);
    void addParagraph(int pIndex, std::string pText);
};

#endif /* ZDZLSearchResult_hpp */
