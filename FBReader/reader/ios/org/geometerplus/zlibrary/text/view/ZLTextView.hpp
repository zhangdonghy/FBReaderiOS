//
//  ZLTextView.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/31.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLTextView_hpp
#define ZLTextView_hpp

#include <stdio.h>
//#include <hash_map>
#include <set>
#include <map>
#include "ZLTextViewBase.hpp"
#include "ZLApplication.hpp"
#include "ZDZLTextModel.hpp"
#include "ZLTextLineInfo.hpp"
#include "ZLTextWordCursor.hpp"
#include "ZLTextPage.hpp"
#include "ZLTextStyleCollection.hpp"
#include "ZLTextHyphenationInfo.hpp"
#include "ZLTextHyphenator.hpp"
#include "CursorManager.hpp"
#include "ZLTextRegion.hpp"
#include "SelectionCursor.hpp"
#include "ZLTextSelection.hpp"
#include "ZLViewWidget.hpp"
#include "ZLViewEnums_PageIndex.hpp"
#include "RationalNumber.hpp"
class ZLTextHighlighting;

class ZLTextPage;
class ZLTextHighlighting;
enum ZLTextView_ScrollingMode {
     NO_OVERLAPPING = 0,
     KEEP_LINES = 1,
     SCROLL_LINES = 2,
     SCROLL_PERCENTAGE = 3
};
enum ZLTextView_SizeUnit {
     PIXEL_UNIT = 0,
     LINE_UNIT = 1
};
class ParagraphSize
{
public:
    int Height;
    int TopMargin;
    int BottomMargin;
};
class ZLTextSelection;

class PagePosition
{
    public:
    int Current;
    int Total;
    PagePosition(int current, int total) {
        Current = current;
        Total = total;
    }
};

class ZLTextView: public ZLTextViewBase {

protected:
    shared_ptr<ZLTextPage> myCurrentPage;
private:
    
    shared_ptr<ZDZLTextModel> myLettersModel;
    int myLettersBufferLength = 0;
    float myCharWidth ;//= -1f;
    short* myLettersBuffer;// = new char[512];
    int myLettersBufferStoredLength = 512;
    
    
    shared_ptr<CursorManager> myCursorManager;
    
    shared_ptr<ZDZLTextModel> myModel;
    
    shared_ptr<ZLTextPage> myPreviousPage;
    
    shared_ptr<ZLTextPage> myNextPage;
    shared_ptr<ZLTextSelection> mySelection ;
    shared_ptr<ZLTextRegion::Soul> myOutlinedRegionSoul;
    bool myShowOutline ;//= true;
    //std::map<shared_ptr<ZLTextLineInfo>,shared_ptr<ZLTextLineInfo>> myLineInfoCache ;//= new HashMap<ZLTextLineInfo,ZLTextLineInfo>();
    std::set<shared_ptr<ZLTextLineInfo>> myLineInfoCache;
    
    std::vector<shared_ptr<ZLTextHighlighting>> myHighlightings;
    
    std::vector<shared_ptr<ZLTextHighlighting>> myReadHighlightings;
    void preparePaintInfo(shared_ptr<ZLTextPage> page);
    
    int myScrollingMode;
    int myOverlappingValue;
    shared_ptr<ZLTextWordCursor> findStartOfPrevousPage( shared_ptr<ZLTextPage> page, shared_ptr<ZLTextWordCursor> end) {
        if (twoColumnView()) {
            end = findStart(page, end, PIXEL_UNIT, page->getTextHeight());
        }
        end = findStart(page, end, PIXEL_UNIT, page->getTextHeight());
        return end;
    }
    shared_ptr<ZLTextWordCursor>
    findStart(shared_ptr<ZLTextPage> page, shared_ptr<ZLTextWordCursor> end, int unit, int height) ;
    
    shared_ptr<ParagraphSize> paragraphSize(shared_ptr<ZLTextPage> page,
                                            shared_ptr<ZLTextWordCursor> cursor,
                                            bool beforeCurrentPosition,
                                            int unit) ;
    
    int infoSize(shared_ptr<ZLTextLineInfo> info, int unit) ;
    void buildInfos(shared_ptr<ZLTextPage> page, shared_ptr<ZLTextWordCursor> start, shared_ptr<ZLTextWordCursor>result) ;
    bool isHyphenationPossible() {
        ZLTextBaseStyle& xx = (ZLTextBaseStyle& )(*(getTextStyleCollection()->getBaseStyle()));
        return xx.AutoHyphenationOption->getValue() && getTextStyle()->allowHyphenations();
    }
    
