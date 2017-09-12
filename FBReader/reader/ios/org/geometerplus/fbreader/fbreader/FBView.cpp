//
//  FBView.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/31.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "FBView.hpp"
#include "ActionCode.hpp"
#include "FBReaderApp.h"
#include "SelectionCursor.hpp"
#include "ZLTextRegion.hpp"
#include "WordCountTraverser.hpp"
#include "ZDZLibrary.hpp"
#include "TextBuildTraverser.hpp"
#include "ZLUnicodeUtil.h"
#include "ZLViewWidget.hpp"
#include "FixedTextSnippet.hpp"
#include "ZLUnderlineHighlighting.hpp"

FBView::FBView(FBReaderApp* reader) : ZLTextView((ZLApplication*)reader) {
    myReader = reader;
    backgroundColor = 0;
    textColor = 0;
}

#pragma mark - touch handler
void FBView::onFingerPress(int x, int y) {
    myReader->runAction(ActionCode::HIDE_TOAST, 0);
    
    float maxDist = ZDZLibrary::Instance()->getDisplayDPI() / 4;
    SelectionCursor::Which cursor = findSelectionCursor(x, y, maxDist * maxDist);
    printf("on finger press\n");
    if (cursor != SelectionCursor::None) {
        myReader->runAction(ActionCode::SELECTION_HIDE_PANEL, 0);
        moveSelectionCursorTo(cursor, x, y);
        return;
    }
    
//    if (myReader.MiscOptions.AllowScreenBrightnessAdjustment.getValue() && x < getContextWidth() / 10) {
//        myIsBrightnessAdjustmentInProgress = true;
//        myStartY = y;
//        myStartBrightness = myReader.getViewWidget().getScreenBrightness();
//        return;
//    }
//    
//    startManualScrolling(x, y);
}

void FBView::onFingerRelease(int x, int y) {
    SelectionCursor::Which cursor = getSelectionCursorInMovement();
    printf("onFingerRelease %d\n",cursor);
    if (cursor != SelectionCursor::None) {
        releaseSelectionCursor();
        return;
    }
    
//    synchronized (this) {
//        if (myIsBrightnessAdjustmentInProgress) {
//            if (x >= getContextWidth() / 5) {
//                myIsBrightnessAdjustmentInProgress = false;
//                startManualScrolling(x, y);
//            } else {
//                final int delta = (myStartBrightness + 30) * (myStartY - y) / getContextHeight();
//                myReader.getViewWidget().setScreenBrightness(myStartBrightness + delta);
//                return;
//            }
//        }
//        
//        if (isFlickScrollingEnabled()) {
//            myReader.getViewWidget().scrollManuallyTo(x, y);
//        }
//    }
}

void FBView::onFingerMove(int x, int y) {
    SelectionCursor::Which cursor = getSelectionCursorInMovement();
    if (cursor != SelectionCursor::None) {
//        releaseSelectionCursor();
        moveSelectionCursorTo(cursor, x, y);
//    } else if (myIsBrightnessAdjustmentInProgress) {
//        myIsBrightnessAdjustmentInProgress = false;
//    } else if (isFlickScrollingEnabled()) {
//        myReader.getViewWidget().startAnimatedScrolling(x, y, myReader.PageTurningOptions.AnimationSpeed.getValue());
    }
}

bool FBView::onFingerLongPress(int x, int y) {
    myReader->runAction(ActionCode::HIDE_TOAST, 0);
    
    shared_ptr<ZLTextRegion> region = findRegion(x, y, maxSelectionDistance(), *ZLTextRegion::anyFiler);
    if (region != 0) {
        shared_ptr<ZLTextRegion::Soul> soul = region->getSoul();
        bool doSelectRegion = false;
        if (soul->getSoulKind() == ZLTextRegion::Soul::SoulKind_ZLTextWordRegionSoul) {
//            switch (myReader->MiscOptions.WordTappingAction.getValue()) {
              //  case startSelecting:
                myReader->runAction(ActionCode::SELECTION_HIDE_PANEL, 0);
                initSelection(x, y);
                SelectionCursor::Which cursor = findSelectionCursor(x, y);
                if (cursor != 0) {
                    moveSelectionCursorTo(cursor, x, y);
                }
                return true;
//                case selectSingleWord:
//                case openDictionary:
//                doSelectRegion = true;
//                break;
//            }
        } else if (soul->getSoulKind() == ZLTextRegion::Soul::SoulKind_ZLTextImageRegionSoul) {
//            doSelectRegion = myReader.ImageOptions.TapAction.getValue() != ImageOptions.TapActionEnum.doNothing;
            doSelectRegion = true;
        } else if (soul->getSoulKind() == ZLTextRegion::Soul::SoulKind_ZLTextHyperlinkRegionSoul) {
            doSelectRegion = true;
        }
        
        if (doSelectRegion) {
            outlineRegion(region);
            myReader->getViewWidget()->reset();
            myReader->getViewWidget()->repaint();
            return true;
        }
    } else {
        printf("pressed region null");
    }
    return false;
}

