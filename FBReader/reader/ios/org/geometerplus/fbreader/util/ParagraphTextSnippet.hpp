//
//  ParagraphTextSnippet.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/21.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ParagraphTextSnippet_hpp
#define ParagraphTextSnippet_hpp

#include <stdio.h>
#include <vector>
#include <string>
#include "TextSnippet.hpp"

class ZLTextWordCursor;

class ParagraphTextSnippet :public TextSnippet
{
private:
    shared_ptr<ZLTextPosition> myStart;
    shared_ptr<ZLTextPosition> myEnd;
    std::string myText;
    std::vector<short> Builder;
    shared_ptr<ZLTextWordCursor> myPageEnd;
    shared_ptr<ZLTextWordCursor> myReadStart;
public:
    ParagraphTextSnippet(shared_ptr<ZLTextWordCursor> start,shared_ptr<ZLTextWordCursor> pageEnd);
    
    bool getParagraphText(std::string*text);
     shared_ptr<ZLTextWordCursor> getReadStart()
    {
        return myReadStart;
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
    void append(const short* data,int length)
    {
        //Builder.append(data);
        for(int i=0;i<length;i++){
            Builder.insert(Builder.end(), data[i]);
        }
    }
    bool IsEnd(shared_ptr<ZLTextWordCursor> pos);
};
#endif /* ParagraphTextSnippet_hpp */
