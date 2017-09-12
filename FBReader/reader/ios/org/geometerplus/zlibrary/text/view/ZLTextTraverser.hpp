//
//  ZLTextTraverser.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/10.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLTextTraverser_hpp
#define ZLTextTraverser_hpp

#include <stdio.h>
#include "ZLTextView.hpp"
#include "ZLTextControlElement.hpp"

class ZLTextTraverser
{
    private:
    ZLTextView*  myView;
    protected:
    virtual void processWord(ZLTextWord& word)=0;
    virtual void processControlElement(ZLTextControlElement& control)=0;
    virtual void processSpace()=0;
    virtual void processNbSpace()=0;
    virtual void processEndOfParagraph()=0;
    ZLTextTraverser(ZLTextView*  view) {
        myView = view;
    }
    ~ZLTextTraverser();
    public:
    void traverse(ZLTextPosition* from, ZLTextPosition* to) {
        int fromParagraph = from->getParagraphIndex();
        int toParagraph = to->getParagraphIndex();
        shared_ptr<ZLTextParagraphCursor> cursor = myView->cursor(fromParagraph);
        for (int i = fromParagraph; i <= toParagraph; ++i) {
//            printf("paragraph: %d, length: %d\n", i, cursor->getParagraphLength());
            int fromElement = i == fromParagraph ? from->getElementIndex() : 0;
            int toElement = i == toParagraph ? to->getElementIndex() : cursor->getParagraphLength() - 1;
//            printf("fromElement: %d, toElement: %d\n", fromElement, toElement);
            for (int j = fromElement; j <= toElement; j++) {
                shared_ptr<ZLTextElement> element = cursor->getElement(j);
                if (!element.isNull()) {
                    if (element->kind() == ZLTextElement::HSPACE_ELEMENT/*ZLTextElement::HSpace*/) {
                        processSpace();
                    } else if (element->kind()  == ZLTextElement::NB_HSPACE_ELEMENT/*== ZLTextElement.NBSpace*/) {
                        processNbSpace();
                    } else if (element->kind() == ZLTextElement::WORD_ELEMENT /*instanceof ZLTextWord*/) {
                        processWord((ZLTextWord&)*element);
                    }
                }
            }
            if (i < toParagraph) {
                processEndOfParagraph();
                cursor = cursor->next();
            }
        }
    }
};
#endif /* ZLTextTraverser_hpp */