    ZLTextWord* myCachedWord;
    shared_ptr<ZLTextHyphenationInfo> myCachedInfo;
    
    shared_ptr<ZLTextHyphenationInfo> getHyphenationInfo(ZLTextWord* word) {
        if (myCachedWord != word)
        {
            myCachedWord = word;
            myCachedInfo = ZLTextHyphenator::Instance()->getInfo(*word);
        }
        return myCachedInfo;
    }
    void drawSelectionCursor(shared_ptr<ZLPaintContext> context, ZLTextPage& page, SelectionCursor::Which which) {
        shared_ptr<ZLTextElementArea> area = getSelectionCursorAreaToDraw(page, which);
        if (area != 0) {
            SelectionCursor::draw(context, which, area, getSelectionCursorColor());
        }
    }
    void prepareTextLine(shared_ptr<ZLTextPage> page, shared_ptr<ZLTextLineInfo> info, int x, int y, int columnIndex) ;
    static  short SPACE[1] ;//=  { ' ' };
    void drawTextLine(shared_ptr<ZLTextPage> page, std::vector<ZLTextHighlighting*> hilites, shared_ptr<ZLTextLineInfo> info, int from, int to) ;
    
    std::vector<ZLTextHighlighting*> findHilites(ZLTextPage& page) ;
    
   ZLTextHighlighting* getWordHilite(shared_ptr<ZLTextPosition> pos, std::vector<ZLTextHighlighting*> hilites ) {
        //for (ZLTextHighlighting h : hilites) {
        for(int i=0;i<hilites.size();i++){
            if (hilites[i]->getStartPosition()->compareToIgnoreChar(pos) <= 0
                && pos->compareToIgnoreChar(hilites[i]->getEndPosition()) <= 0) {
                return hilites[i];
            }
        }
        return 0;
    }
    float computeCharWidth();
    float computeCharsPerPage();
    int computeTextPageNumber(int textSize) {
        if (myModel == 0 || myModel->getParagraphsNumber() == 0) {
            return 1;
        }
        
        float factor = 1.0f / computeCharsPerPage();
        float pages = textSize * factor;
        return std::max((int)(pages + 1.0f - 0.5f * factor), 1);
    }
    float computeCharWidth(short* pattern, int length) {
        return getContext()->getStringWidth(pattern, 0, length) / ((float)length);
    }
   int getCurrentCharNumber(int pageIndex, bool startNotEndOfPage);
    shared_ptr<ZLTextPage> getPage(ZLViewEnums_PageIndex pageIndex) {
        switch (pageIndex.getValue()) {
            default:
            case ZLViewEnums_PageIndex::current:
            return myCurrentPage;
            case ZLViewEnums_PageIndex::previous:
            return myPreviousPage;
            case ZLViewEnums_PageIndex::next:
            return myNextPage;
        }
    }
    int sizeOfTextBeforeCursor(shared_ptr<ZLTextWordCursor> wordCursor) ;
protected:
    int sizeOfFullText() {
        if (myModel == 0 || myModel->getParagraphsNumber() == 0) {
            return 1;
        }
        return myModel->getTextLength(myModel->getParagraphsNumber() - 1);
    }
    
    shared_ptr<ZLTextHighlighting> findHighlighting(int x, int y, int maxDistance);

    shared_ptr<ZLTextRegion> findRegion(int x, int y, ZLTextRegion::Filter& filter);
    shared_ptr<ZLTextRegion> findRegion(int x, int y, int maxDistance, ZLTextRegion::Filter& filter);
    shared_ptr<ZLTextElementAreaVector::RegionPair> findRegionsPair(int x, int y, ZLTextRegion::Filter& filter) ;
    
public:
    
    shared_ptr<RationalNumber> getProgress() {
        shared_ptr<PagePosition> position = pagePosition();
        return RationalNumber::create(position->Current, position->Total);
    }
    void gotoProgress(long progress) {
        shared_ptr<PagePosition> curPosition = pagePosition();
        int target = curPosition->Total * (progress / 100.0);
        gotoPagePosition(target);
    }
    void gotoPagePosition(int page);
    void gotoPositionByEnd(int paragraphIndex, int wordIndex, int charIndex);
  
