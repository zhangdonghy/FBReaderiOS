//
//  ZLTextSelection.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/8.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ZLTextSelection.hpp"
#include "ZLTextView.hpp"
#include "ZLTextFixedPosition.hpp"

ZLTextSelection::ZLTextSelection(shared_ptr<ZLTextView>  view)
{
    myCursorInMovementPoint = new Point(-1,-1);
    myView = view;
}
ZLTextSelection::~ZLTextSelection()
{
    
}
bool ZLTextSelection::isEmpty()
{
    return myLeftMostRegionSoul == 0;
}
shared_ptr<ZLTextPosition>
ZLTextSelection::getStartPosition()
{
    if (isEmpty()) {
        return 0;
    }
    return new ZLTextFixedPosition(
                                   myLeftMostRegionSoul->ParagraphIndex,
                                   myLeftMostRegionSoul->StartElementIndex,
                                   0
                                   );
}
shared_ptr<ZLTextPosition>
ZLTextSelection::getEndPosition()
{
    if (isEmpty()) {
        return 0;
    }
    shared_ptr<ZLTextParagraphCursor> cursor = myView->cursor(myRightMostRegionSoul->ParagraphIndex);
    shared_ptr<ZLTextElement> element = cursor->getElement(myRightMostRegionSoul->EndElementIndex);
    return new ZLTextFixedPosition(
                                   myRightMostRegionSoul->ParagraphIndex,
                                   myRightMostRegionSoul->EndElementIndex,
                                   element->kind() == ZLTextElement::WORD_ELEMENT?
                                   ((ZLTextWord&)*element).Length : 0
                                   );
}
shared_ptr<ZLTextElementArea>
ZLTextSelection::getStartArea(ZLTextPage& page)
{
    if (isEmpty()) {
        return 0;
    }
    shared_ptr<ZLTextElementAreaVector> vector = page.TextElementMap;
   shared_ptr<ZLTextRegion> region = vector->getRegion(myLeftMostRegionSoul);
    if (region != 0) {
        return region->getFirstArea();
    }
    shared_ptr<ZLTextElementArea> firstArea = vector->getFirstArea();
    if (firstArea != 0 && myLeftMostRegionSoul->compareTo(firstArea) <= 0) {
        return firstArea;
    }
    return 0;
}
shared_ptr<ZLTextElementArea>
ZLTextSelection::getEndArea(ZLTextPage& page)
{
    if (isEmpty()) {
        return 0;
    }
    shared_ptr< ZLTextElementAreaVector> vector = page.TextElementMap;
    shared_ptr< ZLTextRegion> region = vector->getRegion(myRightMostRegionSoul);
    if (region != 0) {
        return region->getLastArea();
    }
    shared_ptr<ZLTextElementArea> lastArea = vector->getLastArea();
    if (lastArea != 0 && myRightMostRegionSoul->compareTo(lastArea) >= 0) {
        return lastArea;
    }
    return 0;
}

shared_ptr<ZLColor> ZLTextSelection::getForegroundColor() {
    return myView->getSelectionForegroundColor();
}

shared_ptr<ZLColor> ZLTextSelection::getBackgroundColor() {
   return myView->getSelectionBackgroundColor();
}

shared_ptr<ZLColor> ZLTextSelection::getOutlineColor() {
   return myView->getOutlineColor();
}

shared_ptr<ZLColor> ZLTextSelection::getUnderlineColor() {
    return 0;
}

int ZLTextSelection::getNoteCount() {
    return 0;
}

int ZLTextSelection::getOpinionCount() {
    return 0;
}

