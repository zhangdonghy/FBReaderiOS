//
//  ZLTextView.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/31.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ZLTextView.hpp"
#include "ZLTextPage.hpp"
#include "ZLTextAlignmentType.h"
#include "ZLTextHyphenationInfo.hpp"
#include "ZLTextElementArea.hpp"
#include "ZLTextSelection.hpp"
#include "ZLTextStyleCollection.hpp"
#include "ZLViewWidget.hpp"
#include "AutoTextSnippet.hpp"

//#include <numeric_limits>

short ZLTextView::SPACE[1] = {' '};

ZLTextView::ZLTextView(ZLApplication* application)
:ZLTextViewBase(application)
{
    myPreviousPage = new ZLTextPage();
    myCurrentPage = new ZLTextPage();
    myNextPage = new ZLTextPage();
    
    mySelection = new ZLTextSelection(this);
    myShowOutline = false;
    myLettersModel = 0;
    myCharWidth = -1.0f;
    myLettersBuffer = (short*)malloc(sizeof(short)*512);;
   // mySelection = 0;
}
void ZLTextView::setModel(shared_ptr<ZDZLTextModel> model, int jumpParagraph)
{
    myCursorManager = model != 0 ? new CursorManager(model, getExtensionManager()) : 0;
    
    mySelection->clear();
    myHighlightings.clear();
    myReadHighlightings.clear();
    myModel = model;
    myCurrentPage->reset();
    myPreviousPage->reset();
    myNextPage->reset();
    if (myModel != 0) {
        int paragraphsNumber = myModel->getParagraphsNumber();
        if (paragraphsNumber > 0) {
            myCurrentPage->moveStartCursor(myCursorManager->get(jumpParagraph));
        }
    }
    //Application.getViewWidget().reset();
}
std::vector<ZLTextHighlighting*>
ZLTextView::findHilites(ZLTextPage& page) {
    std::vector<ZLTextHighlighting*> hilites;// = new LinkedList<ZLTextHighlighting>();
    if (mySelection->intersects(page)) {
        hilites.insert(hilites.end(), &(*mySelection));
    }
    //synchronized (myHighlightings)
    {
        //for (ZLTextHighlighting h : myHighlightings) {
        for(int i=0;i<myHighlightings.size();i++){
            if (myHighlightings[i]->intersects(page)) {
                hilites.insert(hilites.end(),&(*myHighlightings[i]));
            }
        }
    }
    for(int i=0;i<myReadHighlightings.size();i++){
        if (myReadHighlightings[i]->intersects(page)) {
            hilites.insert(hilites.end(),&(*myReadHighlightings[i]));
        }
    }

    return hilites;
}

void ZLTextView::onScrollingFinished(int pageIndex)
{
    if(pageIndex == ZLViewEnums_PageIndex::next){
        shared_ptr<ZLTextPage> swap=myPreviousPage;
        myPreviousPage = myCurrentPage;
        myCurrentPage = myNextPage;
        myNextPage = swap;
        swap->reset();
        switch (myCurrentPage->PaintState) {
            case PaintStateEnum::NOTHING_TO_PAINT:
            {
                preparePaintInfo(myPreviousPage);
                myCurrentPage->StartCursor->setCursor(myPreviousPage->EndCursor);
                myCurrentPage->PaintState = PaintStateEnum::START_IS_KNOWN;
            }
            break;
            case PaintStateEnum::READY:
            {
                myNextPage->StartCursor->setCursor(myCurrentPage->EndCursor);
                myNextPage->PaintState= PaintStateEnum::START_IS_KNOWN;
            }   
            default:
            break;
        }
    } else if(pageIndex == ZLViewEnums_PageIndex::previous){
        shared_ptr<ZLTextPage> swap = myNextPage;
        myNextPage = myCurrentPage;
        myCurrentPage = myPreviousPage;
        myPreviousPage = swap;
        myPreviousPage->reset();
        if (myCurrentPage->PaintState == PaintStateEnum::NOTHING_TO_PAINT) {
            preparePaintInfo(myNextPage);
            myCurrentPage->EndCursor->setCursor(myNextPage->StartCursor);
            myCurrentPage->PaintState = PaintStateEnum::END_IS_KNOWN;
        } else if (!myCurrentPage->EndCursor->isNull() &&
                   !myNextPage->StartCursor->isNull() &&
                   !myCurrentPage->EndCursor->samePositionAs(*myNextPage->StartCursor)) {
            myNextPage->reset();
            myNextPage->StartCursor->setCursor(myCurrentPage->EndCursor);
            myNextPage->PaintState = PaintStateEnum::START_IS_KNOWN;
            Application->getViewWidget()->reset();
        }
    }
}

void ZLTextView::paint(shared_ptr<ZLPaintContext> context, int pageIndex) {
//    printf("ZLTextView paint pageIndex: %d\n", pageIndex);
    setContext(context);
    context->clear(getBackgroundColor());
    context->setLineColor(getSelectionBackgroundColor());
    context->setLineWidth(4);

//    shared_ptr<ZLFile> wallpaper = getWallpaperFile();
//    if (wallpaper != null) {
//        context.clear(wallpaper, getFillMode());
//    } else {
//        context.clear(getBackgroundColor());
//    }
    
    if (myModel == 0x0 || myModel->getParagraphsNumber() == 0) {
        return;
    }
    
    shared_ptr<ZLTextPage> page;
    switch (pageIndex) {
        default:
        case ZLViewEnums_PageIndex::current:
            page = myCurrentPage;
            break;
        case ZLViewEnums_PageIndex::previous:
            page = myPreviousPage;
            if (myPreviousPage->PaintState == PaintStateEnum::NOTHING_TO_PAINT) {
                preparePaintInfo(myCurrentPage);
                myPreviousPage->EndCursor->setCursor(myCurrentPage->StartCursor);
                myPreviousPage->PaintState = PaintStateEnum::END_IS_KNOWN;
            }
            break;
        case ZLViewEnums_PageIndex::next:
            page = myNextPage;
            if (myNextPage->PaintState == PaintStateEnum::NOTHING_TO_PAINT) {
                preparePaintInfo(myCurrentPage);
                myNextPage->StartCursor->setCursor(myCurrentPage->EndCursor);
                myNextPage->PaintState = PaintStateEnum::START_IS_KNOWN;
            }
    }
    
    page->TextElementMap->clear();
    
    preparePaintInfo(page);
    
    if (page->StartCursor->isNull() || page->EndCursor->isNull()) {
        return;
    }
    
    std::vector<shared_ptr<ZLTextLineInfo>> lineInfos = page->LineInfos;
    int* labels = new int[lineInfos.size() + 1];
    // 对labels初始化全部赋值为0，防止野指针出现
    for (int i = 0; i < lineInfos.size() + 1; i++) {
        labels[i] = 0;
    }
    int x = getLeftMargin();
    int y = getTopMargin();
    int index = 0;
    int columnIndex = 0;
    shared_ptr<ZLTextLineInfo> previousInfo = 0;
    for(std::vector<shared_ptr<ZLTextLineInfo>>::const_iterator it =  lineInfos.begin();it != lineInfos.end();++it) {
        shared_ptr<ZLTextLineInfo> info = *it;
        info->adjust(previousInfo);
        prepareTextLine(page, info, x, y, columnIndex);
        y += info->Height + info->Descent + info->VSpaceAfter;
        labels[++index] = page->TextElementMap->size();
        if (index == page->Column0Height) {
            y = getTopMargin();
            x += page->getTextWidth() + getSpaceBetweenColumns();
            columnIndex = 1;
        }
        previousInfo = info;
    }
    
    std::vector<ZLTextHighlighting*> hilites = findHilites(*page);

    x = getLeftMargin();
    y = getTopMargin();
    index = 0;
    for(std::vector<shared_ptr<ZLTextLineInfo>>::const_iterator it =  lineInfos.begin();it != lineInfos.end();++it) {
        shared_ptr<ZLTextLineInfo> info = *it;
        drawTextLine(page, hilites, info, labels[index], labels[index + 1]);
        y += info->Height + info->Descent + info->VSpaceAfter;
        ++index;
        if (index == page->Column0Height) {
            y = getTopMargin();
            x += page->getTextWidth() + getSpaceBetweenColumns();
        }
        
    }
    for(std::vector<ZLTextHighlighting*>::const_iterator it =  hilites.begin();it != hilites.end();++it) {
        ZLTextHighlighting* highlight = *it;
        int mode = Hull::None;
        int noteCount = 0;
        shared_ptr<ZLColor> color = 0;
        
        if (highlight->isUnderline()) {
            noteCount = highlight->getNoteCount();
            mode = Hull::Underline;
            color = highlight->getUnderlineColor();
            
        } else if (highlight->isOpinion()) {
//            printf("opinion highlight start pos: %d, %d, %d\n",
//                   highlight->getStartPosition()->getParagraphIndex(),
//                   highlight->getStartPosition()->getElementIndex(),
//                   highlight->getStartPosition()->getCharIndex());
            
        } else if (highlight->isSpeech()) {
            mode = Hull::Speech;
            color = highlight->getBackgroundColor();
            context->setFillColor(color, 128);

        } else {
            mode = Hull::Fill;
            color = highlight->getBackgroundColor();
            context->setFillColor(color, 255 * 0.35);
        }
        
        if (mode != Hull::None) {
            shared_ptr<Hull> hull = highlight->hull(*page);
            if (hull != 0) {
                hull->draw(getContext(), mode, color, noteCount);
            } else {
                printf("Hull is NULL\n");
            }
        }
    }
    
//		final ZLTextRegion outlinedElementRegion = getOutlinedRegion(page);
//		if (outlinedElementRegion != null && myShowOutline) {
//			context.setLineColor(getSelectionBackgroundColor());
//			outlinedElementRegion.hull().draw(context, Hull.DrawMode.Outline);
//		}
    
    drawSelectionCursor(context, *page, SelectionCursor::Left);
    drawSelectionCursor(context, *page, SelectionCursor::Right);
}

