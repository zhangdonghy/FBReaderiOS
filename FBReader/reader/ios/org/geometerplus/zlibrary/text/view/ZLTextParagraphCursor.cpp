//
//  ZLTextParagraphCursor.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/1.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include <vector>

#include "ZLTextParagraphCursor.hpp"
#include "CursorManager.hpp"
#include "ZDZLTextParagraph.hpp"
#include "ZLTextWord.hpp"
#include "ZLTextHyperlink.hpp"
#include "ZDZLImage.hpp"
#include "ZLTextFixedHSpaceElement.hpp"
#include "ZLTextControlElement.hpp"
#include "ZLTextHyperlinkControlElement.hpp"
#include "ZDZLImageEntry.hpp"
#include "ZLTextStyleElement.hpp"
#include "ZLTextImageElement.hpp"
#include "ZLImageManager.hpp"


#include "ZLUnicodeUtil.h"

const short ZLTextParagraphCursor::SPACE_ARRAY[1] = {' '};

uint8_t* ZLTextParagraphCursor::ourBreaks = 0;//[1024]={0};
ZLTextParagraphCursor::~ZLTextParagraphCursor()
{
    //if(ourBreaks != 0) delete ourBreaks;
}
ZLTextParagraphCursor::Processor::Processor(shared_ptr<ZDZLTextParagraph> paragraph, shared_ptr<ExtensionElementManager> extManager, shared_ptr<LineBreaker> lineBreaker,std::vector<shared_ptr<ZDZLTextMark>> marks, int paragraphIndex, std::vector<shared_ptr<ZLTextElement>> elements
                                            ,shared_ptr<ZLTextParagraphCursor> cursor)
{
    myExtManager = extManager;
    myParagraph = paragraph;
    myLineBreaker = lineBreaker;
   // myElements = elements;
    myCursor = cursor;
    myMarks = marks;
    shared_ptr<ZDZLTextMark> mark = new ZDZLTextMark(paragraphIndex, 0, 0);
    int i;
    for (i = 0; i < myMarks.size(); i++) {
        if (myMarks[i]->compareTo(*mark) >= 0) {
            break;
        }
    }
    myFirstMark = i;
    myLastMark = myFirstMark;
    for (; myLastMark != myMarks.size() && (myMarks[myLastMark])->ParagraphIndex == paragraphIndex; myLastMark++);
    myOffset = 0;
}
void ZLTextParagraphCursor::Processor::addWord(short* data, int offset, int len, int paragraphOffset, shared_ptr<ZLTextHyperlink> hyperlink) {
    //ZLTextWord* word = new ZLTextWord(data, offset, len, paragraphOffset);
    
    shared_ptr<ZLTextElement> word = new ZLTextWord(data, offset, len, paragraphOffset);
    for (int i = myFirstMark; i < myLastMark; ++i) {
        shared_ptr<ZDZLTextMark> mark = (shared_ptr<ZDZLTextMark>)myMarks[i];//.get(i);
        if ((mark->Offset < paragraphOffset + len) && (mark->Offset + mark->Length > paragraphOffset)) {
            ((ZLTextWord&)*word).addMark(mark->Offset - paragraphOffset, mark->Length);
        }
    }
    if (hyperlink != 0) {
        hyperlink->addElementIndex(myCursor->myElements.size());
    }
    myCursor->myElements.insert(myCursor->myElements.end(), word);
}
void ZLTextParagraphCursor::Processor::processTextEntry( short* data,  int offset,  int length, shared_ptr<ZLTextHyperlink> hyperlink)
{
    if (length != 0) {
       // if (ourBreaks.length < length) {
        if(1024<length){
            delete  ourBreaks;
            ourBreaks = new uint8_t[length];
        }
       // byte[] breaks = ourBreaks;
        //char* data, int offset, int length, uint8_t* breaks,uint16_t breaks_len
        myLineBreaker->setLineBreaks(data, offset, length, ourBreaks,1024);
        
       // shared_ptr<ZLTextElement> hSpace = ZLTextElement::HSpace;
        //shared_ptr<ZLTextElement> nbSpace = ZLTextElement::NBSpace;
        //std::vector<shared_ptr<ZLTextElement>> elements = myCursor->myElements;
        short ch = 0;
        short previousChar = 0;
        int spaceState = NO_SPACE;
        int wordStart = 0;
        for (int index = 0; index < length; ++index) {
            previousChar = ch;
            ch = data[offset + index];
            if (ZLUnicodeUtil::isSpace(ch)) {
                if (index > 0 && spaceState == NO_SPACE) {
                    addWord(data, offset + wordStart, index - wordStart, myOffset + wordStart, hyperlink);
                }
                spaceState = SPACE;
            } else if (ZLUnicodeUtil::isNBSpace(ch)) {
                if (index > 0 && spaceState == NO_SPACE) {
                    addWord(data, offset + wordStart, index - wordStart, myOffset + wordStart, hyperlink);
                }
                myCursor->myElements.insert(myCursor->myElements.end(),ZLTextElement::NBSpace);
                if (spaceState != SPACE) {
                    spaceState = NON_BREAKABLE_SPACE;
                }
            } else {
                switch (spaceState) {
                    case SPACE:
                    //if (breaks[index - 1] == LineBreak.NOBREAK || previousChar == '-') {
                    //}
                   // elements.add(hSpace);
                    myCursor->myElements.insert(myCursor->myElements.end(),ZLTextElement::HSpace);
                    wordStart = index;
                    break;
                    case NON_BREAKABLE_SPACE:
                    wordStart = index;
                    break;
                    case NO_SPACE:
                    if (index > 0 &&
                        ourBreaks[index - 1] != LineBreaker::NOBREAK &&
                        previousChar != '-' &&
                        index != wordStart) {
                        addWord(data, offset + wordStart, index - wordStart, myOffset + wordStart, hyperlink);
                        wordStart = index;
                    }
                    break;
                }
                spaceState = NO_SPACE;
            }
        }
        switch (spaceState) {
            case SPACE:
           // elements.add(hSpace);
            myCursor->myElements.insert(myCursor->myElements.end(),ZLTextElement::HSpace);
            break;
            case NON_BREAKABLE_SPACE:
           // elements.add(nbSpace);
            myCursor->myElements.insert(myCursor->myElements.end(),ZLTextElement::NBSpace);
            break;
            case NO_SPACE:
            addWord(data, offset + wordStart, length - wordStart, myOffset + wordStart, hyperlink);
            break;
        }
        myOffset += length;
    }
}