bool
ZLTextSelection::clear()
{
    if (isEmpty()) {
        return false;
    }
    
    stop();
    myLeftMostRegionSoul = 0;
    myRightMostRegionSoul = 0;
    myCursorInMovement = SelectionCursor::None;
    
    return true;
}
bool
ZLTextSelection::start(int x, int y) {
    clear();
    
    shared_ptr<ZLTextRegion> region = myView->findRegion(
                                                         x, y, (*myView).maxSelectionDistance(), *ZLTextRegion::anyFiler
                                                  );
    if (region == 0) {
        return false;
    }
    
    myRightMostRegionSoul = myLeftMostRegionSoul = region->getSoul();
    return true;
}
void ZLTextSelection::setCursorInMovement(SelectionCursor::Which which, int x, int y) {
    myCursorInMovement = which;
    myCursorInMovementPoint->X = x;
    myCursorInMovementPoint->Y = y;
}
void ZLTextSelection::stop() {
    myCursorInMovement = SelectionCursor::None;
    if (myScroller != 0) {
        myScroller->stop();
        myScroller = 0;
    }
}
void ZLTextSelection::expandTo(ZLTextPage& page, int x, int y)
{
    if (isEmpty()) {
        return;
    }
    
    shared_ptr<ZLTextElementAreaVector> vector = page.TextElementMap;
    shared_ptr<ZLTextElementArea> firstArea = vector->getFirstArea();
    shared_ptr<ZLTextElementArea> lastArea = vector->getLastArea();
    if (firstArea != 0 && y < firstArea->YStart) {
        if (myScroller != 0 && myScroller->scrollsForward()) {
            myScroller->stop();
            myScroller = 0;
        }
        if (myScroller == 0) {
            myScroller = new Scroller(this,myView,page, false, x, y);
            return;
        }
        //} else if (lastArea != null && y + ZLTextSelectionCursor.getHeight() / 2 + ZLTextSelectionCursor.getAccent() / 2 > lastArea.YEnd) {
    } else if (lastArea != 0 && y > lastArea->YEnd) {
        if (myScroller != 0 && !myScroller->scrollsForward()) {
            myScroller->stop();
            myScroller = 0;
        }
        if (myScroller == 0) {
            myScroller = new Scroller(this,myView,page, true, x, y);
            return;
        }
    } else {
        if (myScroller != 0) {
            myScroller->stop();
            myScroller = 0;
        }
    }
    
    if (myScroller != 0) {
        myScroller->setXY(x, y);
    }
    
    shared_ptr<ZLTextRegion> region = myView->findRegion(x, y, myView->maxSelectionDistance(), *ZLTextRegion::anyFiler);
    if (region == 0) {
        shared_ptr<ZLTextElementAreaVector::RegionPair> pair =
        myView->findRegionsPair(x, y, *ZLTextRegion::anyFiler);
        if (pair->Before != 0 || pair->After != 0) {
            shared_ptr<ZLTextRegion::Soul> base =
            myCursorInMovement == SelectionCursor::Right
            ? myLeftMostRegionSoul : myRightMostRegionSoul;
            if (pair->Before != 0) {
                if (base->compareTo(pair->Before->getSoul()) <= 0) {
                    region = pair->Before;
                } else {
                    region = pair->After;
                }
            } else {
                if (base->compareTo(pair->After->getSoul()) >= 0) {
                    region = pair->After;
                } else {
                    region = pair->Before;
                }
            }
        }
    }
    if (region == 0) {
        return;
    }
    
    shared_ptr<ZLTextRegion::Soul> soul = region->getSoul();
    if (myCursorInMovement == SelectionCursor::Right) {
        if (myLeftMostRegionSoul->compareTo(soul) <= 0) {
            myRightMostRegionSoul = soul;
        } else {
            myRightMostRegionSoul = myLeftMostRegionSoul;
            myLeftMostRegionSoul = soul;
            myCursorInMovement = SelectionCursor::Left;
        }
    } else {
        if (myRightMostRegionSoul->compareTo(soul) >= 0) {
            myLeftMostRegionSoul = soul;
        } else {
            myLeftMostRegionSoul = myRightMostRegionSoul;
            myRightMostRegionSoul = soul;
            myCursorInMovement = SelectionCursor::Right;
        }
    }
    
    if (myCursorInMovement == SelectionCursor::Right) {
        if (hasPartAfterPage(page)) {
            myView->turnPage(true, SCROLL_LINES, 1);
            //myView.Application.getViewWidget().reset();
            myView->preparePaintInfo();
        }
    } else {
        if (hasPartBeforePage(page)) {
            myView->turnPage(false, SCROLL_LINES, 1);
            //myView.Application.getViewWidget().reset();
            myView->preparePaintInfo();
        }
    }
}
void ZLTextSelection::Scroller::run() {
    myView->turnPage(myScrollForward, SCROLL_LINES, 1);
    myView->preparePaintInfo();
    mySelection->expandTo(myPage, myX, myY);
    //myView.Application.getViewWidget().reset();
    //myView.Application.getViewWidget().repaint();
}