//void ZLTextView::paint(shared_ptr<ZLPaintContext> context,int value) {
//    /*final ZLFile wallpaper = getWallpaperFile();
//    if (wallpaper != null) {
//        context.clear(wallpaper, getFillMode());
//    } else {
//        context.clear(getBackgroundColor());
//    }*/
//    setContext(context);
//    context->clear(getBackgroundColor());
//    if(myModel == 0x0){
//        return;
//    }
//    shared_ptr<ZLTextPage> page;
//    switch (value) {
//        case ZLViewEnums_PageIndex::current:
//            page = myCurrentPage;
//            break;
//        case ZLViewEnums_PageIndex::next:
//            page = myNextPage;
//        printf("end, p:%d  e:%d, c:%d\n",myCurrentPage->EndCursor->getParagraphIndex(),
//               myCurrentPage->EndCursor->getElementIndex(),
//               myCurrentPage->EndCursor->getCharIndex());
//        if(page->PaintState == PaintStateEnum::NOTHING_TO_PAINT) {
//            preparePaintInfo(myCurrentPage);
//            myNextPage->StartCursor->setCursor(myCurrentPage->EndCursor);
//            myNextPage->PaintState = PaintStateEnum::START_IS_KNOWN;
//        }
//        break;
//        case ZLViewEnums_PageIndex::previous:
//            page=myPreviousPage;
//            if(page->PaintState == PaintStateEnum::NOTHING_TO_PAINT){
//                preparePaintInfo(myCurrentPage);
//                myPreviousPage->EndCursor->setCursor(myCurrentPage->StartCursor);
//                myPreviousPage->PaintState = PaintStateEnum::END_IS_KNOWN;
//                
//            }
//        default:
//            break;
//    }
//    page->TextElementMap->clear();
//    //page->moveStartCursor(10, 1, 1);
//    preparePaintInfo(page);
//    if (page->StartCursor == 0x0 || page->EndCursor == 0 || page->StartCursor->isNull() || page->EndCursor->isNull()) {
//        return;
//    }
//    std::vector<shared_ptr<ZLTextLineInfo>> lineInfos  = page->LineInfos;
//    
//    int* labels = new int[lineInfos.size() + 1];
//    labels[0] = 0;
//    int x = getLeftMargin();
//    int y = getTopMargin();
//    int index = 0;
//    int columnIndex = 0;
//    shared_ptr<ZLTextLineInfo> previousInfo = 0;
//    //for (ZLTextLineInfo info : lineInfos)
//    for(std::vector<shared_ptr<ZLTextLineInfo>>::const_iterator it =  lineInfos.begin();it != lineInfos.end();
//        ++it)
//    {
//        shared_ptr<ZLTextLineInfo> info = *it;
//        info->adjust(previousInfo);
//        prepareTextLine(page, info, x, y, columnIndex);
//        y += info->Height + info->Descent + info->VSpaceAfter;
////        printf("info->Height: %d, info->Descent: %d, info->VSpaceAfter: %d\n", info->Height, info->Descent, info->VSpaceAfter);
//        labels[++index] = page->TextElementMap->size();
//        if (index == page->Column0Height) {
//            y = getTopMargin();
//            x += page->getTextWidth() + getSpaceBetweenColumns();
//            columnIndex = 1;
//        }
//        previousInfo = info;
//    }
//   
//    std::vector<ZLTextHighlighting*> hilites = findHilites(*page);
//    
//    x = getLeftMargin();
//    y = getTopMargin();
//    index = 0;
//    //for (ZLTextLineInfo info : lineInfos)
//    for(std::vector<shared_ptr<ZLTextLineInfo>>::const_iterator it =  lineInfos.begin();it != lineInfos.end();
//        ++it)
//    {
//        shared_ptr<ZLTextLineInfo> info = *it;
//        drawTextLine(page, hilites, info, labels[index], labels[index + 1]);
//        y += info->Height + info->Descent + info->VSpaceAfter;
//        ++index;
//        if (index == page->Column0Height) {
//            y = getTopMargin();
//            x += page->getTextWidth() + getSpaceBetweenColumns();
//        }
//    }
//    
//    delete[] labels;
//    
//    for (int i = 0; i < hilites.size(); i++) {
//        int mode = Hull::None;
//        ZLTextHighlighting* hilit = hilites[i];
//        
//        shared_ptr<ZLColor> bgColor = hilit->getBackgroundColor();
//        if (bgColor != 0) {
//            context->setFillColor(bgColor, 128);
//            mode |= Hull::Fill;
//        }
//        
//        shared_ptr<ZLColor> outlineColor = hilit->getOutlineColor();
//        if (outlineColor != 0) {
//            context->setLineColor(outlineColor);
//            mode |= Hull::Outline;
//        }
//        
//        shared_ptr<ZLColor> underlineColor = hilit->getUnderlineColor();
//        if (underlineColor != 0) {
//            context->setLineColor(underlineColor);
//            mode |= Hull::Underline;
//        }
//        
//        int noteCount = hilit->getNoteCount();
//
//        if (mode != Hull::None) {
//            shared_ptr<Hull> xx = hilit->hull(*page);
//            if (xx != 0) {
//                 xx->draw(getContext(), mode, noteCount);
//            } else {
//                printf("HULL is null\n");
//            }
//        }
//    }
//    
//     shared_ptr<ZLTextRegion> outlinedElementRegion = getOutlinedRegion(*page);
//    if (outlinedElementRegion != 0 && myShowOutline) {
//        context->setLineColor(getSelectionBackgroundColor());
//        outlinedElementRegion->hull()->draw(context, Hull::Outline);
//    }
//    
//    drawSelectionCursor(context, *page, SelectionCursor::Left);
//    drawSelectionCursor(context, *page, SelectionCursor::Right);
//}

void ZLTextView::drawTextLine(shared_ptr<ZLTextPage> page,
                              std::vector<ZLTextHighlighting*> hilites,
                              shared_ptr<ZLTextLineInfo> info,
                              int from, int to) {
//    printf("drawTextLine from: %d, to: %d\n", from, to);
     shared_ptr<ZLPaintContext> context = getContext();
     shared_ptr<ZLTextParagraphCursor> paragraph = info->ParagraphCursor;
    int index = from;
    int endElementIndex = info->EndElementIndex;
    int charIndex = info->RealStartCharIndex;
    std::vector<shared_ptr<ZLTextElementArea>> pageAreas = page->TextElementMap->areas();
    if (to > pageAreas.size()) {
        return;
    }

    int opinionCount = 0;
    if (info->isEndOfParagraph()) {
        for (std::vector<ZLTextHighlighting*>::iterator it = hilites.begin(); it != hilites.end(); it++) {
            ZLTextHighlighting* hilit = *it;
            if (hilit->isOpinion() && hilit->getStartPosition()->getParagraphIndex() == paragraph->Index) {
                opinionCount = getOpinionCount(paragraph->Index);
                break;
            }
        }
    }

//    printf("endElementIndex: %d, to: %d\n", endElementIndex, to);
    for (int wordIndex = info->RealStartElementIndex; wordIndex != endElementIndex && index < to; ++wordIndex, charIndex = 0) {
//        printf("wordIndex: %d, index: %d\n", wordIndex, index);
        shared_ptr<ZLTextElement> element = paragraph->getElement(wordIndex);
        shared_ptr<ZLTextElementArea> area = pageAreas[index];
        if (element == area->Element) {
            ++index;
            if (area->ChangeStyle) {
                setTextStyle(area->Style);
            }
            int areaX = area->XStart;
            int areaY = area->YEnd - getElementDescent(*element) - getTextStyle()->getVerticalAlign(metrics());
//            printf("area: (%d, %d)\n", areaX, areaY);
            if (element->kind() == ZLTextElement::WORD_ELEMENT) {
                shared_ptr<ZLTextPosition> pos = new ZLTextFixedPosition(info->ParagraphCursor->Index, wordIndex, 0);
                
                ZLTextHighlighting* hl = getWordHilite(pos, hilites);
                shared_ptr<ZLColor> hlColor = hl != 0 ? hl->getForegroundColor() : 0;
                shared_ptr<ZLColor> textColor = hlColor != 0 ? hlColor : getTextColor(getTextStyle()->Hyperlink);
                drawWord(areaX, areaY, (ZLTextWord&)(*element), charIndex, -1, false, textColor);

                if (opinionCount > 0) {
                    if (index == to) {
                        int width = getElementWidth(*element, charIndex);
                        int height = getElementHeight(*element);
//                        printf("element width: %d, height: %d, opinion count: %d\n", width, height, opinionCount);
                        context->drawOpinion(areaX + width, areaY, height, paragraph->Index, opinionCount);
                    }
                }
            
            } else if (element->kind() == ZLTextElement::IMAGE_ELEMENT) {
                //shared_ptr<ZLTextImageElement> imageElement = (ZLTextImageElement*)element.getT();
                context->drawImage(areaX, areaY,
                                   *(((const ZLTextImageElement&)(*element)).ImageData),
                                   getTextAreaSize(),
                                   getScalingType((const ZLTextImageElement&)(*element)),
                                   getAdjustingModeForImages());

            } else if (element->kind() == ZLTextElement::VIDEO_ELEMENT) {
                // TODO: draw video
//                context.setLineColor(getTextColor(ZLTextHyperlink.NO_LINK));
//                context.setFillColor(new ZLColor(127, 127, 127));
//                final int xStart = area.XStart + 10;
//                final int xEnd = area.XEnd - 10;
//                final int yStart = area.YStart + 10;
//                final int yEnd = area.YEnd - 10;
//                context.fillRectangle(xStart, yStart, xEnd, yEnd);
//                context.drawLine(xStart, yStart, xStart, yEnd);
//                context.drawLine(xStart, yEnd, xEnd, yEnd);
//                context.drawLine(xEnd, yEnd, xEnd, yStart);
//                context.drawLine(xEnd, yStart, xStart, yStart);
//                final int l = xStart + (xEnd - xStart) * 7 / 16;
//                final int r = xStart + (xEnd - xStart) * 10 / 16;
//                final int t = yStart + (yEnd - yStart) * 2 / 6;
//                final int b = yStart + (yEnd - yStart) * 4 / 6;
//                final int c = yStart + (yEnd - yStart) / 2;
//                context.setFillColor(new ZLColor(196, 196, 196));
//                context.fillPolygon(new int[] { l, l, r }, new int[] { t, b, c });

            } else if (element->kind() == ZLTextElement::EXTENSION_ELEMENT) {
                (((const ExtensionElement&)*element)).draw(context, area);
                
            } else if (element == ZLTextElement::HSpace || element == ZLTextElement::NBSpace) {
                 int cw = context->getSpaceWidth();
                for (int len = 0; len < area->XEnd - area->XStart; len += cw) {
                    context->drawString(areaX + len, areaY, SPACE, 0, 1);
                }
            }
        }
    }

//    printf("finish drawTextLine\n");
}

 shared_ptr<ZLTextWordCursor>