void
ZLTextParagraphCursor::Processor::fill()
{
    int hyperlinkDepth = 0;
    shared_ptr<ZLTextHyperlink> hyperlink = 0;
    
   // std::vector<shared_ptr<ZLTextElement>> elements = myElements;
    for (shared_ptr<ZDZLTextParagraph::EntryIterator> it = myParagraph->iterator(); it->next(); ) {
        switch ((ZDZLTextParagraph::Entry)it->getType()) {
            case ZDZLTextParagraph::TEXT:
            processTextEntry(it->getTextData(), it->getTextOffset(), it->getTextLength(), hyperlink);
            break;
            case ZDZLTextParagraph::CONTROL:
            {
            if (hyperlink != 0) {
                hyperlinkDepth += it->getControlIsStart() ? 1 : -1;
                if (hyperlinkDepth == 0) {
                    hyperlink = 0;
                }
            }
            //elements.add(ZLTextControlElement::get(it->getControlKind(), it->getControlIsStart()));
            myCursor->myElements.insert( myCursor->myElements.end(),
                                        (ZLTextControlElement::get(it->getControlKind(), it->getControlIsStart()))
                                        );
            }
            break;
            case ZDZLTextParagraph::HYPERLINK_CONTROL:
            {
                uint8_t hyperlinkType = it->getHyperlinkType();
                if (hyperlinkType != 0) {
                    //shared_ptr<ZLTextHyperlinkControlElement> control =
                    //new ZLTextHyperlinkControlElement(
                                                    //  it->getControlKind(), hyperlinkType, it->getHyperlinkId()
                                                      //);
                   // elements.add(control);
                   //ZLTextHyperlinkControlElement* control  =  new ZLTextHyperlinkControlElement(
                    shared_ptr<ZLTextElement> control  =  new ZLTextHyperlinkControlElement(
                                                      it->getControlKind(), hyperlinkType, it->getHyperlinkId()
                                                      );
                     myCursor->myElements.insert( myCursor->myElements.end(),
                                                control
                                                 );
                    hyperlink = ((ZLTextHyperlinkControlElement&)*control).Hyperlink;
                    hyperlinkDepth = 1;
                }
                break;
            }
            case ZDZLTextParagraph::IMAGE:
            {
             shared_ptr<ZDZLImageEntry> imageEntry = it->getImageEntry();
             shared_ptr<ZLFileImage> image = imageEntry->getImage();
            
                if (image != 0) {
                   // shared_ptr<ZLImageData> data = ZLImageManager::Instance();
                    shared_ptr<ZLImageData> data = ZLImageManager::Instance().getImageData(image);
                    
                    //if (data != 0)
                    {
                        //if (hyperlink != 0) {
                         //   hyperlink->addElementIndex(myCursor->myElements.size());
                        //}
                         myCursor->myElements.insert(myCursor->myElements.end(),
                                                     new ZLTextImageElement(imageEntry->Id, data, imageEntry->Id, imageEntry->IsCover,
                                                                           image ));
                    }
                }
            
            }
            break;
           /* case ZDZLTextParagraph.Entry.AUDIO:
            break;
            case ZDZLTextParagraph.Entry.VIDEO:
            elements.add(new ZLTextVideoElement(it.getVideoEntry().sources()));
            break;
            case ZDZLTextParagraph.Entry.EXTENSION:
            if (myExtManager != null) {
                elements.addAll(myExtManager.getElements(it.getExtensionEntry()));
            }*/
           // break;
            case ZDZLTextParagraph::STYLE_CSS :
            case ZDZLTextParagraph::STYLE_OTHER:
            //elements.add(new ZLTextStyleElement(it.getStyleEntry()));
            {
                myCursor->myElements.insert( myCursor->myElements.end(),
                                            new ZLTextStyleElement(it->getStyleEntry())
                                            );
            }
           
            break;
            case ZDZLTextParagraph::STYLE_CLOSE:
           // elements.add(ZLTextElement::StyleClose);
            myCursor->myElements.insert( myCursor->myElements.end(),
                                        ZLTextElement::StyleClose
                                        );
            break;
            case ZDZLTextParagraph::FIXED_HSPACE:
            //elements.add(ZLTextFixedHSpaceElement.getElement(it.getFixedHSpaceLength()));
            myCursor->myElements.insert( myCursor->myElements.end(),
                                        ZLTextFixedHSpaceElement::getElement(it->getFixedHSpaceLength())
                                        );
            break;
            default:
            break;
        }
    }
}
ZLTextParagraphCursor::ZLTextParagraphCursor(shared_ptr<ZDZLTextModel> model, int index)
{
    //std::list<shared_ptr<ZLTextElement>> myElements = new std::list<shared_ptr<ZLTextElement>> ();
    CursorManager* newm = new CursorManager(model,0);
    ZLTextParagraphCursor(newm,model,index);
    ourBreaks = new uint8_t[1024];
    
}
ZLTextParagraphCursor::ZLTextParagraphCursor(CursorManager* manager, shared_ptr<ZDZLTextModel>  model, int index)
{
    ourBreaks = new uint8_t[1024];
    if(cursorManager != manager) {
        cursorManager = manager;
    }
    Model = model;
    Index = std::min(index, model->getParagraphsNumber());
    fill();
    
}

