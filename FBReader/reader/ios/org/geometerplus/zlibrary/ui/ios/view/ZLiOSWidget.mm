//
//  ZLiOSWidget.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/10.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ZLiOSWidget.h"

#include "ZLViewEnums_PageIndex.hpp"

ZLiOSWidget::ZLiOSWidget(ZDFBReaderInterface* model) {
    myModel = model;
}

ZLiOSWidget::~ZLiOSWidget() {
    NSLog(@"~ZLiOSWidget");
}

void ZLiOSWidget::reset() {
}

void ZLiOSWidget::repaint() {
    [myModel repaint];
}

void ZLiOSWidget::startManualScrolling(int x, int y, ZLViewEnums_PageIndex::Direction direction) {
}

void ZLiOSWidget::scrollManuallyTo(int x, int y) {
}

void ZLiOSWidget::startAnimatedScrolling(int pageIndex, int x, int y, ZLViewEnums_PageIndex::Direction direction, int speed) {
}

void ZLiOSWidget::startAnimatedScrolling(int pageIndex, ZLViewEnums_PageIndex::Direction direction, int speed) {
}

void ZLiOSWidget::startAnimatedScrolling(int x, int y, int speed) {
}

void ZLiOSWidget::setScreenBrightness(int percent) {
}

int ZLiOSWidget::getScreenBrightness() {
    return 100;
}

shared_ptr<ZLColor> ZLiOSWidget::selectingBgColor() {
    int red = 0;
    int green = 0;
    int blue = 0;
    [myModel getSelectingBgColorWithRed:&red green:&green blue:&blue];
    return new ZLColor(red, green, blue);
}

shared_ptr<ZLColor> ZLiOSWidget::selectingCursorColor() {
    int red = 0;
    int green = 0;
    int blue = 0;
    [myModel getSelectingCursorColorWithRed:&red green:&green blue:&blue];
    return new ZLColor(red, green, blue);
}

shared_ptr<ZLColor> ZLiOSWidget::underlineColor(int pStart, int eStart, int cStart, int pEnd, int eEnd, int cEnd) {
    int red = 0;
    int green = 0;
    int blue = 0;
    [myModel getUnderlineColorWithRed:&red green:&green blue:&blue
                   fromStartParagraph:pStart Element:eStart Character:cStart
                         endParagraph:pEnd Element:eEnd Character:cEnd];
    return new ZLColor(red, green, blue);
}

int ZLiOSWidget::countNote(int pStart, int eStart, int cStart, int pEnd, int eEnd, int cEnd) {
    ZDEPubLocation* startLoc = [[ZDEPubLocation alloc] init];
    startLoc.bookType = ZD_BOOK_TYPE_EPUB;
    startLoc.pIndex = pStart;
    startLoc.eIndex = eStart;
    startLoc.cIndex = cStart;
    ZDEPubLocation* endLoc = [[ZDEPubLocation alloc] init];
    endLoc.bookType = ZD_BOOK_TYPE_EPUB;
    endLoc.pIndex = pEnd;
    endLoc.eIndex = eEnd;
    endLoc.cIndex = cEnd;

    return (int)[myModel countNoteBetweenStart:startLoc toEnd:endLoc];
}

int ZLiOSWidget::countOpinionInParagraph(int pIndex) {
    return [myModel countOpinionInParagraph:pIndex];
}