ZLTextView::findStart(shared_ptr<ZLTextPage> page, shared_ptr<ZLTextWordCursor> end, int unit, int height) {
    shared_ptr<ZLTextWordCursor> start = new ZLTextWordCursor(end);
    shared_ptr<ParagraphSize> size = paragraphSize(page, start, true, unit);
    
    height -= size->Height;
    bool positionChanged = !start->isStartOfParagraph();
    
    start->moveToParagraphStart();
    while (height > 0) {
        shared_ptr<ParagraphSize> previousSize = size;
        if (positionChanged && start->getParagraphCursor()->isEndOfSection()) {
            break;
        }
        if (!start->previousParagraph()) {
            break;
        }
        if (!start->getParagraphCursor()->isEndOfSection()) {
            positionChanged = true;
        }
        size = paragraphSize(page, start, false, unit);
        height -= size->Height;
        if (previousSize != 0) {
            height += std::min(size->BottomMargin, previousSize->TopMargin);
        }
    }
    skip(page, start, unit, -height);
    
    if (unit == PIXEL_UNIT) {
        bool sameStart = start->samePositionAs(*end);
        if (!sameStart && start->isEndOfParagraph() && end->isStartOfParagraph()) {
            shared_ptr<ZLTextWordCursor> startCopy = new ZLTextWordCursor(start);
            startCopy->nextParagraph();
            sameStart = startCopy->samePositionAs(*end);
        }
        if (sameStart) {
            start->setCursor(findStart(page, end, LINE_UNIT, 1));
        }
    }
    
    return start;
}
 void
ZLTextView::skip(shared_ptr<ZLTextPage> page, shared_ptr<ZLTextWordCursor >cursor, int unit, int size) {
     shared_ptr<ZLTextParagraphCursor> paragraphCursor = cursor->getParagraphCursor();
    if (paragraphCursor == 0x0) {
        return;
    }
    int endElementIndex = paragraphCursor->getParagraphLength();
    
    resetTextStyle();
    applyStyleChanges(paragraphCursor, 0, cursor->getElementIndex());
    
    shared_ptr<ZLTextLineInfo> info = 0;
    while (!cursor->isEndOfParagraph() && size > 0) {
        info = processTextLine(page, paragraphCursor, cursor->getElementIndex(), cursor->getCharIndex(), endElementIndex, info);
        cursor->moveTo(info->EndElementIndex, info->EndCharIndex);
        size -= infoSize(info, unit);
    }
}
shared_ptr<ParagraphSize>
ZLTextView::paragraphSize(
                          shared_ptr<ZLTextPage> page,
                          shared_ptr<ZLTextWordCursor> cursor,
                          bool beforeCurrentPosition,
                          int unit) {
     shared_ptr<ParagraphSize> size = new ParagraphSize();
    
     shared_ptr<ZLTextParagraphCursor> paragraphCursor = cursor->getParagraphCursor();
    if (paragraphCursor == 0) {
        return size;
    }
    int endElementIndex =
    beforeCurrentPosition ? cursor->getElementIndex() : paragraphCursor->getParagraphLength();
    
    resetTextStyle();
    
    int wordIndex = 0;
    int charIndex = 0;
    shared_ptr<ZLTextLineInfo> info = 0;
    while (wordIndex != endElementIndex) {
         shared_ptr<ZLTextLineInfo> prev = info;
        info = processTextLine(page, paragraphCursor, wordIndex, charIndex, endElementIndex, prev);
        wordIndex = info->EndElementIndex;
        charIndex = info->EndCharIndex;
        size->Height += infoSize(info, unit);
        if (prev == 0) {
            size->TopMargin = info->VSpaceBefore;
        }
        size->BottomMargin = info->VSpaceAfter;
    }
    
    return size;
}
int
ZLTextView::infoSize(shared_ptr<ZLTextLineInfo> info, int unit) {
    return (unit == PIXEL_UNIT) ? (info->Height + info->Descent + info->VSpaceAfter) : (info->IsVisible ? 1 : 0);
}

