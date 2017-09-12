//
//  ZLTextSelection.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/8.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLTextSelection_hpp
#define ZLTextSelection_hpp

#include <stdio.h>
#include "ZLTextHighlighting.hpp"
#include "ZLTextRegion.hpp"
#include "SelectionCursor.hpp"
#include "ZLTextElementAreaVector.hpp"

class ZLTextView;
class ZLTextSelection : public ZLTextHighlighting {
public:
    class Point {
    public:
        int X;
        int Y;
        ~Point() {
        }
        Point(int x, int y) {
            X = x;
            Y = y;
        }
    };

private:
    class Scroller /*implements Runnable*/ {
    private:
        ZLTextPage myPage;
        bool myScrollForward;
        int myX, myY;
        shared_ptr<ZLTextView> myView;
        ZLTextSelection* mySelection;

    public:
        Scroller(ZLTextSelection* selection,shared_ptr<ZLTextView> view,ZLTextPage& page, bool forward, int x, int y) {
            myPage = page;
            myScrollForward = forward;
            setXY(x, y);
            myView =view;
            mySelection = selection;
            //myView.Application.addTimerTask(this, 400);
        }
        
        bool scrollsForward() {
            return myScrollForward;
        }
        
        void setXY(int x, int y) {
            myX = x;
            myY = y;
        }
        
        void run() ;
        
        void stop() {
            //myView.Application.removeTimerTask(this);
        }
    };
    
    
    shared_ptr<ZLTextView> myView;
    shared_ptr<ZLTextRegion::Soul> myLeftMostRegionSoul;
    shared_ptr<ZLTextRegion::Soul> myRightMostRegionSoul;
    SelectionCursor::Which myCursorInMovement = SelectionCursor::None;
    shared_ptr<Point> myCursorInMovementPoint ;//= new Point(-1, -1);
    shared_ptr<Scroller> myScroller;

public:
    ZLTextSelection(shared_ptr<ZLTextView>  view);
    ~ZLTextSelection();
    virtual bool isEmpty();
    virtual shared_ptr<ZLTextPosition> getStartPosition();
    virtual shared_ptr<ZLTextPosition> getEndPosition();
    virtual shared_ptr<ZLTextElementArea> getStartArea(ZLTextPage& page);
    virtual shared_ptr<ZLTextElementArea> getEndArea(ZLTextPage& page);
    virtual shared_ptr<ZLColor> getForegroundColor();
    virtual shared_ptr<ZLColor> getBackgroundColor();
    virtual shared_ptr<ZLColor> getOutlineColor();
    virtual shared_ptr<ZLColor> getUnderlineColor();
    virtual int getNoteCount();
    virtual int getOpinionCount();
    virtual bool isOpinion() {
//        printf("ZLTextSelection isOpinion\n");
        return false;
    };
    virtual bool isSpeech() {
//        printf("ZLTextSelection isSpeech\n");
        return false;
    };
    virtual bool isUnderline() {
//        printf("ZLTextSelection isUnderline\n");
        return false;
    };

    bool clear();
    
    bool start(int x, int y) ;
    SelectionCursor::Which getCursorInMovement() {
        return myCursorInMovement;
    }
    shared_ptr<Point> getCursorInMovementPoint() {
        return myCursorInMovementPoint;
    }
    bool hasPartBeforePage(ZLTextPage& page) {
        if (isEmpty()) {
            return false;
        }
        shared_ptr< ZLTextElementArea> firstPageArea = page.TextElementMap->getFirstArea();
        if (firstPageArea == 0) {
            return false;
        }
        int cmp = myLeftMostRegionSoul->compareTo(firstPageArea);
        return cmp < 0 || (cmp == 0 && !firstPageArea->isFirstInElement());
    }
    bool hasPartAfterPage(ZLTextPage& page) {
        if (isEmpty()) {
            return false;
        }
        shared_ptr< ZLTextElementArea> lastPageArea = page.TextElementMap->getLastArea();
        if (lastPageArea == 0) {
            return false;
        }
        int cmp = myRightMostRegionSoul->compareTo(lastPageArea);
        return cmp > 0 || (cmp == 0 && !lastPageArea->isLastInElement());
    }
    void setCursorInMovement(SelectionCursor::Which which, int x, int y);
    void expandTo(ZLTextPage& page, int x, int y);
    void stop();
};

#endif /* ZLTextSelection_hpp */
