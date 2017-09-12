//
//  AutoTextSnippet.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/21.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef AutoTextSnippet_hpp
#define AutoTextSnippet_hpp

#include <vector>
#include <stdio.h>
#include "TextSnippet.hpp"
#include "ZLTextWordCursor.hpp"
#include "ZLTextControlElement.hpp"
#include "FBTextKind.h"
#include "ZLTextFixedPosition.hpp"
#include "ZLUnicodeUtil.h"

class AutoTextSnippet : public TextSnippet
{
    public:
    class Buffer {
       // StringBuilder Builder = new StringBuilder();
        public:
        std::vector<short> Builder;
        //shared_ptr<ZLTextWordCursor> Cursor;
        shared_ptr<ZLTextPosition> Cursor;
        Buffer(shared_ptr<ZLTextWordCursor> cursor) {
            Cursor = new ZLTextWordCursor(cursor);
        }
        
        bool isEmpty() {
           // return Builder.length() == 0;
            return  Builder.size() ==0;
        }
        
        void append( shared_ptr<Buffer> buffer) {
            //Builder.append(buffer.Builder);
            //Builder.insert(Builder.end(), buffer->Builder);
            for(int i=0;i<buffer->Builder.size();i++){
                Builder.insert(Builder.end(), buffer->Builder[i]);
            }
           // Cursor->setCursor(buffer->Cursor);
            ((ZLTextWordCursor&)*Cursor).setCursor((const ZLTextWordCursor&)(*(buffer->Cursor)));
            buffer->Builder.clear();
            //buffer.Builder.delete(0, buffer.Builder.length());
        }
        
        //void append(CharSequence data) {
        void append(const short* data,int length)
        {
            //Builder.append(data);
            for(int i=0;i<length;i++){
                Builder.insert(Builder.end(), data[i]);
            }
        }
        std::string toString(){
            //std::vector<short> val = traverser->getText();
            std::string to="";
            ZLUnicodeUtil::Ucs2String from;
            for(int i=0;i<Builder.size();i++){
                from.insert(from.end(), Builder[i]);
            }
            ZLUnicodeUtil::ucs2ToUtf8(to, from);
            return to;
        }
    };
private:
    shared_ptr<ZLTextPosition> myStart;
    shared_ptr<ZLTextPosition> myEnd;
    std::string myText;
    bool IsEndOfText;
public:
    AutoTextSnippet(shared_ptr<ZLTextWordCursor> start, int maxChars)
    {
        
        shared_ptr<ZLTextWordCursor>
            cursor = new ZLTextWordCursor(start);
        
        shared_ptr<Buffer> buffer = new Buffer(cursor);
        shared_ptr<Buffer> sentenceBuffer = new Buffer(cursor);
        shared_ptr<Buffer> phraseBuffer = new Buffer(cursor);
        IsEndOfText = false;
        int wordCounter = 0;
        int sentenceCounter = 0;
        int storedWordCounter = 0;
        bool lineIsNonEmpty = false;
        bool appendLineBreak = false;
        
        while (
               buffer->Builder.size() + sentenceBuffer->Builder.size() + phraseBuffer->Builder.size() < maxChars
               && sentenceCounter < maxChars / 20) {
            bool needcontinue = false;
            short tmpShort =0;
            while (cursor->isEndOfParagraph()) {
                if (!cursor->nextParagraph()) {
                    needcontinue = true;
                    break ;
                }
                if (!buffer->isEmpty() && cursor->getParagraphCursor()->isLikeEndOfSection()) {
                    needcontinue = true;
                    break ;
                }
                if (!phraseBuffer->isEmpty()) {
                    sentenceBuffer->append(phraseBuffer);
                }
                if (!sentenceBuffer->isEmpty()) {
                    if (appendLineBreak) {
                        tmpShort = '\n';
                        buffer->append(&tmpShort, 1);
                        //buffer.append("\n");
                    }
                    buffer->append(sentenceBuffer);
                    ++sentenceCounter;
                    storedWordCounter = wordCounter;
                }
                lineIsNonEmpty = false;
                if (!buffer->isEmpty()) {
                    appendLineBreak = true;
                }
                
            }
            if(needcontinue) {
                break;
            }
            shared_ptr<ZLTextElement> element = cursor->getElement();
            if (element == ZLTextElement::HSpace) {
                if (lineIsNonEmpty) {
                    tmpShort = ' ';
                    phraseBuffer->append(&tmpShort, 1);
                    //phraseBuffer.append(" ");
                }
                }else if (element == ZLTextElement::NBSpace) {
                    if (lineIsNonEmpty) {
                        tmpShort = '\240';
                        phraseBuffer->append(&tmpShort, 1);
                        //phraseBuffer.append("\240");
                    }
                }else if (element->kind() == ZLTextElement::WORD_ELEMENT/* instanceof ZLTextWord*/) {
                    ZLTextWord& word = (ZLTextWord&)*element;
                    //phraseBuffer->Builder.append(&word.Data[word.Offset])//(word.Data, word.Offset, word.Length);
                    phraseBuffer->append(&word.Data[word.Offset], word.Length);
                    
                    ((ZLTextWordCursor&)*(phraseBuffer->Cursor)).setCursor(cursor);
                    ((ZLTextWordCursor&)*(phraseBuffer->Cursor)).setCharIndex(word.Length);
                    ++wordCounter;
                    lineIsNonEmpty = true;
                    switch (word.Data[word.Offset + word.Length - 1]) {
                        case ',':
                        case ':':
                        case ';':
                        case ')':
                        sentenceBuffer->append(phraseBuffer);
                        break;
                        case '.':
                        case '!':
                        case '?':
                        ++sentenceCounter;
                        if (appendLineBreak) {
                            tmpShort = '\n';
                            buffer->append(&tmpShort, 1);
                            //buffer.append("\n");
                            appendLineBreak = false;
                        }
                        sentenceBuffer->append(phraseBuffer);
                        buffer->append(sentenceBuffer);
                        storedWordCounter = wordCounter;
                        break;
                    }
                } else if (element->kind() ==ZLTextElement::TEXT_CONTROL_ELEMENT/* instanceof ZLTextControlElement*/) {
                    ZLTextControlElement& control = (ZLTextControlElement&)*element;
                    if (control.IsStart) {
                        switch (control.Kind) {
                            case H1:
                            case H2:
                            if (!buffer->isEmpty()) {
                                needcontinue = true;
                                break;
                               // break mainLoop;
                            }
                            break;
                        }
                    }
                }
                if(needcontinue) {
                    break;
                }
                cursor->nextWord();
           // }
            
        }
        
        IsEndOfText =
        cursor->isEndOfText() || cursor->getParagraphCursor()->isLikeEndOfSection();
        
        if (IsEndOfText) {
            sentenceBuffer->append(phraseBuffer);
            if (appendLineBreak) {
                short tmpShort =0;
                tmpShort = '\n';
                buffer->append(&tmpShort, 1);
                //buffer.append("\n");
            }
            buffer->append(sentenceBuffer);
        } else if (storedWordCounter < 4 || sentenceCounter < maxChars / 30) {
            if (sentenceBuffer->isEmpty()) {
                sentenceBuffer->append(phraseBuffer);
            }
            if (appendLineBreak) {
                short tmpShort =0;
                tmpShort = '\n';
                buffer->append(&tmpShort, 1);
                //buffer.append("\n");
            }
            buffer->append(sentenceBuffer);
        }
        
        myStart = new ZLTextFixedPosition(*start);
        myEnd = buffer->Cursor;
        myText = buffer->toString();// .Builder.toString();
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
#endif /* AutoTextSnippet_hpp */
