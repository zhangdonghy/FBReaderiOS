//
//  ZLTextParagraphCursor.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/1.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLTextParagraphCursor_hpp
#define ZLTextParagraphCursor_hpp

#include <stdio.h>
#include <vector>
#include "ZDZLTextModel.hpp"
#include "CursorManager.hpp"
#include "ZLTextElement.hpp"
#include "ZLTextParagraphCursor.hpp"
#include "ZDZLTextParagraph.hpp"
#include "CursorManager.hpp"
#include "ZDZLTextMark.hpp"
#include "LineBreaker.hpp"

class ZLTextHyperlink;
class ZLTextParagraphCursor
{
private:
   // std::list<shared_ptr<ZLTextElement>> myElements;// = new ArrayList<ZLTextElement>();
    std::vector<shared_ptr<ZLTextElement>> myElements;
    
    
    static uint8_t* ourBreaks;//[1024] ;
     static const int NO_SPACE = 0;
     static const int SPACE = 1;
     static const int NON_BREAKABLE_SPACE = 2;
    
    
    protected:
    
public:
    ~ZLTextParagraphCursor();
    class Processor
    {
        public:
        shared_ptr<ZDZLTextParagraph> myParagraph;
        shared_ptr<ExtensionElementManager> myExtManager;
        shared_ptr<LineBreaker> myLineBreaker;
       // std::vector<shared_ptr<ZLTextElement>> myElements;
         int myOffset;
         int myFirstMark;
         int myLastMark;
        std::vector<shared_ptr<ZDZLTextMark>> myMarks;
        shared_ptr<ZLTextParagraphCursor> myCursor;
        Processor(shared_ptr<ZDZLTextParagraph> paragraph, shared_ptr<ExtensionElementManager> extManager, shared_ptr<LineBreaker> lineBreaker,std::vector<shared_ptr<ZDZLTextMark>> marks, int paragraphIndex, std::vector<shared_ptr<ZLTextElement>> elements,shared_ptr<ZLTextParagraphCursor> cursor);
         void fill();
        void processTextEntry( short* data,  int offset,  int length, shared_ptr<ZLTextHyperlink> hyperlink);
        void addWord(short* data, int offset, int len, int paragraphOffset, shared_ptr<ZLTextHyperlink> hyperlink);
    };
   
    static const short SPACE_ARRAY[1] ;//= { ' ' };
    
    int Index;
    CursorManager* cursorManager;
    shared_ptr<ZDZLTextModel> Model;
    ZLTextParagraphCursor(shared_ptr<ZDZLTextModel> model, int index);
    ZLTextParagraphCursor(/*shared_ptr<CursorManager> cManager*/CursorManager*manager, shared_ptr<ZDZLTextModel>  model, int index) ;
    bool isFirst() {
        return Index == 0;
    }
    bool isLast() {
        return Index + 1 >= Model->getParagraphsNumber();
    }
    int getParagraphLength() {
        return (int)myElements.size();
    }
    shared_ptr<ZLTextParagraphCursor> next() {
        return isLast() ? 0 : cursorManager->get(Index + 1);
    }
    int hashCode();
    bool isEndOfSection() {
        return Model->getParagraph(Index)->getKind() == ZDZLTextParagraph::END_OF_SECTION_PARAGRAPH;
    }
    shared_ptr<ZLTextElement> getElement(int index) {
        //int x= test[5];
        return myElements[index];
    }
    shared_ptr<ZLTextParagraphCursor> previous() {
        return isFirst() ? 0 : cursorManager->get(Index - 1);
    }
    void fill();
    
    bool isLikeEndOfSection() {
        switch (Model->getParagraph(Index)->getKind()) {
            case ZDZLTextParagraph::END_OF_SECTION_PARAGRAPH:
            case ZDZLTextParagraph::PSEUDO_END_OF_SECTION_PARAGRAPH:
            return true;
            default:
            return false;
        }
    }
    void clear() {
        myElements.clear();
    }
    friend class Processer;
    
};
#endif /* ZLTextParagraphCursor_hpp */
