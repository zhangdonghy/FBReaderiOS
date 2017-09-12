//
//  TextSnippet.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/13.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef TextSnippet_hpp
#define TextSnippet_hpp

#include <stdio.h>
#include "ZLTextPosition.hpp"
#include <string>
class TextSnippet
{
protected:
    virtual shared_ptr<ZLTextPosition> getStart()=0;
    virtual shared_ptr<ZLTextPosition> getEnd()=0;
    virtual std::string getText()=0;
};
#endif /* TextSnippet_hpp */