#if 1
void ZLTextView::prepareTextLine(shared_ptr<ZLTextPage> page, shared_ptr<ZLTextLineInfo> info, int x, int y, int columnIndex)
{
    y = std::min(y + info->Height, getTopMargin() + page->getTextHeight() - 1);
    
    shared_ptr<ZLPaintContext> context = getContext();
    shared_ptr<ZLTextParagraphCursor> paragraphCursor = info->ParagraphCursor;
    
    setTextStyle(info->StartStyle);
    int spaceCounter = info->SpaceCounter;
    int fullCorrection = 0;
    bool endOfParagraph = info->isEndOfParagraph();
    bool wordOccurred = false;
    bool changeStyle = true;
    x += info->LeftIndent;
    
    int maxWidth = page->getTextWidth();
    switch (getTextStyle()->getAlignment()) {
        case ZLTextAlignmentType::ALIGN_RIGHT:
        x += maxWidth - getTextStyle()->getRightIndent(metrics()) - info->Width;
        break;
        case ZLTextAlignmentType::ALIGN_CENTER:
        x += (maxWidth - getTextStyle()->getRightIndent(metrics()) - info->Width) / 2;
        break;
        case ZLTextAlignmentType::ALIGN_JUSTIFY:
        if (!endOfParagraph && (paragraphCursor->getElement(info->EndElementIndex) != ZLTextElement::AfterParagraph)) {
            fullCorrection = maxWidth - getTextStyle()->getRightIndent(metrics()) - info->Width;
        }
        break;
        case ZLTextAlignmentType::ALIGN_LEFT:
        case ZLTextAlignmentType::ALIGN_UNDEFINED:
        break;
    }
    
    
    shared_ptr<ZLTextParagraphCursor> paragraph = info->ParagraphCursor;
    int paragraphIndex = paragraph->Index;
    int endElementIndex = info->EndElementIndex;
    int charIndex = info->RealStartCharIndex;
    shared_ptr<ZLTextElementArea> spaceElement = 0;
    
    for (int wordIndex = info->RealStartElementIndex; wordIndex != endElementIndex; ++wordIndex, charIndex = 0) {
        shared_ptr<ZLTextElement> element = paragraph->getElement(wordIndex);
       // uint8_t kind = element->kind();
         int width = getElementWidth(*element, charIndex);
        
        if (element->kind() == ZLTextElement::HSPACE_ELEMENT) {
            if (wordOccurred && spaceCounter > 0) {
                 int correction = fullCorrection / spaceCounter;
                 int spaceLength = context->getSpaceWidth() + correction;
                if (getTextStyle()->isUnderline()) {
                    spaceElement = new ZLTextElementArea(paragraphIndex, wordIndex, 0,
                                                         0, // length
                                                         true, // is last in element
                                                         false, // add hyphenation sign
                                                         false, // changed style
                                                         getTextStyle(), element, x, x + spaceLength, y, y, columnIndex);
                } else {
                    spaceElement = 0;
                }
                x += spaceLength;
                fullCorrection -= correction;
                wordOccurred = false;
                --spaceCounter;
            }
        } else if (element->kind() ==ZLTextElement::WORD_ELEMENT
                   || element->kind() == ZLTextElement::IMAGE_ELEMENT||
                   element->kind() == ZLTextElement::VIDEO_ELEMENT
                   || element->kind() == ZLTextElement::EXTENSION_ELEMENT) {
            int height = getElementHeight(*element);
            int descent = getElementDescent(*element);
            int length = element->kind() == ZLTextElement::WORD_ELEMENT ? ((ZLTextWord&)*element).Length : 0;
            if (spaceElement != 0) {
                page->TextElementMap->add(spaceElement);
                spaceElement = 0;
            }
            page->TextElementMap->add(new ZLTextElementArea(paragraphIndex, wordIndex, charIndex,
                                                            length - charIndex,
                                                            true, // is last in element
                                                            false, // add hyphenation sign
                                                            changeStyle, getTextStyle(), element,
                                                            x, x + width - 1, y - height + 1, y + descent, columnIndex));
            changeStyle = false;
            wordOccurred = true;
        } else if (isStyleChangeElement(element)) {
            applyStyleChangeElement(element);
            changeStyle = true;
        }
        x += width;
    }
    
    if (!endOfParagraph) {
         int len = info->EndCharIndex;
        if (len > 0) {
             int wordIndex = info->EndElementIndex;
            // ZLTextWord& word = (ZLTextWord& )*paragraph->getElement(wordIndex);
            shared_ptr<ZLTextElement>  word = paragraph->getElement(wordIndex);
            bool addHyphenationSign = ((ZLTextWord&)*word).Data[ ((ZLTextWord&)*word).Offset + len - 1] != '-';
            int width = getWordWidth(((ZLTextWord&)*word), 0, len, addHyphenationSign);
            int height = getElementHeight(((ZLTextWord&)*word));
            int descent = context->getDescent();
            //ZLTextElementArea(int paragraphIndex, int elementIndex, int charIndex, int length, bool lastInElement, bool addHyphenationSign, bool changeStyle, shared_ptr<ZLTextStyle> style, shared_ptr<ZLTextElement> element, int xStart, int xEnd, int yStart, int yEnd, int columnIndex)
            page->TextElementMap->add(new ZLTextElementArea(paragraphIndex, wordIndex, 0,
                                                            len,
                                                            false, // is last in element
                                                            addHyphenationSign,
                                                            changeStyle, getTextStyle(), word,
                                                            x, x + width - 1, y - height + 1, y + descent, columnIndex));
        }
    }
    
    
}

shared_ptr<ZLTextHighlighting> ZLTextView::findHighlighting(int x, int y, int maxDistance) {
//    printf("max selection distance: %d\n", maxDistance + 16);
    shared_ptr<ZLTextRegion> region = findRegion(x, y, maxDistance + 16, *ZLTextRegion::anyFiler);
    if (region == 0) {
        return 0;
    }

    shared_ptr<ZLTextRegion::Soul> soul = region->getSoul();
//    printf("region: p:%d, eStart:%d, eEnd:%d\n", soul->ParagraphIndex, soul->StartElementIndex, soul->EndElementIndex);

    for (int i = 0; i < myHighlightings.size(); i++) {
        shared_ptr<ZLTextHighlighting> h = myHighlightings[i];
        if (h->isOpinion()) {
            shared_ptr<ZLTextPosition> startPos = h->getStartPosition();
            shared_ptr<ZLTextPosition> endPos = h->getEndPosition();
//            printf("opinion: p:%d, eStart:%d, cStart:%d, eEnd: %d, cEnd: %d\n",
//                   startPos->getParagraphIndex(), startPos->getElementIndex(), startPos->getCharIndex(),
//                   endPos->getElementIndex(), endPos->getCharIndex());
            if (startPos->getParagraphIndex() == soul->ParagraphIndex
                && startPos->getElementIndex() <= soul->EndElementIndex + 4
                && startPos->getElementIndex() >= soul->StartElementIndex) {
                return h;
            }
        } else {
            if (h->getBackgroundColor() != 0 && h->intersects(*region)) {
                return h;
            }
        }
    }
    return 0;
}

shared_ptr<ZLTextRegion>
ZLTextView::findRegion(int x, int y, ZLTextRegion::Filter& filter) {
    
    return findRegion(x, y, std::numeric_limits<int>::max() - 1, filter);
}

shared_ptr<ZLTextRegion>
ZLTextView::findRegion(int x, int y, int maxDistance, ZLTextRegion::Filter& filter) {
    return myCurrentPage->TextElementMap->findRegion(x, y, maxDistance, filter);
}

shared_ptr<ZLTextElementAreaVector::RegionPair>
ZLTextView::findRegionsPair(int x, int y, ZLTextRegion::Filter& filter) {
    return myCurrentPage->TextElementMap->findRegionsPair(x, y, getColumnIndex(x), filter);
}

bool
ZLTextView::initSelection(int x, int y) {
    y -= ((ZLTextBaseStyle& )*(getTextStyleCollection()->getBaseStyle())).getFontSize() / 2;
    if (!mySelection->start(x, y)) {
        return false;
    }
    Application->getViewWidget()->reset();
    Application->getViewWidget()->repaint();
    return true;
}

SelectionCursor::Which
ZLTextView::findSelectionCursor(int x, int y, float maxDistance2) {
    if (mySelection->isEmpty()) {
        return SelectionCursor::None;
    }
    
    float leftDistance2 = distance2ToCursor(x, y, SelectionCursor::Left);
    float rightDistance2 = distance2ToCursor(x, y, SelectionCursor::Right);
    
    if (rightDistance2 < leftDistance2) {
        return rightDistance2 <= maxDistance2 ? SelectionCursor::Right : SelectionCursor::None;
    } else {
        return leftDistance2 <= maxDistance2 ? SelectionCursor::Left : SelectionCursor::None;
    }
}
SelectionCursor::Which
ZLTextView::findSelectionCursor(int x, int y) {
    return findSelectionCursor(x, y, std::numeric_limits<float>::max());
}
float
ZLTextView::distance2ToCursor(int x, int y, SelectionCursor::Which which) {
    shared_ptr<ZLTextSelection::Point> point = getSelectionCursorPoint(*myCurrentPage, which);
    if (point == 0) {
        return std::numeric_limits<float>::max();
    }
    float dX = x - point->X;
    float dY = y - point->Y;
    return dX * dX + dY * dY;
}
void
ZLTextView::moveSelectionCursorTo(SelectionCursor::Which which, int x, int y) {
    y -= ((ZLTextBaseStyle& )*getTextStyleCollection()->getBaseStyle()).getFontSize() / 2;
    
    mySelection->setCursorInMovement(which, x, y);
    mySelection->expandTo(*myCurrentPage, x, y);
    Application->getViewWidget()->reset();
    Application->getViewWidget()->repaint();
}

void
ZLTextView::turnPage(bool forward, int scrollingMode, int value) {
    preparePaintInfo(myCurrentPage);
    myPreviousPage->reset();
    myNextPage->reset();
    if (myCurrentPage->PaintState == READY) {
        myCurrentPage->PaintState = forward ? TO_SCROLL_FORWARD :TO_SCROLL_BACKWARD;
        myScrollingMode = scrollingMode;
        myOverlappingValue = value;
    }
}
void
ZLTextView::preparePaintInfo() {
    myPreviousPage->reset();
    myNextPage->reset();
    preparePaintInfo(myCurrentPage);
}

void ZLTextView::releaseSelectionCursor() {
    mySelection->stop();
    Application->getViewWidget()->reset();
    Application->getViewWidget()->repaint();
}

void ZLTextView::clearSelection() {
    if (mySelection != 0 && mySelection->clear()) {
        Application->getViewWidget()->reset();
        Application->getViewWidget()->repaint();
    }
}

int
ZLTextView::getSelectionStartY() {
    if (mySelection->isEmpty()) {
        return 0;
    }
    shared_ptr<ZLTextElementArea> selectionStartArea = mySelection->getStartArea(*myCurrentPage);
    if (selectionStartArea != 0) {
        return selectionStartArea->YStart;
    }
    if (mySelection->hasPartBeforePage(*myCurrentPage)) {
        shared_ptr<ZLTextElementArea> firstArea = myCurrentPage->TextElementMap->getFirstArea();
        return firstArea != 0 ? firstArea->YStart : 0;
    } else {
        shared_ptr<ZLTextElementArea> lastArea = myCurrentPage->TextElementMap->getLastArea();
        return lastArea != 0 ? lastArea->YEnd : 0;
    }
}
int
ZLTextView::getSelectionStartX()
{
    shared_ptr<ZLTextElementArea> selectionStartArea = mySelection->getStartArea(*myCurrentPage);
    if (selectionStartArea != 0) {
        return selectionStartArea->XStart;
    }
    if (mySelection->hasPartBeforePage(*myCurrentPage)) {
        shared_ptr<ZLTextElementArea> firstArea = myCurrentPage->TextElementMap->getFirstArea();
        return firstArea != 0 ? firstArea->XStart : 0;
    } else {
        shared_ptr<ZLTextElementArea> lastArea = myCurrentPage->TextElementMap->getLastArea();
        return lastArea != 0 ? lastArea->XEnd : 0;
    }
}

