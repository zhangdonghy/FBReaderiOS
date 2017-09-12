//
//  FBReaderApp.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/31.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef FBReaderApp_hpp
#define FBReaderApp_hpp

#include "ZLApplication.hpp"
#include "shared_ptr.h"
#include "FBView.hpp"
#include "ECatalogTree.hpp"

#include "ZDZLSearchResult.hpp"

class ZDBookModel;

class BookNoteContext {
public:
    int ParagraphIndex;
    int ElementIndex;
    int CharIndex;
    int ParagraphEndIndex;
    int ElementEndIndex;
    int CharEndIndex;

    int StartY;
    int StartX;
    int EndY;
    int EndX;
};

class BookNote {
public:
    int ParagraphIndex;
    int ElementIndex;
    int CharIndex;
    
    int ParagraphEndIndex;
    int ElementEndIndex;
    int CharEndIndex;

    bool hasDesc;
};

class FBReaderApp: public ZLApplication {
private:
    ZDBookModel* zdBookModel;
    bool aIsEnglishBook;

public:
    shared_ptr<ZLTextView> BookTextView;
    FBReaderApp();
    ~FBReaderApp();

    bool IsEnglishBook() {
        return aIsEnglishBook;
    }
    bool openBook(const char* bookpath, int length);

    void getLastElementOfParagraph(int pIndex, int* eIndex, int* cIndex);

    ZDBookModel* getModel() {
        return zdBookModel;
    }

    std::string getSelectStr();
    void clearSelectText();

    ECatalogTree* getECatalogTree();

    void getSelectionArea(int* startX, int* startY,int* endX,int* endY);

    bool isPressedOnCursor(int x,int y);
    bool isPressedOnSelectedRegion(int x,int y);
    void getSelectionLocation(int* startPIndex, int* startEIndex, int* startCIndex,
                              int* endPIndex, int* endEIndex, int* endCIndex,
                              int* percent, std::string* word);
    
    //// get current page start context
    void getPageStart(int* ParagraphIndex,int* ElementIndex,int* CharIndex,int* Percent,std::string* word);
    void jumpToLocation(int ParagraphIndex,int ElementIndex,int CharIndex);
    bool isInCurPage(int curPIndex, int curEIndex, int curCIndex);
    
    void changeBackgroundColor(int r, int g, int b);
    void changeTextColor(int r, int g, int b);
    
    //bool checkHightlighting(int x, int y);
    void BeginRead();
    void ExitRead();
    shared_ptr<ZLTextWordCursor> myReadStart;
    bool NextParagraph(std::string* outWord);
    bool hasNextPageInReading(int curPageIndex);
    void moveReadStartToParagraph(int pIndex);
    
    bool setFontSize(int fontSize);
    bool setFontName(std::string fontName);
    bool setLineSpace(int lineSpace);

    bool hasPrevPage();
    bool hasNextPage();
    
    long curPercent();
    void jumpToPercent(long percent);
    shared_ptr<PagePosition> pagePosition();

    void onScrollingFinished(int pageIndex) {
        BookTextView->onScrollingFinished(pageIndex);
    };
    bool onFingerLongPress(int x, int y) {
        return BookTextView->onFingerLongPress(x, y);
    };
    void onFingerReleaseAfterLongPress(int x, int y) {
        BookTextView->onFingerReleaseAfterLongPress(x, y);
    };
    void onFingerMove(int x, int y) {
        BookTextView->onFingerMove(x, y);
    };
    void onFingerPress(int x, int y) {
        BookTextView->onFingerPress(x, y);
    };
    void onFingerRelease(int x, int y) {
        BookTextView->onFingerRelease(x, y);
    };
    bool onFingerSingleTap(int x, int y) {
        return BookTextView->onFingerSingleTap(x, y);
    };
    int getCountOfSelectedWords() {
        return ((FBView&)(*BookTextView)).getCountOfSelectedWords();
    };
    int paragraphIndexOfPos(int x, int y) {
        return ((FBView&)(*BookTextView)).paragraphIndexOfPos(x, y);
    };
    
    // calculate
    int calculatePageCount() {
        return ((FBView&)(*BookTextView)).calculatePageCount();
    };

    // search
    ZDZLSearchResult* searchText(std::string text, long length, bool ignoreCase, bool wholeText, bool backward,
                                 bool thisSectionOnly);

    // selecting
    shared_ptr<ZLColor> selectingBgColor();
    shared_ptr<ZLColor> selectingCursorColor();

    // lined text
    shared_ptr<ZLColor> underlineColor(shared_ptr<ZLTextPosition> startPos, shared_ptr<ZLTextPosition> endPos);
    void addUnderline(BookNote bn);
    void removeUnderline(BookNote bn);

    // note
    int getNoteCount(shared_ptr<ZLTextPosition> start, shared_ptr<ZLTextPosition> end);

    // opinion
    void addOpinion(BookNote opinion);
    void removeOpinion(BookNote opinion);
    int getOpinionCount(int pIndex);
    

protected:
};


#endif /* FBReaderApp_hpp */
