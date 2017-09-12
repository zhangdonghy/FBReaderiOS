//
//  ParagraphTextSnippet.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/21.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ParagraphTextSnippet.hpp"
#include "ZLTextWordCursor.hpp"
#include "ZLUnicodeUtil.h"

bool ParagraphTextSnippet::IsEnd(shared_ptr<ZLTextWordCursor> cursor)
{
    if(cursor->getParagraphIndex()>myPageEnd->getParagraphIndex()){
        printf("END3\n");
        return true;
    }else if(cursor->getParagraphIndex()==myPageEnd->getParagraphIndex())
    {
        if(cursor->getElementIndex()>myPageEnd->getElementIndex()){
            printf("END2\n");
            return true;
        }else if(cursor->getElementIndex()==myPageEnd->getElementIndex()){
            if(cursor->getCharIndex()>=myPageEnd->getCharIndex()){
                printf("END\n");
                return true;
            }
        }
    }
    return false;
}
bool ParagraphTextSnippet::getParagraphText(std::string*text)
{
    // skip the space;
    while (myReadStart->isEndOfParagraph()) {
        if (!myReadStart->nextParagraph()) {
            return false;
        }
    }
    if(IsEnd(myReadStart)){
        return false;
    }
    while(!myReadStart->isEndOfParagraph()&&!IsEnd(myReadStart)){
        shared_ptr<ZLTextElement> element = myReadStart->getElement();
        ZLTextWord& word = (ZLTextWord&)*element;
        if (element->kind() == ZLTextElement::WORD_ELEMENT) {
            append(&word.Data[word.Offset], word.Length);
        }
        
        myReadStart->nextWord();
    }
    if(Builder.size()>0){
        std::string to="";
        ZLUnicodeUtil::Ucs2String from;
        for(int i=0;i<Builder.size();i++){
            from.insert(from.end(), Builder[i]);
        }
        ZLUnicodeUtil::ucs2ToUtf8(to, from);
        *text = to;
        //printf("content: %s\n",to.c_str());
    }
    Builder.clear();
    if(IsEnd(myReadStart)){
        return false;
    } else {
        return true;
    }
}
ParagraphTextSnippet::ParagraphTextSnippet(shared_ptr<ZLTextWordCursor> start,shared_ptr<ZLTextWordCursor> pageEnd)
{
    myReadStart = new ZLTextWordCursor(start);
    shared_ptr<ZLTextWordCursor>
    cursor = new ZLTextWordCursor(start);
    myPageEnd = new ZLTextWordCursor(pageEnd);
   /* while (true)
    {
        if(IsEnd(cursor)){
            break;
        }
        while (cursor->isEndOfParagraph()) {
            if (!cursor->nextParagraph()) {
                break ;
            }
            if(cursor->getParagraphCursor()->isLikeEndOfSection()){
                printf("PAGE END\n");
                break;
            }
        }
        bool needbreak= false;
        
        while(!cursor->isEndOfParagraph()&&!IsEnd(cursor)){
            shared_ptr<ZLTextElement> element = cursor->getElement();
            ZLTextWord& word = (ZLTextWord&)*element;
            if (element->kind() == ZLTextElement::WORD_ELEMENT) {
                append(&word.Data[word.Offset], word.Length);
            }
            
            cursor->nextWord();
        }
        if(Builder.size()>0){
            std::string to="";
            ZLUnicodeUtil::Ucs2String from;
            for(int i=0;i<Builder.size();i++){
                from.insert(from.end(), Builder[i]);
            }
            ZLUnicodeUtil::ucs2ToUtf8(to, from);
            printf("content: %s\n",to.c_str());
        }
        Builder.clear();
        if(IsEnd(cursor)){
            break;
        }
        
    }*/
}