int
ZLTextView::getSelectionEndX()
{
    if (mySelection->isEmpty()) {
        return 0;
    }
    shared_ptr<ZLTextElementArea> selectionEndArea = mySelection->getEndArea(*myCurrentPage);
    if (selectionEndArea != 0) {
        return selectionEndArea->XEnd;
    }
    if (mySelection->hasPartAfterPage(*myCurrentPage)) {
        shared_ptr<ZLTextElementArea> lastArea = myCurrentPage->TextElementMap->getLastArea();
        return lastArea != 0 ? lastArea->XEnd : 0;
    } else {
        shared_ptr<ZLTextElementArea> firstArea = myCurrentPage->TextElementMap->getFirstArea();
        return firstArea != 0 ? firstArea->XStart : 0;
    }
}
int
ZLTextView::getSelectionEndY() {
    if (mySelection->isEmpty()) {
        return 0;
    }
    shared_ptr<ZLTextElementArea> selectionEndArea = mySelection->getEndArea(*myCurrentPage);
    if (selectionEndArea != 0) {
        return selectionEndArea->YEnd;
    }
    if (mySelection->hasPartAfterPage(*myCurrentPage)) {
        shared_ptr<ZLTextElementArea> lastArea = myCurrentPage->TextElementMap->getLastArea();
        return lastArea != 0 ? lastArea->YEnd : 0;
    } else {
        shared_ptr<ZLTextElementArea> firstArea = myCurrentPage->TextElementMap->getFirstArea();
        return firstArea != 0 ? firstArea->YStart : 0;
    }
}
bool ZLTextView::isPressedOnSelectedRegion(int x,int y)
{
    if(mySelection == 0) return false;
    if (mySelection->isEmpty()) {
        return false;
    }
    return mySelection->checkIn(*myCurrentPage,x,y);
    
}
float ZLTextView::computeCharWidth() {
    if (myLettersModel != myModel) {
        myLettersModel = myModel;
        myLettersBufferLength = 0;
        myCharWidth = -1.0f;
        
        int paragraph = 0;
        int textSize = myModel->getTextLength(myModel->getParagraphsNumber() - 1);
        if (textSize > /*myLettersBuffer.length*/myLettersBufferStoredLength) {
            paragraph = myModel->findParagraphByTextLength((textSize - myLettersBufferStoredLength/*myLettersBuffer.length*/) / 2);
        }
        while (paragraph < myModel->getParagraphsNumber()
               && myLettersBufferLength < myLettersBufferStoredLength/*myLettersBuffer.length*/) {
            
            shared_ptr<ZDZLTextParagraph::EntryIterator> it = myModel->getParagraph(paragraph++)->iterator();
            
            while (myLettersBufferLength < myLettersBufferStoredLength/*myLettersBuffer.length*/ && it->next()) {
                if (((ZDZLTextParagraph::Entry)it->getType()) == ZDZLTextParagraph::TEXT) {
                    int len =std::min(it->getTextLength(),
                                             /*myLettersBuffer.length*/ - myLettersBufferLength);
                   // System.arraycopy(it.getTextData(), it.getTextOffset(),
                             //        myLettersBuffer, myLettersBufferLength, len);
                    
                    IOSUtil::arraycopy(it->getTextData(), it->getTextOffset(), myLettersBuffer, myLettersBufferLength, len);
                    
                    myLettersBufferLength += len;
                }
            }
        }
        
        if (myLettersBufferLength == 0) {
            short ourDefaultLetters[]={'H','E'};
            //myLettersBufferLength = std::min(myLettersBuffer.length, ourDefaultLetters.length);
            myLettersBufferLength = std::min(512, 2);
            //System.arraycopy(ourDefaultLetters, 0, myLettersBuffer, 0, myLettersBufferLength);
            IOSUtil::arraycopy(ourDefaultLetters,0, myLettersBuffer, 0, myLettersBufferLength);
        }
    }
    
    if (myCharWidth < 0.0f) {
        myCharWidth = computeCharWidth(myLettersBuffer, myLettersBufferLength);
    }
    return myCharWidth;
}
float ZLTextView::computeCharsPerPage()
{
    setTextStyle(getTextStyleCollection()->getBaseStyle());
    
    int textWidth = getTextColumnWidth();
    int textHeight = getTextAreaHeight();
    
    int num = myModel->getParagraphsNumber();
    int totalTextSize = myModel->getTextLength(num - 1);
    float charsPerParagraph = ((float)totalTextSize) / num;
    
    float charWidth = computeCharWidth();
    
    int indentWidth = getElementWidth(*ZLTextElement::Indent, 0);
    float effectiveWidth = textWidth - (indentWidth + 0.5f * textWidth) / charsPerParagraph;
    float charsPerLine = std::min(effectiveWidth / charWidth,
                                  charsPerParagraph * 1.2f);
    
    int strHeight = getWordHeight() + getContext()->getDescent();
    int effectiveHeight = (int)
    (textHeight -
     (getTextStyle()->getSpaceBefore(metrics())
      + getTextStyle()->getSpaceAfter(metrics()) / 2) / charsPerParagraph);
     int linesPerPage = effectiveHeight / strHeight;
    
    return charsPerLine * linesPerPage;
}
int ZLTextView::sizeOfTextBeforeCursor(shared_ptr<ZLTextWordCursor> wordCursor)
{
    shared_ptr<ZLTextParagraphCursor> paragraphCursor = wordCursor->getParagraphCursor();
    if (paragraphCursor == 0) {
        return -1;
    }
     int paragraphIndex = paragraphCursor->Index;
    int sizeOfText = myModel->getTextLength(paragraphIndex - 1);
    int paragraphLength = paragraphCursor->getParagraphLength();
    if (paragraphLength > 0) {
        sizeOfText +=
        (myModel->getTextLength(paragraphIndex) - sizeOfText)
        * wordCursor->getElementIndex()
        / paragraphLength;
    }
    return sizeOfText;
}
shared_ptr<PagePosition> ZLTextView::pagePosition()
{
    int current = computeTextPageNumber(getCurrentCharNumber(ZLViewEnums_PageIndex::current, false));
    int total = computeTextPageNumber(sizeOfFullText());
    
    if (total > 3) {
        return new PagePosition(current, total);
    }
    preparePaintInfo(myCurrentPage);
    shared_ptr<ZLTextWordCursor> cursor = myCurrentPage->StartCursor;
    if (cursor == 0 || cursor->isNull()) {
        return new PagePosition(current, total);
    }
    
    if (cursor->isStartOfText()) {
        current = 1;
    } else {
        shared_ptr<ZLTextWordCursor> prevCursor = myPreviousPage->StartCursor;
        if (prevCursor == 0|| prevCursor->isNull()) {
            preparePaintInfo(myPreviousPage);
            prevCursor = myPreviousPage->StartCursor;
        }
        if (prevCursor != 0 && !prevCursor->isNull()) {
            current = prevCursor->isStartOfText() ? 2 : 3;
        }
    }
    
    total = current;
    cursor = myCurrentPage->EndCursor;
    if (cursor == 0 || cursor->isNull()) {
        return new PagePosition(current, total);
    }
    if (!cursor->isEndOfText()) {
        shared_ptr<ZLTextWordCursor> nextCursor = myNextPage->EndCursor;
        if (nextCursor == 0 || nextCursor->isNull()) {
            preparePaintInfo(myNextPage);
            nextCursor = myNextPage->EndCursor;
        }
        if (nextCursor != 0) {
            total += nextCursor->isEndOfText() ? 1 : 2;
        }
    }
    
    return new PagePosition(current, total);
}

void ZLTextView::gotoPagePosition(int page) {
    printf("goto page: %d\n", page);
    if (myModel == 0x0 || myModel->getParagraphsNumber() == 0) {
        return;
    }
    
    float factor = computeCharsPerPage();
    float textSize = page * factor;
    
    int intTextSize = (int)textSize;
    int paragraphIndex = myModel->findParagraphByTextLength(intTextSize);
    
    if (paragraphIndex > 0 && myModel->getTextLength(paragraphIndex) > intTextSize) {
        --paragraphIndex;
    }
    intTextSize = myModel->getTextLength(paragraphIndex);
    
    int sizeOfTextBefore = myModel->getTextLength(paragraphIndex - 1);
    while (paragraphIndex > 0 && intTextSize == sizeOfTextBefore) {
        --paragraphIndex;
        intTextSize = sizeOfTextBefore;
        sizeOfTextBefore = myModel->getTextLength(paragraphIndex - 1);
    }
    
    int paragraphLength = intTextSize - sizeOfTextBefore;
    
    int wordIndex;
    if (paragraphLength == 0) {
        wordIndex = 0;
    } else {
        preparePaintInfo(myCurrentPage);
        shared_ptr<ZLTextWordCursor> cursor = new ZLTextWordCursor(myCurrentPage->EndCursor);
        cursor->moveToParagraph(paragraphIndex);
        wordIndex = cursor->getParagraphCursor()->getParagraphLength();
    }
    
    gotoPositionByEnd(paragraphIndex, wordIndex, 0);
}

