//
//  FixedTextSnippet.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/13.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef FixedTextSnippet_hpp
#define FixedTextSnippet_hpp

#include <stdio.h>
#include "TextSnippet.hpp"

class FixedTextSnippet: public TextSnippet
{
private:
    shared_ptr<ZLTextPosition> myStart;
    shared_ptr<ZLTextPosition> myEnd;
    std::string myText;
public:
    FixedTextSnippet(shared_ptr<ZLTextPosition> start,
                     shared_ptr<ZLTextPosition> end,
                       std::string text){
        myStart = start;
        myEnd = end;
        myText = text;
    }
    virtual shared_ptr<ZLTextPosition> getStart()
    {
        return myStart;
    }
    virtual shared_ptr<ZLTextPosition> getEnd()
    {
        return myEnd;
    }
    virtual std::string getText()
    {
        return myText;
    }
};
#endif /* FixedTextSnippet_hpp */