int ZLTextParagraphCursor::hashCode()
{
    // zhangdong 
    return 100;
}
void
ZLTextParagraphCursor::fill() {
    shared_ptr<ZDZLTextParagraph> paragraph = Model->getParagraph(Index);
    switch (paragraph->getKind()) {
        case ZDZLTextParagraph::TEXT_PARAGRAPH:
        {
            LineBreaker* b = new LineBreaker(Model->getLanguage());
            Processor* p = new Processor(paragraph, cursorManager->ExtensionManager, b, Model->getMarks(), Index, myElements,this);
            p->fill();
        }
                break;
        case ZDZLTextParagraph::EMPTY_LINE_PARAGRAPH:
        myElements.insert(myElements.end(), new ZLTextWord(SPACE_ARRAY,0,1,0));//.add(new ZLTextWord(SPACE_ARRAY, 0, 1, 0));
        break;
        case ZDZLTextParagraph::ENCRYPTED_SECTION_PARAGRAPH:
        {
           /* final ZLTextStyleEntry entry = new ZLTextOtherStyleEntry();
            entry.setFontModifier(ZLTextStyleEntry.FontModifier.FONT_MODIFIER_BOLD, true);
            myElements.add(new ZLTextStyleElement(entry));
            myElements.add(new ZLTextWord(ZLResource.resource("drm").getResource("encryptedSection").getValue(), 0));*/
            break;
        }
        default:
        break;
    }
}