void ZLTextView::gotoPositionByEnd(int paragraphIndex, int wordIndex, int charIndex) {
    printf("goto position by end: p:%d, e:%d, c:%d\n", paragraphIndex, wordIndex, charIndex);
    if (myModel != 0x0 && myModel->getParagraphsNumber() > 0) {
        myCurrentPage->moveEndCursor(paragraphIndex, wordIndex, charIndex);
        myPreviousPage->reset();
        myNextPage->reset();
        preparePaintInfo(myCurrentPage);
        if (myCurrentPage->isEmptyPage()) {
            turnPage(false, NO_OVERLAPPING, 0);
        }
    }
}

int ZLTextView::getCurrentCharNumber(int pageIndex, bool startNotEndOfPage)
{
    if (myModel == 0 || myModel->getParagraphsNumber() == 0) {
        return 0;
    }
    shared_ptr<ZLTextPage> page = getPage(pageIndex);
    preparePaintInfo(page);
    if (startNotEndOfPage) {
        return std::max(0, sizeOfTextBeforeCursor(page->StartCursor));
    } else {
        int end = sizeOfTextBeforeCursor(page->EndCursor);
        if (end == -1) {
            end = myModel->getTextLength(myModel->getParagraphsNumber() - 1) - 1;
        }
        return std::max(1, end);
    }
    return  0;
}

#endif

#pragma mark - opinion
int ZLTextView::getOpinionCount(int pIndex) {
    return 0;
}

#pragma mark - search
int ZLTextView::searchText(std::string text, long length, bool ignoreCase, bool wholeText, bool backward,
                                                    bool thisSectionOnly) {
    if (myModel == NULL || text.length() == 0) {
        return 0;
    }
    int startIndex = 0;
    int endIndex = myModel->getParagraphsNumber();
    if (thisSectionOnly) {
        // TODO: implement
    }
    int count = myModel->search(text, length, startIndex, endIndex, ignoreCase);
    printf("search text count: %d\n", count);
    
//    myPreviousPage->reset();
//    myNextPage->reset();
//    if (!myCurrentPage->StartCursor.isNull()) {
//        rebuildPaintInfo();
//        if (count > 0) {
//            shared_ptr<ZDZLTextMark> mark = myCurrentPage->StartCursor->getMark();
//            gotoMark(wholeText ?
//                     (backward ? myModel->getLastMark() : myModel->getFirstMark()) :
//                     (backward ? myModel->getPreviousMark(mark) : myModel->getNextMark(mark)));
//        }
//        Application->getViewWidget()->reset();
//        Application->getViewWidget()->repaint();
//    }

    return count;
}

std::vector<shared_ptr<ZDZLTextMark>> ZLTextView::getMarks() {
    return myModel->getMarks();
}

void ZLTextView::gotoMark(shared_ptr<ZDZLTextMark> mark) {
    if (mark == NULL) {
        return;
    }
    
    myPreviousPage->reset();
    myNextPage->reset();
    bool doRepaint = false;
    if (myCurrentPage->StartCursor->isNull()) {
        doRepaint = true;
        preparePaintInfo(myCurrentPage);
    }
    if (myCurrentPage->StartCursor->isNull()) {
        return;
    }
    if (myCurrentPage->StartCursor->getParagraphIndex() != mark->ParagraphIndex
        || myCurrentPage->StartCursor->getMark()->compareTo(*mark) > 0) {
        doRepaint = true;
        gotoPosition(mark->ParagraphIndex, 0, 0);
        preparePaintInfo(myCurrentPage);
    }
    if (myCurrentPage->EndCursor.isNull()) {
        preparePaintInfo(myCurrentPage);
    }
    while (mark->compareTo(*myCurrentPage->EndCursor->getMark()) > 0) {
        doRepaint = true;
        turnPage(true, NO_OVERLAPPING, 0);
        preparePaintInfo(myCurrentPage);
    }
    if (doRepaint) {
        if (myCurrentPage->StartCursor->isNull()) {
            preparePaintInfo(myCurrentPage);
        }
        Application->getViewWidget()->reset();
        Application->getViewWidget()->repaint();
    }
}

#pragma mark - calculate page count
int ZLTextView::calculatePageCount() {
    ZLTextPage* calculatePage = new ZLTextPage();
    calculatePage->moveStartCursor(myCursorManager->get(0));

    int pageCount = 0;
    while (!calculatePage->StartCursor->isEndOfText()) {
        shared_ptr<ZLTextWordCursor> endCursor = new ZLTextWordCursor();
        buildInfos(calculatePage, calculatePage->StartCursor, endCursor);
        if (!calculatePage->isEmptyPage()
            && (myScrollingMode != KEEP_LINES || !endCursor->samePositionAs(*calculatePage->EndCursor))) {
            calculatePage->EndCursor->setCursor(endCursor);
        }
        
        calculatePage->StartCursor->setCursor(endCursor);
        pageCount++;
    }
    printf("page count: %d\n", pageCount);
    return pageCount;
}

#pragma mark - prepare paint info
// 准备绘制需要的信息
void ZLTextView::preparePaintInfo(shared_ptr<ZLTextPage> page) {
    // 设置页面大小
    page->setSize(getTextColumnWidth(), getTextAreaHeight(), twoColumnView(), page == myPreviousPage);
    
    // 检查页面刷新状态
    if (page->PaintState == NOTHING_TO_PAINT || page->PaintState == READY) {
        return;
    }
    // 记录刷新状态
    int oldState = page->PaintState;
    
//    HashMap<ZLTextLineInfo,ZLTextLineInfo> cache = myLineInfoCache;
//    for (ZLTextLineInfo info : page.LineInfos) {
//        cache.put(info, info);
//    }
    // 获取页面的行信息数组
    std::vector<shared_ptr<ZLTextLineInfo>> LineInfos = page->LineInfos;
//    for (std::list < shared_ptr<ZLTextLineInfo> > ::iterator it = LineInfos.begin(); it != LineInfos.end(); ++ it) {
//        shared_ptr<ZLTextLineInfo> item = *it;
//        myLineInfoCache.put
//        myLineInfoCache.insert(std::make_pair(item,item));
//    }
//    for(int i=0;i<page->LineInfos.size();i++) {
//        myLineInfoCache.insert(std::make_pair(page->LineInfos[i],page->LineInfos[i]));
//    }
    // 将行信息的首尾添加到当前视图的缓存中
    myLineInfoCache.insert(page->LineInfos.begin(),page->LineInfos.end());

    switch (page->PaintState) {
        case TO_SCROLL_FORWARD:
            // 向前翻页
            if (!page->EndCursor->isEndOfText()) {
                // 当前页的结束游标不是全文末尾
                // 新建当前页的开始游标
                shared_ptr<ZLTextWordCursor> startCursor = new ZLTextWordCursor();
                // 根据翻页方式移动游标（功能代码已注释）
                switch (myScrollingMode) {
                    case NO_OVERLAPPING:
                        break;
                    case KEEP_LINES:
                        page->findLineFromEnd(startCursor, myOverlappingValue);
                        break;
                    case SCROLL_LINES:
                        page->findLineFromStart(startCursor, myOverlappingValue);
                        if (startCursor->isEndOfParagraph()) {
                            startCursor->nextParagraph();
                        }
                        break;
                    case SCROLL_PERCENTAGE:
                        page->findPercentFromStart(startCursor, myOverlappingValue);
                        break;
                }
                
                if (!startCursor->isNull() && startCursor->samePositionAs(((const ZLTextPosition&)(*page->StartCursor)))) {
                    // 功能代码已注释
                    page->findLineFromStart(startCursor, 1);
                }
                
                if (!startCursor->isNull()) {
                    // 新建结束游标
                    shared_ptr<ZLTextWordCursor> endCursor = new ZLTextWordCursor();
                    // IMPORTANT: 根据开始和结束游标创建页面信息
                    buildInfos(page, startCursor, endCursor);
                    if (!page->isEmptyPage() && (myScrollingMode != KEEP_LINES || !endCursor->samePositionAs(*page->EndCursor))) {
                        // 页面信息创建成功，设置页面开始和结束游标后，退出枚举
                        page->StartCursor->setCursor(startCursor);
                        page->EndCursor->setCursor(endCursor);
                        break;
                    }
                }
                
                // 开始游标为空/页面创建失败，将页面开始游标设置为结束游标，重新创建页面信息
                page->StartCursor->setCursor(page->EndCursor);
                buildInfos(page, page->StartCursor, page->EndCursor);
            }
            break;
            
        case TO_SCROLL_BACKWARD:
            // 向后翻页
            if (!page->StartCursor->isStartOfText()) {
                // 当前页的开始游标不是全文开头
                // 根据翻页方式移动游标
                switch (myScrollingMode) {
                    case NO_OVERLAPPING:
                        page->StartCursor->setCursor(findStartOfPrevousPage(page, page->StartCursor));
                        break;
                    case KEEP_LINES:
                    {
                        shared_ptr<ZLTextWordCursor> endCursor = new ZLTextWordCursor();
                        page->findLineFromStart(endCursor, myOverlappingValue);
                        if (!endCursor->isNull() && endCursor->samePositionAs(*page->EndCursor)) {
                            page->findLineFromEnd(endCursor, 1);
                        }
                        if (!endCursor->isNull()) {
                            shared_ptr<ZLTextWordCursor> startCursor = findStartOfPrevousPage(page, endCursor);
                            if (startCursor->samePositionAs(*page->StartCursor)) {
                                page->StartCursor->setCursor(findStartOfPrevousPage(page, page->StartCursor));
                            } else {
                                page->StartCursor->setCursor(*startCursor);
                            }
                        } else {
                            page->StartCursor->setCursor(findStartOfPrevousPage(page, page->StartCursor));
                        }
                        break;
                    }
                    case SCROLL_LINES:
                        page->StartCursor->setCursor(findStart(page, page->StartCursor, LINE_UNIT, myOverlappingValue));
                        break;
                    case SCROLL_PERCENTAGE:
                        page->StartCursor->setCursor(findStart(page, page->StartCursor, PIXEL_UNIT, page->getTextHeight() * myOverlappingValue / 100));
                        break;
                }
                // IMPORTANT: 根据开始和结束游标创建页面信息
                buildInfos(page, page->StartCursor, page->EndCursor);
                if (page->isEmptyPage()) {
                    // 页面创建失败，重新设置页面开始游标，重新创建页面信息
                    page->StartCursor->setCursor(findStart(page, page->StartCursor, LINE_UNIT, 1));
                    buildInfos(page, page->StartCursor, page->EndCursor);
                }
            }
            break;
            
        case START_IS_KNOWN:
            // 页面开始游标已确定
            if (!page->StartCursor->isNull()) {
                buildInfos(page, page->StartCursor, page->EndCursor);
            }
            break;
            
        case END_IS_KNOWN:
            // 页面结束游标已确定
            if (!page->EndCursor->isNull()) {
                page->StartCursor->setCursor(findStartOfPrevousPage(page, page->EndCursor));
                buildInfos(page, page->StartCursor, page->EndCursor);
            }
            break;
            
        default:
            break;
    }
    
    // 将页面状态设置为准备好绘制
    page->PaintState = READY;
    // 清除当前视图的行信息缓存
    myLineInfoCache.clear();
    
    // 如果准备绘制的是当前展示页，则根据条件判断是否要重置前一页/后一页
    if (page == myCurrentPage) {
        if (oldState != START_IS_KNOWN) {
            myPreviousPage->reset();
        }
        if (oldState != END_IS_KNOWN) {
            myNextPage->reset();
        }
    }
}