    shared_ptr<PagePosition> pagePosition() ;
    void appendHightLighting(shared_ptr<ZLTextHighlighting> hi) {
        myHighlightings.insert(myHighlightings.end(), hi);
    }
    void removeHightLighting(shared_ptr<ZLTextHighlighting> hi) {
        if (myHighlightings.size() > 0) {
            for (std::vector<shared_ptr<ZLTextHighlighting>>::const_iterator it = myHighlightings.begin();
                 it != myHighlightings.end(); it++) {
                shared_ptr<ZLTextHighlighting> curHi = *it;
                if ((curHi->getStartPosition()->compareTo(*(hi->getStartPosition()))) == 0
                    && (curHi->getEndPosition()->compareTo(*(hi->getEndPosition())) ) == 0) {
                    myHighlightings.erase(it);
                    break;
                }
            }
        }
    }
    void appendReadHightLighting(shared_ptr<ZLTextHighlighting> hi){
        myReadHighlightings.insert(myReadHighlightings.end(), hi);
    }
    void clearReadHightLighting(){
        myReadHighlightings.clear();
    }
    void gotoPosition(shared_ptr<ZLTextPosition> position) {
        if (position != 0) {
            gotoPosition(position->getParagraphIndex(),
                         position->getElementIndex(),
                         position->getCharIndex());
        }
    }
   void gotoPosition(int paragraphIndex, int wordIndex, int charIndex) {
        if (myModel != 0 && myModel->getParagraphsNumber() > 0) {
            Application->getViewWidget()->reset();
            myCurrentPage->moveStartCursor(paragraphIndex, wordIndex, charIndex);
            myPreviousPage->reset();
            myNextPage->reset();
            preparePaintInfo(myCurrentPage);
            if (myCurrentPage->isEmptyPage()) {
                turnPage(true, NO_OVERLAPPING, 0);
            }
        }
    }
    shared_ptr<ZLTextWordCursor> getStartCursor() {
        if (myCurrentPage->StartCursor->isNull()) {
            preparePaintInfo(myCurrentPage);
        }
        return myCurrentPage->StartCursor;
    }
    shared_ptr<ZLTextWordCursor> getEndCursor() {
        return myCurrentPage->EndCursor;
    }
    int getSelectionStartX();
    
    int getSelectionEndX();
    
    int getSelectionStartY();
    
    int getSelectionEndY();
    
    void clearSelection();

    shared_ptr<ZLTextLineInfo>
    processTextLine(
                                shared_ptr<ZLTextPage> page,
                                shared_ptr<ZLTextParagraphCursor> paragraphCursor,
                                int startIndex,
                                int startCharIndex,
                                int endIndex,
                                shared_ptr<ZLTextLineInfo> previousInfo
                                );
    shared_ptr<ZLTextLineInfo> processTextLineInternal(
                                           shared_ptr<ZLTextPage> page,
                                           shared_ptr<ZLTextParagraphCursor> paragraphCursor,
                                             int startIndex,
                                            int startCharIndex,
                                            int endIndex,
                                           shared_ptr<ZLTextLineInfo> previousInfo
                                           ) ;
    
        void skip(shared_ptr<ZLTextPage> page, shared_ptr<ZLTextWordCursor >cursor, int unit, int size);
    void setModel(shared_ptr<ZDZLTextModel> model, int jumpParagraph);
    ZLTextView(ZLApplication* application);
    virtual void paint(shared_ptr<ZLPaintContext> context,int value);
    
    virtual ZLPaintContext::ColorAdjustingMode getAdjustingModeForImages()=0;
    virtual shared_ptr<ExtensionElementManager> getExtensionManager()=0;
    virtual void onScrollingFinished(int pageIndex);
    