void FBView::onFingerReleaseAfterLongPress(int x, int y) {
    SelectionCursor::Which cursor = getSelectionCursorInMovement();
     printf("onFingerReleaseAfterLongPress: %d\n", cursor);
    if (cursor != SelectionCursor::None) {
        releaseSelectionCursor();
        return;
    }

    shared_ptr<ZLTextRegion> region = getOutlinedRegion();
    if (region != 0) {
        shared_ptr<ZLTextRegion::Soul> soul = region->getSoul();

        bool doRunAction = false;
        if (soul->getSoulKind() == ZLTextRegion::Soul::SoulKind_ZLTextWordRegionSoul) {
            doRunAction = false;
//            myReader.MiscOptions.WordTappingAction.getValue() == MiscOptions.WordTappingActionEnum.openDictionary;
            
        } else if (soul->getSoulKind() == ZLTextRegion::Soul::SoulKind_ZLTextImageRegionSoul) {
            // open image not support temp, zhangdong
            doRunAction = true;
//            myReader.ImageOptions.TapAction.getValue() == ImageOptions.TapActionEnum.openImageView;
        }
        
        if (doRunAction) {
            myReader->runAction(ActionCode::PROCESS_HYPERLINK,0);
        }
    } else {
         printf("released region null");
    }
}

void FBView::onFingerMoveAfterLongPress(int x, int y) {
    SelectionCursor::Which cursor = getSelectionCursorInMovement();
    if (cursor != SelectionCursor::None) {
        moveSelectionCursorTo(cursor, x, y);
        return;
    }
    
    shared_ptr<ZLTextRegion> region = getOutlinedRegion();
    if (region != 0) {
        shared_ptr<ZLTextRegion::Soul> soul = region->getSoul();
        if (soul->getSoulKind()== ZLTextRegion::Soul::SoulKind_ZLTextHyperlinkRegionSoul ||
            soul->getSoulKind() == ZLTextRegion::Soul::SoulKind_ZLTextWordRegionSoul) {
//            if (myReader->MiscOptions.WordTappingAction.getValue() != MiscOptions.WordTappingActionEnum.doNothing)
            {
                region = findRegion(x, y, maxSelectionDistance(), *ZLTextRegion::anyFiler);
                if (region != 0) {
                    soul = region->getSoul();
                    if (soul->getSoulKind() == ZLTextRegion::Soul::SoulKind_ZLTextHyperlinkRegionSoul
                        || soul->getSoulKind() == ZLTextRegion::Soul::SoulKind_ZLTextWordRegionSoul) {
                        outlineRegion(region);
                        myReader->getViewWidget()->reset();
                        myReader->getViewWidget()->repaint();
                    }
                }
            }
        }
    } else {
        printf("moved region null");
    }
}

bool FBView::onFingerSingleTap(int x, int y) {
    bool isTapOpinion = isTapOnOpinion(x, y);
    if (isTapOpinion) {
        return true;
    }

    bool isTapLinedText = isTapOnLinedText(x, y);
    return isTapLinedText;
}

void FBView::onFingerDoubleTap(int x, int y) {
}

void FBView::onFingerEventCancelled() {
}

bool FBView::isScrollbarShown() {
    return false;
}

int FBView::getScrollbarFullSize() {
    return  100;
}

shared_ptr<ZLColor> FBView::getBackgroundColor() {
    if (backgroundColor != 0) {
        return backgroundColor;
    } else {
        return new ZLColor(255, 255, 0);
    }
}