// 根据页面的开始和结束游标创建页面信息
void ZLTextView::buildInfos(shared_ptr<ZLTextPage> page, shared_ptr<ZLTextWordCursor> start, shared_ptr<ZLTextWordCursor>result) {
    // 将结束游标移到开始游标位置
    result->setCursor(start);
    // 获取页面高度
    int textAreaHeight = page->getTextHeight();
    // 清空页面的行信息
    page->LineInfos.clear();
    page->Column0Height = 0;
    bool nextParagraph;
    bool endSection = false;
    shared_ptr<ZLTextLineInfo> info = 0;
    do {
        shared_ptr<ZLTextLineInfo> previousInfo = info;
        // 重置样式
        resetTextStyle();
        // 获取结束游标的段落游标
        shared_ptr<ZLTextParagraphCursor> paragraphCursor = result->getParagraphCursor();
        // 获取结束游标的元素索引
        int wordIndex = result->getElementIndex();
        // 对段落中的元素设置样式
        applyStyleChanges(paragraphCursor, 0, wordIndex);
        // 根据段落、单词、字母的游标以及样式创建行信息
        info = new ZLTextLineInfo(paragraphCursor, wordIndex, result->getCharIndex(), getTextStyle());
        // 获取段落中单词数量
        int endIndex = info->ParagraphCursorLength;
        while (info->EndElementIndex != endIndex) {
            // 行信息结束游标没有指向段落的最后一个单词
            // IMPORTANT: 更新行信息（包含计算行高度）
            info = processTextLine(page, paragraphCursor, info->EndElementIndex, info->EndCharIndex, endIndex, previousInfo);
            // 页面高度减去该行的高度
            textAreaHeight -= info->Height + info->Descent;
            
            // 若已经铺满一页则换页；若页面行数超过一栏的限制，则换一栏
            if (textAreaHeight < 0 && page->LineInfos.size() > page->Column0Height) {
                if (page->Column0Height == 0 && page->twoColumnView()) {
                    textAreaHeight = page->getTextHeight();
                    textAreaHeight -= info->Height + info->Descent;
                    page->Column0Height = (int)page->LineInfos.size();
                } else {
                    break;
                }
            }
            // 没有铺满一页，页面高度减去行间距
            textAreaHeight -= info->VSpaceAfter;
            // 将结束游标移动到当前行的最后一个单词的最后一个字母
            result->moveTo(info->EndElementIndex, info->EndCharIndex);
            // 将当前行插入当前页的行信息数组中
            page->LineInfos.insert(page->LineInfos.end(), info);
            // 减去行间距后若当前页高度不足，则换页或换一栏
            if (textAreaHeight < 0) {
                if (page->Column0Height == 0 && page->twoColumnView()) {
                    textAreaHeight = page->getTextHeight();
                    page->Column0Height = (int)page->LineInfos.size();
                } else {
                    break;
                }
            }
        }
        
        // 检查是否要切换下一段落
        nextParagraph = result->isEndOfParagraph() && result->nextParagraph();
        if (nextParagraph && result->getParagraphCursor()->isEndOfSection()) {
            // 如果是分栏显示，重新设置页面高度
            if (page->Column0Height == 0 && page->twoColumnView() && page->LineInfos.size()>0) {
                textAreaHeight = page->getTextHeight();
                page->Column0Height = (int)page->LineInfos.size();
            }
        }
        // 检查是否是section？末尾
        endSection = result->getParagraphCursor()->isEndOfSection();
        
    } while (nextParagraph && textAreaHeight >= 0 && (!endSection || page->LineInfos.size() == page->Column0Height));
    
    // 重置样式
    resetTextStyle();
}

#pragma mark - process text line
// 更新行信息（接口）
shared_ptr<ZLTextLineInfo> ZLTextView::processTextLine(shared_ptr<ZLTextPage> page,
                                                       shared_ptr<ZLTextParagraphCursor> paragraphCursor,
                                                       int startIndex,
                                                       int startCharIndex,
                                                       int endIndex,
                                                       shared_ptr<ZLTextLineInfo> previousInfo) {
    // 更新行信息
    shared_ptr<ZLTextLineInfo> info = processTextLineInternal(page, paragraphCursor, startIndex, startCharIndex, endIndex,
                                                              previousInfo);
    
    if (info->EndElementIndex == startIndex && info->EndCharIndex == startCharIndex) {
        info->EndElementIndex = paragraphCursor->getParagraphLength();
        info->EndCharIndex = 0;
        // TODO: add error element
    }
    
    return info;
}

