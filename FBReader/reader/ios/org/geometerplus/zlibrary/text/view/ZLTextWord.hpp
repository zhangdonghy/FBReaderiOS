//
//  ZLTextWord.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/2.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLTextWord_hpp
#define ZLTextWord_hpp

#include "ZLPaintContext.hpp"

#include <stdio.h>
#include "ZLTextElement.hpp"
#include <string>




class ZLTextViewBase;
class ZLTextHyphenator;
class ZLTextWord: public ZLTextElement
{
    class Mark {
        public:
         int Start;
         int Length;
         Mark* myNext;
        ~Mark();
         Mark(int start, int length) {
            Start = start;
            Length = length;
            myNext = 0;
        }
        
         Mark* getNext() {
            return myNext;
        }
        
         void setNext(Mark* mark) {
            myNext = mark;
        }
        friend class ZLTextWord;
    };
public:
    ~ZLTextWord();
    ZLTextWord(const short* data,int offset,int length, int paragraphOffset);
    
    int Length;

// private
    const short* Data;
    int Offset;
private:
    int myWidth=-1;
    Mark* myMark;
    int myParagraphOffset;
    public:
    
     bool isASpace() {
        for (int i = Offset; i < Offset + Length; ++i) {
            if (!std::isspace(Data[i])) {
                return false;
            }
        }
        return true;
    }
    
     Mark* getMark() {
        return myMark;
    }
    
     int getParagraphOffset() {
        return myParagraphOffset;
    }
    
    void addMark(int start, int length) ;
    
    virtual Kind kind() const;
     int getWidth( shared_ptr<ZLPaintContext> context) {
        int width = myWidth;
        if (width <= 1) {
            width = context->getStringWidth(Data, Offset, Length);
            //context.LIGHTEN_TO_BACKGROUND
            myWidth = width;
        }
        return width;
    }
    
    virtual  std::string toString() {
        return "";//getString();
    }
    
    /*virtual std::string getString() {
        return  std::string(Data);
    }*/
    friend class TextBuildTraverser;
    friend class ZLTextViewBase;
    friend class ZLTextView;
    friend class ZLTextHyphenator;
    friend class AutoTextSnippet;
    friend class ParagraphTextSnippet;
};
#endif /* ZLTextWord_hpp */