void FBView::setBackgroundColor(int r, int g, int b) {
    backgroundColor = new ZLColor(r, g, b);
}

shared_ptr<ZLColor> FBView::getTextColor(shared_ptr<ZLTextHyperlink> hyperlink) {
    if (textColor == 0) {
        textColor = new ZLColor(0, 0, 0);
    }
    return textColor;
}

void FBView::setTextColor(int r, int g, int b) {
    textColor = new ZLColor(r, g, b);
}

shared_ptr<ZLTextStyleCollection> FBView::getTextStyleCollection() {
    if(myTextStyleColloction == 0x0){
        myTextStyleColloction = new ZLTextStyleCollection("Base");
    }
    return myTextStyleColloction;
}

ZLPaintContext::ColorAdjustingMode FBView::getAdjustingModeForImages() {
//    if (myReader->ImageOptions->MatchBackground->getValue()) {
//        if (ColorProfile.DAY.equals(myViewOptions.getColorProfile().Name)) {
//            return ZLPaintContext.ColorAdjustingMode.DARKEN_TO_BACKGROUND;
//        } else {
//            return ZLPaintContext.ColorAdjustingMode.LIGHTEN_TO_BACKGROUND;
//        }
//    } else {
//        return ZLPaintContext.ColorAdjustingMode.NONE;
//    }
    return ZLPaintContext::NONE;
}

ZLTextViewBase_ImageFitting FBView::getImageFitting() {
    return none;
}

int FBView::getLeftMargin() {
    int dpi = ZDZLibrary::Instance()->getDisplayDPI();
    int x = getContextWidth();
    int y = getContextHeight();
    int horimargin = std::min(dpi/5,std::min(x,y)/30);
    return horimargin;
}

int FBView::getRightMargin() {
    int dpi = ZDZLibrary::Instance()->getDisplayDPI();
    int x = getContextWidth();
    int y = getContextHeight();
    int horimargin = std::min(dpi/5,std::min(x,y)/30);
    return horimargin;
}

int FBView::getTopMargin() {
    return 0;
}

int FBView::getBottomMargin() {
    return 4;
}

int FBView::getSpaceBetweenColumns() {
    return 0;
}

bool FBView::twoColumnView() {
    return false;
}

shared_ptr<ZLFile> FBView::getWallpaperFile() {
    return 0;
}

ZLPaintContext_FillMode FBView::getFillMode() {
    return tile;
}

shared_ptr<ZLColor> FBView::getSelectionBackgroundColor() {
    return myReader->selectingBgColor();
//    return new ZLColor(247, 193, 185);
//    return new ZLColor(82,134,194);
}

shared_ptr<ZLColor> FBView::getSelectionCursorColor() {
    return myReader->selectingCursorColor();
}

shared_ptr<ZLColor> FBView::getSelectionForegroundColor() {
    if (textColor == 0) {
        textColor = new ZLColor(0, 0, 0);
    }
    return textColor;
}

shared_ptr<ZLColor> FBView::getHighlightingBackgroundColor() {
    return new ZLColor(247, 193, 185);
//    return new ZLColor(96,96,128);
//    return new ZLColor(255,192,128);
//    return new ZLColor(255,255,0);
}

shared_ptr<ZLColor> FBView::getHighlightingForegroundColor() {
    if (textColor == 0) {
        textColor = new ZLColor(0, 0, 0);
    }
    return textColor;
}

shared_ptr<ZLColor> FBView::getOutlineColor() {
    return 0;
//    return new ZLColor(96,96,128);
//    return new ZLColor(255,192,128);
//    return new ZLColor(255,255,0);
}

shared_ptr<ZLColor> FBView::getUnderlineColor(shared_ptr<ZLTextPosition> start, shared_ptr<ZLTextPosition> end) {
    return myReader->underlineColor(start, end);
//    return new ZLColor(247, 193, 185);
}

int FBView::getNoteCount(shared_ptr<ZLTextPosition> start, shared_ptr<ZLTextPosition> end) {
    return myReader->getNoteCount(start, end);
}

shared_ptr<ExtensionElementManager> FBView::getExtensionManager() {
    return 0;
}

