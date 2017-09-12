//
//  ZLTextWord.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/2.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ZLTextWord.hpp"

ZLTextWord::ZLTextWord(const short* data,int offset,int length, int paragraphOffset)
:Data(data),myParagraphOffset(paragraphOffset),Offset(offset),Length(length)
{
    //this(word.toCharArray(), 0, word.length(), paragraphOffset);
    myMark = 0;
}
ZLTextWord::Mark::~Mark()
{
    if(myNext !=0){
        delete myNext;
        myNext = 0;
    }
}

ZLTextWord::~ZLTextWord()
{
    if(myMark !=0){
        delete myMark;
        myMark = 0;
    }
}
void ZLTextWord::addMark(int start, int length) {
    Mark* existingMark = myMark;
    Mark* mark = new Mark(start, length);
    if ((existingMark == 0) || (existingMark->Start > start)) {
        mark->setNext(existingMark);
        myMark = mark;
    } else {
        while ((existingMark->getNext() != 0) && (existingMark->getNext()->Start < start)) {
            existingMark = existingMark->getNext();
        }
        mark->setNext(existingMark->getNext());
        existingMark->setNext(mark);
    }
}
ZLTextElement::Kind ZLTextWord::kind() const
{
    return ZLTextElement::WORD_ELEMENT;
}
