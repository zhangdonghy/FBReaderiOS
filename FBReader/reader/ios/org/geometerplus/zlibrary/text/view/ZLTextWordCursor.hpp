//
//  ZLTextWordCursor.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/1.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLTextWordCursor_hpp
#define ZLTextWordCursor_hpp

#include <stdio.h>
#include "ZLTextPosition.hpp"
#include "shared_ptr.h"
#include "ZLTextParagraphCursor.hpp"
#include "ZLTextWord.hpp"

class ZLTextWordCursor: public ZLTextPosition
{
private:
    shared_ptr<ZLTextParagraphCursor> myParagraphCursor;
    int myElementIndex;
    int myCharIndex;
protected:
public:
    ZLTextWordCursor();
    ZLTextWordCursor(shared_ptr<ZLTextParagraphCursor> cursor,int elementIndex,int charIndex);
    ZLTextWordCursor(shared_ptr<ZLTextWordCursor> cursor) ;
    void setCursor(const ZLTextWordCursor& cursor) ;
    ZLTextWordCursor(shared_ptr<ZLTextParagraphCursor> cursor) ;
    void setCursor(shared_ptr<ZLTextParagraphCursor> cursor) ;
    
    bool isNull() ;
    
    
    shared_ptr<ZLTextParagraphCursor> getParagraphCursor() {
        return myParagraphCursor;
    }
    
    
    bool isStartOfParagraph() {
        return myElementIndex == 0 && myCharIndex == 0;
    }
    
    bool isStartOfText() {
        return isStartOfParagraph() && myParagraphCursor->isFirst();
    }
    
    bool isEndOfParagraph() {
        return
        myParagraphCursor != 0 &&
        myElementIndex == myParagraphCursor->getParagraphLength();
    }
    
    bool isEndOfText() {
        return isEndOfParagraph() && myParagraphCursor->isLast();
    }
    bool isLastParagraph(){
        return
        myParagraphCursor != 0 && myParagraphCursor->isLast();
    }
    virtual int getParagraphIndex() const {
        return myParagraphCursor != 0x0 ? myParagraphCursor->Index : 0;
    }
    
    virtual int getElementIndex() const{
        return myElementIndex;
    }
    
    virtual int getCharIndex() const{
        return myCharIndex;
    }
    
    void reset() {
        myParagraphCursor = 0;
        myElementIndex = 0;
        myCharIndex = 0;
    }
    
    bool nextParagraph() {
        if (!isNull()) {
            if (!myParagraphCursor->isLast()) {
                myParagraphCursor = myParagraphCursor->next();
                moveToParagraphStart();
                return true;
            }
        }
        return false;
    }
    void moveToParagraphStart() {
        if (!isNull()) {
            myElementIndex = 0;
            myCharIndex = 0;
        }
    }
    void moveToParagraphEnd() {
        if (!isNull()) {
            myElementIndex = myParagraphCursor->getParagraphLength();
            myCharIndex = 0;
        }
    }
    bool previousParagraph() {
        if (!isNull()) {
            if (!myParagraphCursor->isFirst()) {
                myParagraphCursor = myParagraphCursor->previous();
                moveToParagraphStart();
                return true;
            }
        }
        return false;
    }
    void moveToParagraph(int paragraphIndex) {
        if (!isNull() && (paragraphIndex != myParagraphCursor->Index)) {
            shared_ptr<ZDZLTextModel> model = myParagraphCursor->Model;
            paragraphIndex = std::max(0, std::min(paragraphIndex, model->getParagraphsNumber() - 1));
            myParagraphCursor = myParagraphCursor->cursorManager->get(paragraphIndex);
            moveToParagraphStart();
        }
    }
    void moveTo(int wordIndex, int charIndex) {
        if (!isNull()) {
            if (wordIndex == 0 && charIndex == 0) {
                myElementIndex = 0;
                myCharIndex = 0;
            } else {
                wordIndex = std::max(0, wordIndex);
                int size = myParagraphCursor->getParagraphLength();
                if (wordIndex > size) {
                    myElementIndex = size;
                    myCharIndex = 0;
                } else {
                    myElementIndex = wordIndex;
                    setCharIndex(charIndex);
                }
            }
        }
    }
    void setCharIndex(int charIndex) {
        charIndex = std::max(0, charIndex);
        myCharIndex = 0;
        if (charIndex > 0) {
            shared_ptr<ZLTextElement> element = myParagraphCursor->getElement(myElementIndex);
            //element.getT();
            //if (element instanceof ZLTextWord)
            {
                if (charIndex <= ((const ZLTextWord&)*element).Length) {
                    myCharIndex = charIndex;
                }
            }
        }
    }
    void moveTo(shared_ptr<ZLTextPosition> position) {
        moveToParagraph(position->getParagraphIndex());
        moveTo(position->getElementIndex(), position->getCharIndex());
    }
    shared_ptr<ZLTextElement> getElement() {
        return myParagraphCursor->getElement(myElementIndex);
    }
    void nextWord() {
        myElementIndex++;
        myCharIndex = 0;
    }
    
    void previousWord() {
        myElementIndex--;
        myCharIndex = 0;
    }
    
    void rebuild() {
        if (!isNull()) {
            myParagraphCursor->clear();
            myParagraphCursor->fill();
            moveTo(myElementIndex, myCharIndex);
        }
    }
    shared_ptr<ZDZLTextMark> getMark() {
        if (myParagraphCursor == NULL) {
            return NULL;
        }
        shared_ptr<ZLTextParagraphCursor> paragraph = myParagraphCursor;
        int paragraphLength = paragraph->getParagraphLength();
        int wordIndex = myElementIndex;
        while ((wordIndex < paragraphLength) && (paragraph->getElement(wordIndex)->kind() != ZLTextElement::WORD_ELEMENT)) {
            wordIndex++;
        }
        if (wordIndex < paragraphLength) {
            return new ZDZLTextMark(paragraph->Index, ((ZLTextWord&)*paragraph->getElement(wordIndex)).getParagraphOffset(), 0);
        }
        return new ZDZLTextMark(paragraph->Index + 1, 0, 0);
    }
};
inline  bool ZLTextWordCursor::isNull() {
    return myParagraphCursor == 0;
}

#endif /* ZLTextWordCursor_hpp */