void FBView::onScrollingFinished(int pageIndex) {
    ZLTextView::onScrollingFinished(pageIndex);
}

void FBView::releaseSelectionCursor() {
    ZLTextView::releaseSelectionCursor();
    int wordcount = getCountOfSelectedWords();
    printf("check select word count: %d\n", wordcount);
    if (wordcount> 0) {
        // 打开panel前先添加划线
        shared_ptr<TextSnippet> snippet = getSelectedSnippet();
        shared_ptr<ZLTextPosition> textStart = ((FixedTextSnippet&)(*snippet)).getStart();
        shared_ptr<ZLTextPosition> textEnd = ((FixedTextSnippet&)(*snippet)).getEnd();
        int xStart = getSelectionStartX();
        int yStart = getSelectionStartY();
        int xEnd = getSelectionEndX();
        int yEnd = getSelectionEndY();

        ZLTextFixedPosition* fixedStart = new ZLTextFixedPosition(textStart);
        ZLTextFixedPosition* fixedEnd = new ZLTextFixedPosition(textEnd);
        appendHightLighting(new ZLUnderlineHighlighting(this, fixedStart, fixedEnd));
//        appendHightLighting(new ZLTextManualHighlighting(this, fixedStart, fixedEnd));

        BookNoteContext noteContext = createBookNoteContext(textStart->getParagraphIndex(),
                                                            textStart->getElementIndex(),
                                                            textStart->getCharIndex(),
                                                            xStart,
                                                            yStart,
                                                            textEnd->getParagraphIndex(),
                                                            textEnd->getElementIndex(),
                                                            textEnd->getCharIndex(),
                                                            xEnd,
                                                            yEnd);
        printf("Show panel with lined text\n");
        myReader->runAction(ActionCode::SELECTION_SHOW_PANEL, &noteContext);

    } else {
        printf("### meet wrong number??\n");
//        int wordcount = getCountOfSelectedWords();
    }
}

int FBView::getCountOfSelectedWords() {
    WordCountTraverser* traverser = new WordCountTraverser(this);
    if (!isSelectionEmpty()) {
        traverser->traverse(getSelectionStartPosition().getT(), getSelectionEndPosition().getT());
    }
    int cnt =  traverser->getCount();
    delete traverser;
    
    
//    // check words
//    if(cnt >0) {
//        TextBuildTraverser* newb =  new TextBuildTraverser(this);
//        if (!isSelectionEmpty()) {
//            newb->traverse(*getSelectionStartPosition(), *getSelectionEndPosition());
//            std::vector<short> val = newb->getText();
//            if(val.size()>0) {
//                std::string to="";
//                ZLUnicodeUtil::Ucs2String from;
//                for(int i=0;i<val.size();i++){
//                    from.insert(from.end(), val[i]);
//                }
//                ZLUnicodeUtil::ucs2ToUtf8(to, from);
//                
//            }
//        }
//    }
    
    return  cnt;
}

shared_ptr<TextSnippet> FBView::getSelectedSnippet() {
    shared_ptr<ZLTextPosition> start = getSelectionStartPosition();
    shared_ptr<ZLTextPosition> end = getSelectionEndPosition();
    if (start == 0 || end == 0) {
        return 0;
    }
    TextBuildTraverser* traverser = new TextBuildTraverser(this);
    traverser->traverse(start.getT(), end.getT());
    std::string to = traverser->getText();
    return new FixedTextSnippet(start, end, to);
}

std::string FBView::textInParagraph(int pIndex) {
    shared_ptr<ZLTextParagraphCursor> pCursor = cursor(pIndex);
    
    ZLTextFixedPosition* firstPos = new ZLTextFixedPosition(pIndex, 0, 0);
    ZLTextFixedPosition* lastPos = new ZLTextFixedPosition(pIndex, pCursor->getParagraphLength() - 1, 0);
    TextBuildTraverser* traverser = new TextBuildTraverser(this);
    traverser->traverse(firstPos, lastPos);
    std::string traverseText = traverser->getText();
//    printf("paragraph contains mark: %s\n", traverseText.c_str());
    delete firstPos;
    delete lastPos;
    delete traverser;
    
    return traverseText;
}