// 更新行信息（实现）
shared_ptr<ZLTextLineInfo> ZLTextView::processTextLineInternal(shared_ptr<ZLTextPage> page,
                                                               shared_ptr<ZLTextParagraphCursor> paragraphCursor,
                                                               int startIndex,
                                                               int startCharIndex,
                                                               int endIndex,
                                                               shared_ptr<ZLTextLineInfo> previousInfo) {
    // 获取绘图容器
    shared_ptr<ZLPaintContext> context = getContext();
    // 创建行信息
    shared_ptr<ZLTextLineInfo> info = new ZLTextLineInfo(paragraphCursor, startIndex, startCharIndex, getTextStyle());
    
    // 在行信息缓存中寻找相同的行信息
    std::set<shared_ptr<ZLTextLineInfo>>::const_iterator it = myLineInfoCache.find(info);
    if (it != myLineInfoCache.end()) {
        // 找到缓存的行信息
        shared_ptr<ZLTextLineInfo> storedinfo = *it;
        // 根据前一行信息调整当前行的高度
        storedinfo->adjust(previousInfo);
        // 对段落中的元素设置样式
        applyStyleChanges(paragraphCursor, startIndex, storedinfo->EndElementIndex);
        // 返回缓存的行信息
        return storedinfo;
    }
    
    // 缓存中没有相同的行信息
    // 设置元素和字母的索引
    int currentElementIndex = startIndex;
    int currentCharIndex = startCharIndex;
    // 检查是否是段落第一行
    bool isFirstLine = startIndex == 0 && startCharIndex == 0;
    
    // 创建游标并设置到段落的开头
    shared_ptr<ZLTextWordCursor> checkCursor = new ZLTextWordCursor(paragraphCursor, startIndex, startCharIndex);
//    AutoTextSnippet xx(checkCursor,50);
//    std::string checkstr = xx.getText();
//    printf(" STR: %s \n",checkstr.c_str());
    
    if (isFirstLine) {
        // 当前是段落第一行
        shared_ptr<ZLTextElement> element = paragraphCursor->getElement(currentElementIndex);
//        uint8_t kind = element->kind();
        // 为每个元素设置样式
        while (isStyleChangeElement(element)) {
            applyStyleChangeElement(element);
            ++currentElementIndex;
            currentCharIndex = 0;
            if (currentElementIndex == endIndex) {
                break;
            }
            element = paragraphCursor->getElement(currentElementIndex);
        }
        // 设置行的开始的索引
        info->StartStyle = getTextStyle();
        info->RealStartElementIndex = currentElementIndex;
        info->RealStartCharIndex = currentCharIndex;
    }
    
    // 获取样式
    shared_ptr<ZLTextStyle> storedStyle = getTextStyle();
    
    // 获取页面宽度
    int maxWidth = page->getTextWidth() - storedStyle->getRightIndent(metrics());
    // 获取左缩进
    info->LeftIndent = storedStyle->getLeftIndent(metrics());
    // 如果是段落第一行，左缩进要添加首行缩进的值
    if (isFirstLine && storedStyle->getAlignment() != ALIGN_CENTER) {
        info->LeftIndent += storedStyle->getFirstLineIndent(metrics());
    }
    // 如果左缩进太多，调整缩进数值
    if (info->LeftIndent > maxWidth - 20) {
        info->LeftIndent = maxWidth * 3 / 4;
    }
    
    // 设置行宽度
    info->Width = info->LeftIndent;
    
    // 如果该行开始的游标就在段落末尾，结束该行并返回
    if (info->RealStartElementIndex == endIndex) {
        info->EndElementIndex = info->RealStartElementIndex;
        info->EndCharIndex = info->RealStartCharIndex;
        return info;
    }
    
    // 记录行目前的宽度和高度
    int newWidth = info->Width;
    int newHeight = info->Height;
    int newDescent = info->Descent;
    // 是否要显示文字内容
    bool wordOccurred = false;
    // 是否可见
    bool isVisible = false;
    // 空格宽度
    int lastSpaceWidth = 0;
    // 统计空格数量
    int internalSpaceCounter = 0;
    // 是否要删除最后一个空格
    bool removeLastSpace = false;
    
    do {
        // 获取到段落中的元素
        shared_ptr<ZLTextElement> element = paragraphCursor->getElement(currentElementIndex);
        // 添加元素宽度
        newWidth += getElementWidth(*element, currentCharIndex);
        // 获取元素高度，并更新行高度
        newHeight = std::max(newHeight, getElementHeight(*element));
        newDescent = std::max(newDescent, getElementDescent(*element));
        
        if (element->kind() == ZLTextElement::HSPACE_ELEMENT) {
            // 空格元素
            if (wordOccurred) {
                wordOccurred = false;
                internalSpaceCounter++;
                lastSpaceWidth = context->getSpaceWidth();
                newWidth += lastSpaceWidth;
            }
            
        } else if (element->kind() == ZLTextElement::NB_HSPACE_ELEMENT) {
            wordOccurred = true;
            
        } else if (element->kind() == ZLTextElement::WORD_ELEMENT) {
            // 文字元素
            wordOccurred = true;
            isVisible = true;
            
        } else if (element->kind() == ZLTextElement::IMAGE_ELEMENT) {
            // 图片元素
            wordOccurred = true;
            isVisible = true;
            
        } else if (element->kind() == ZLTextElement::VIDEO_ELEMENT) {
            // 视频元素
            wordOccurred = true;
            isVisible = true;
            
        } else if (element->kind() == ZLTextElement::EXTENSION_ELEMENT) {
            // 扩展元素
            wordOccurred = true;
            isVisible = true;
            
        } else if (isStyleChangeElement(element)) {
            applyStyleChangeElement(element);
        }
        
        // 超出行宽限制，如果当前元素不是该行第一个元素或者当前元素是文字元素，则结束该行
        if (newWidth > maxWidth) {
            if (info->EndElementIndex != startIndex || element->kind() == ZLTextElement::WORD_ELEMENT) {
                break;
            }
        }
        
        // 准备获取下一个元素，先记录当前元素
        shared_ptr<ZLTextElement> previousElement = element;
        // 当前元素索引+1
        ++currentElementIndex;
        currentCharIndex = 0;
        // 判断是否可以打断
        bool allowBreak = currentElementIndex == endIndex;
        if (!allowBreak) {
            element = paragraphCursor->getElement(currentElementIndex);
            allowBreak = previousElement->kind() != ZLTextElement::NB_HSPACE_ELEMENT
            && element->kind() != ZLTextElement::NB_HSPACE_ELEMENT
            && (!(element->kind() == ZLTextElement::WORD_ELEMENT) || previousElement->kind() == ZLTextElement::WORD_ELEMENT)
            && !(element->kind() == ZLTextElement::IMAGE_ELEMENT)
            && !(element->kind() == ZLTextElement::TEXT_CONTROL_ELEMENT);
        }
        if (allowBreak) {
            // 允许打断
            // 刷新当前行信息
            info->IsVisible = isVisible;
            info->Width = newWidth;
            if (info->Height < newHeight) {
                info->Height = newHeight;
            }
            if (info->Descent < newDescent) {
                info->Descent = newDescent;
            }
            info->EndElementIndex = currentElementIndex;
            info->EndCharIndex = currentCharIndex;
            info->SpaceCounter = internalSpaceCounter;
            
            storedStyle = getTextStyle();
            removeLastSpace = !wordOccurred && (internalSpaceCounter > 0);
        }
    } while (currentElementIndex != endIndex);
    
    // 连字符
    if (currentElementIndex != endIndex && (isHyphenationPossible() || info->EndElementIndex == startIndex)) {
        shared_ptr<ZLTextElement>  element = paragraphCursor->getElement(currentElementIndex);
        if (element->kind() == ZLTextElement::WORD_ELEMENT) {
            ZLTextWord& word = (ZLTextWord&)(*element);
            newWidth -= getWordWidth(word, currentCharIndex);
            int spaceLeft = maxWidth - newWidth;
            if ((word.Length > 3 && spaceLeft > 2 * context->getSpaceWidth()) || info->EndElementIndex == startIndex) {
                shared_ptr<ZLTextHyphenationInfo> hyphenationInfo = getHyphenationInfo(&word);
                int hyphenationPosition = currentCharIndex;
                int subwordWidth = 0;
                for (int right = word.Length - 1, left = currentCharIndex; right > left;) {
                    int mid = (right + left + 1) / 2;
                    int m1 = mid;
                    while (m1 > left && hyphenationInfo != 0 && !hyphenationInfo->isHyphenationPossible(m1)) {
                        --m1;
                    }
                    if (m1 > left) {
                        int w = getWordWidth(word,
                                             currentCharIndex,
                                             m1 - currentCharIndex,
                                             word.Data[word.Offset + m1 - 1] != '-');
                        if (w < spaceLeft) {
                            left = mid;
                            hyphenationPosition = m1;
                            subwordWidth = w;
                        } else {
                            right = mid - 1;
                        }
                    } else {
                        left = mid;
                    }
                }
                if (hyphenationPosition == currentCharIndex && info->EndElementIndex == startIndex) {
                    subwordWidth = getWordWidth(word, currentCharIndex, 1, false);
                    int right = word.Length == currentCharIndex + 1 ? word.Length : word.Length - 1;
                    int left = currentCharIndex + 1;
                    while (right > left) {
                        int mid = (right + left + 1) / 2;
                        int w = getWordWidth(word,
                                             currentCharIndex,
                                             mid - currentCharIndex,
                                             word.Data[word.Offset + mid - 1] != '-');
                        if (w <= spaceLeft) {
                            left = mid;
                            subwordWidth = w;
                        } else {
                            right = mid - 1;
                        }
                    }
                    hyphenationPosition = right;
                }
                if (hyphenationPosition > currentCharIndex) {
                    info->IsVisible = true;
                    info->Width = newWidth + subwordWidth;
                    if (info->Height < newHeight) {
                        info->Height = newHeight;
                    }
                    if (info->Descent < newDescent) {
                        info->Descent = newDescent;
                    }
                    info->EndElementIndex = currentElementIndex;
                    info->EndCharIndex = hyphenationPosition;
                    info->SpaceCounter = internalSpaceCounter;
                    storedStyle = getTextStyle();
                    removeLastSpace = false;
                }
            }
        }
    }
    
    // 删除最后一个空格
    if (removeLastSpace) {
        info->Width -= lastSpaceWidth;
        info->SpaceCounter--;
    }
    
    // 设置样式
    setTextStyle(storedStyle);
    
    // 如果是段落第一行，行的高度要添加与上一段的间隔
    if (isFirstLine) {
        info->VSpaceBefore = info->StartStyle->getSpaceBefore(metrics());
        if (previousInfo != 0) {
            info->PreviousInfoUsed = true;
            info->Height += std::max(0, info->VSpaceBefore - previousInfo->VSpaceAfter);
        } else {
            info->PreviousInfoUsed = false;
            info->Height += info->VSpaceBefore;
        }
    }
    // 如果当前行是段落末尾，要设置当前行与下一段的间隔
    if (info->isEndOfParagraph()) {
        info->VSpaceAfter = getTextStyle()->getSpaceAfter(metrics());
    }
    
    // 将当前行加入缓存
    if (info->EndElementIndex != endIndex || endIndex == info->ParagraphCursorLength) {
        myLineInfoCache.insert(info);
    }
    
    return info;
}