    bool initSelection(int x, int y) ;
    shared_ptr<ZLTextSelection>  getSelection()
    {
        return mySelection;
    }
    shared_ptr<ZLTextSelection::Point> getSelectionCursorPoint(ZLTextPage& page, SelectionCursor::Which which) {
        if (which == SelectionCursor::None) {
            return 0;
        }
        
        if (which == mySelection->getCursorInMovement()) {
            return mySelection->getCursorInMovementPoint();
        }
    
        if (which == SelectionCursor::Left) {
            if (mySelection->hasPartBeforePage(page)) {
                return 0;
            }
            shared_ptr<ZLTextElementArea> area = mySelection->getStartArea(page);
            if (area != 0) {
                return new ZLTextSelection::Point(area->XStart, (area->YStart + area->YEnd) / 2);
            }
        } else {
            if (mySelection->hasPartAfterPage(page)) {
                return 0;
            }
            shared_ptr< ZLTextElementArea> area = mySelection->getEndArea(page);
            if (area != 0) {
                return new ZLTextSelection::Point(area->XEnd, (area->YStart + area->YEnd) / 2);
            }
        }
        return 0;
    }
    shared_ptr<ZLTextElementArea> getSelectionCursorAreaToDraw(ZLTextPage& page, SelectionCursor::Which which) {
        if (which == SelectionCursor::None) {
            return 0;
        }

        if (which == SelectionCursor::Left) {
            if (mySelection->hasPartBeforePage(page)) {
                return 0;
            }
            shared_ptr<ZLTextElementArea> area = mySelection->getStartArea(page);
            if (area != 0) {
                return area;
            }
        } else {
            if (mySelection->hasPartAfterPage(page)) {
                return 0;
            }
            shared_ptr< ZLTextElementArea> area = mySelection->getEndArea(page);
            if (area != 0) {
                return area;
            }
        }
        return 0;
    }
    
    
    float distance2ToCursor(int x, int y, SelectionCursor::Which which) ;
    SelectionCursor::Which findSelectionCursor(int x, int y) ;
    
    SelectionCursor::Which findSelectionCursor(int x, int y, float maxDistance2) ;
    void moveSelectionCursorTo(SelectionCursor::Which which, int x, int y);
   
    void outlineRegion( shared_ptr< ZLTextRegion> region) {
        outlineRegion(region != 0 ? region->getSoul() : 0);
    }
    
    void outlineRegion( shared_ptr< ZLTextRegion::Soul> soul) {
        myShowOutline = true;
        myOutlinedRegionSoul = soul;
    }
    //public synchronized final
    void turnPage(bool forward, int scrollingMode, int value);
    //protected synchronized
    void preparePaintInfo() ;
    SelectionCursor::Which getSelectionCursorInMovement() {
        return mySelection->getCursorInMovement();
    }
    shared_ptr< ZLTextRegion> getOutlinedRegion(ZLTextPage& page) {
        return page.TextElementMap->getRegion(myOutlinedRegionSoul);
    }
    
    shared_ptr< ZLTextRegion> getOutlinedRegion() {
        return getOutlinedRegion(*myCurrentPage);
    }
    void hideOutline() {
        myShowOutline = false;
        //Application.getViewWidget().reset();
    }
    virtual  void releaseSelectionCursor() ;
    bool isSelectionEmpty() {
        return mySelection->isEmpty();
    }
    shared_ptr<ZLTextPosition> getSelectionStartPosition() {
        return mySelection->getStartPosition();
    }
    
    shared_ptr<ZLTextPosition> getSelectionEndPosition() {
        return mySelection->getEndPosition();
    }
    shared_ptr<ZLTextParagraphCursor> cursor(int index) {
        return myCursorManager->get(index);
    }
    bool isPressedOnSelectedRegion(int x,int y);
    friend class ZLTextSelection;
    void rebuildPaintInfo() {
        
        if (myCursorManager != 0) {
            myCursorManager->evictAll();
        }
        
        if (myCurrentPage->PaintState != NOTHING_TO_PAINT) {
            myCurrentPage->LineInfos.clear();
            if (!myCurrentPage->StartCursor->isNull()) {
                myCurrentPage->StartCursor->rebuild();
                myCurrentPage->EndCursor->reset();
                myCurrentPage->PaintState = START_IS_KNOWN;
            } else if (!myCurrentPage->EndCursor->isNull()) {
                myCurrentPage->EndCursor->rebuild();
                myCurrentPage->StartCursor->reset();
                myCurrentPage->PaintState = END_IS_KNOWN;
            }
        }
        myPreviousPage->reset();
        myNextPage->reset();
        myLineInfoCache.clear();
    }
    void clearCaches() {
        resetMetrics();
        rebuildPaintInfo();
        //Application.getViewWidget().reset();
        myCharWidth = -1;
    }
    

    // opinion
    virtual int getOpinionCount(int pIndex);
    
    // calculate page count
    int calculatePageCount();
    
    // search
    int searchText(std::string text, long length, bool ignoreCase, bool wholeText, bool backward, bool thisSectionOnly);
    std::vector<shared_ptr<ZDZLTextMark>> getMarks();
    void gotoMark(shared_ptr<ZDZLTextMark> mark);
};
#endif /* ZLTextView_hpp */