int FBView::paragraphIndexOfPos(int x, int y) {
    shared_ptr<ZLTextRegion> region = findRegion(x, y, maxSelectionDistance(), *ZLTextRegion::anyFiler);
    if (region != 0) {
        shared_ptr<ZLTextRegion::Soul> soul = region->getSoul();
        return soul->ParagraphIndex;
    }
    return -1;
}

#pragma mark - lined text
bool FBView::isTapOnLinedText(int x, int y) {
    shared_ptr<ZLTextHighlighting> highlighting = findHighlighting(x, y, maxSelectionDistance());
    if (highlighting == 0) {
        return false;
    }
    
    if (!highlighting->isUnderline()) {
        return false;
    }

    // 点击划线
    shared_ptr<ZLTextPosition> textStart = highlighting->getStartPosition();
    shared_ptr<ZLTextPosition> textEnd = highlighting->getEndPosition();
    shared_ptr<ZLTextElementArea> startArea = highlighting->getStartArea(*myCurrentPage);
    shared_ptr<ZLTextElementArea> endArea = highlighting->getEndArea(*myCurrentPage);
    int xStart = startArea != 0 ? startArea->XStart : 0;
    int yStart = startArea != 0 ? startArea->YStart : 0;
    int xEnd = endArea != 0 ? endArea->XEnd : 0;
    int yEnd = endArea != 0 ? endArea->YEnd : 0;
    
    BookNoteContext noteContext = createBookNoteContext(textStart->getParagraphIndex(),
                                                        textStart->getElementIndex(),
                                                        textStart->getCharIndex(),
                                                        xStart,
                                                        yStart,
                                                        textEnd->getParagraphIndex(),
                                                        textEnd->getElementIndex(),
                                                        textEnd->getCharIndex(),
                                                        xEnd,
                                                        yEnd);
    
    printf("Show panel with clicked note\n");
    myReader->runAction(ActionCode::SELECTION_NOTES, &noteContext);
    
    return true;
}

BookNoteContext FBView::createBookNoteContext(int pStart, int eStart, int cStart, int xStart, int yStart,
                                              int pEnd, int eEnd, int cEnd, int xEnd, int yEnd) {
    BookNoteContext note;
    note.ParagraphIndex = pStart;
    note.ElementIndex = eStart;
    note.CharIndex = cStart;
    note.ParagraphEndIndex = pEnd;
    note.ElementEndIndex = eEnd;
    note.CharEndIndex = cEnd;
    note.StartX = xStart;
    note.StartY = yStart;
    note.EndX = xEnd;
    note.EndY = yEnd;
    return note;
}

#pragma mark - opinion
int FBView::getOpinionCount(int pIndex) {
    return myReader->getOpinionCount(pIndex);
}

bool FBView::isTapOnOpinion(int x, int y) {
    shared_ptr<ZLTextHighlighting> highlighting = findHighlighting(x, y, maxSelectionDistance());
    if (highlighting == 0) {
        return false;
    }
    
    if (!highlighting->isOpinion()) {
        return false;
    }

    // 点击看法
    shared_ptr<ZLTextPosition> textStart = highlighting->getStartPosition();
    shared_ptr<ZLTextPosition> textEnd = highlighting->getEndPosition();
    int pIndex = textStart->getParagraphIndex();
    myReader->runAction(ActionCode::SELECTION_OPINIONS, &pIndex);

    return true;

//    shared_ptr<ZLTextRegion> region = findRegion(x, y, maxSelectionDistance(), *ZLTextRegion::anyFiler);
//    if (region == 0) {
//        return false;
//    }
//
//    shared_ptr<ZLTextRegion::Soul> soul = region->getSoul();
//    int pIndex = soul->ParagraphIndex;
//    int eIndex = 0;
//    int cIndex = 0;
//    myReader->getLastElementOfParagraph(pIndex, &eIndex, &cIndex);
//    shared_ptr<ZLTextPosition> pEnd = new ZLTextFixedPosition(pIndex, eIndex, cIndex);
//    if (soul->compareTo(pEnd) == 0) {
//        if (getOpinionCount(pIndex)) {
//            // 点击看法
//            printf("click opinion at p:%d\n", pIndex);
//            myReader->runAction(ActionCode::SELECTION_OPINIONS, &pIndex);
//            return true;
//        }
//    }
//
//    return false;
}

