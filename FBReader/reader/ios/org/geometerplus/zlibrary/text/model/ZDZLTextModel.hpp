//
//  ZDZLTextModel.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/16.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZDZLTextModel_hpp
#define ZDZLTextModel_hpp
//#include <string>
//#include <stdio.h>
#include "shared_ptr.h"
#include <vector>
#include <list>
#include "ZDZLTextMark.hpp"

//using namespace std;
class ZDZLTextParagraph;
class ZDZLTextModel{
public:
    virtual std::string getId()=0;
    virtual std::string getLanguage()=0;
    virtual int getParagraphsNumber()=0;
    virtual shared_ptr<ZDZLTextParagraph> getParagraph(int index)=0;
    
    virtual void removeAllMarks()=0;
    virtual shared_ptr<ZDZLTextMark> getFirstMark()=0;
    virtual shared_ptr<ZDZLTextMark> getLastMark()=0;
    virtual shared_ptr<ZDZLTextMark> getNextMark(shared_ptr<ZDZLTextMark> position)=0;
    virtual shared_ptr<ZDZLTextMark> getPreviousMark(shared_ptr<ZDZLTextMark> position)=0;
    
    virtual std::vector<shared_ptr<ZDZLTextMark>> getMarks()=0;
    
    // text length for paragraphs from 0 to index
    virtual int getTextLength(int index)=0;
    virtual int findParagraphByTextLength(int length)=0;
    
    virtual int search(std::string text, long length, int startIndex, int endIndex, bool ignoreCase)=0;
};
#endif /* ZDZLTextModel_hpp */
