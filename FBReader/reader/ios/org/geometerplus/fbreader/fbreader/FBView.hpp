//
//  FBView.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/31.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef FBView_hpp
#define FBView_hpp

#include <stdio.h>
#include "shared_ptr.h"

#include "ZLTextView.hpp"

class FBReaderApp;
class BookNoteContext;
class TextSnippet;
class TextBuildTraverser;
class FBView: public ZLTextView {
private:
    FBReaderApp* myReader;
    shared_ptr<ZLColor> backgroundColor;
    shared_ptr<ZLColor> textColor;
    shared_ptr<ZLTextStyleCollection> myTextStyleColloction;

public:
    FBView(FBReaderApp* reader);

    // touch handler
    virtual void onFingerPress(int x, int y);
    virtual void onFingerRelease(int x, int y);
    virtual void onFingerMove(int x, int y);
    virtual bool onFingerLongPress(int x, int y);
    virtual void onFingerReleaseAfterLongPress(int x, int y);
    virtual void onFingerMoveAfterLongPress(int x, int y);
    virtual bool onFingerSingleTap(int x, int y);
    virtual void onFingerDoubleTap(int x, int y);
    virtual void onFingerEventCancelled();
    
    virtual bool isScrollbarShown();
    virtual int getScrollbarFullSize();

    virtual shared_ptr<ZLColor> getBackgroundColor();
    virtual void setBackgroundColor(int r, int g, int b);

    virtual shared_ptr<ZLTextStyleCollection> getTextStyleCollection();
    
    virtual ZLTextViewBase_ImageFitting getImageFitting();
    
    virtual int getLeftMargin();
    virtual int getRightMargin();
    virtual int getTopMargin();
    virtual int getBottomMargin();
    virtual int getSpaceBetweenColumns();

    virtual bool twoColumnView();

    virtual shared_ptr<ZLFile> getWallpaperFile();
    virtual ZLPaintContext_FillMode getFillMode();
    
    virtual ZLPaintContext::ColorAdjustingMode getAdjustingModeForImages();
    virtual shared_ptr<ExtensionElementManager> getExtensionManager();
    
    virtual shared_ptr<ZLColor> getSelectionBackgroundColor();
    virtual shared_ptr<ZLColor> getSelectionCursorColor();
    virtual shared_ptr<ZLColor> getSelectionForegroundColor();
    virtual shared_ptr<ZLColor> getHighlightingBackgroundColor();
    virtual shared_ptr<ZLColor> getHighlightingForegroundColor();
    virtual shared_ptr<ZLColor> getOutlineColor();
    virtual shared_ptr<ZLColor> getUnderlineColor(shared_ptr<ZLTextPosition> start, shared_ptr<ZLTextPosition> end);
    virtual shared_ptr<ZLColor> getTextColor(shared_ptr<ZLTextHyperlink> hyperlink);
    virtual void setTextColor(int r, int g, int b);
    
    // moved from ViewOptions zhangdong
    virtual void onScrollingFinished(int pageIndex);
    virtual void releaseSelectionCursor();

    int getCountOfSelectedWords();
    shared_ptr<TextSnippet> getSelectedSnippet();
    
    std::string textInParagraph(int pIndex);
    int paragraphIndexOfPos(int x, int y);

    // lined text
    virtual bool isTapOnLinedText(int x, int y);
    BookNoteContext createBookNoteContext(int pStart, int eStart, int cStart, int xStart, int yStart,
                                          int pEnd, int eEnd, int cEnd, int xEnd, int yEnd);

    // note
    virtual int getNoteCount(shared_ptr<ZLTextPosition> start, shared_ptr<ZLTextPosition> end);
    
    // opinion
    virtual int getOpinionCount(int pIndex);
    virtual bool isTapOnOpinion(int x, int y);
};

#endif /* FBView_hpp */

