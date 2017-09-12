//
//  ZLUnderlineHighlighting.cpp
//  ZDLibrary
//
//  Created by 蒋晨霄 on 2017/3/3.
//  Copyright © 2017年 蒋晨霄. All rights reserved.
//

#include "ZLUnderlineHighlighting.hpp"

#include "ZLTextView.hpp"

ZLUnderlineHighlighting::ZLUnderlineHighlighting(shared_ptr<ZLTextView> view,
                                                 shared_ptr<ZLTextPosition> start,
                                                 shared_ptr<ZLTextPosition> end)
: ZLTextManualHighlighting(view, start, end) {
}

shared_ptr<ZLColor> ZLUnderlineHighlighting::getForegroundColor() {
    return View->getHighlightingForegroundColor();
}

shared_ptr<ZLColor> ZLUnderlineHighlighting::getBackgroundColor() {
    return View->getHighlightingBackgroundColor();
}

shared_ptr<ZLColor> ZLUnderlineHighlighting::getOutlineColor() {
    return View->getOutlineColor();
}

shared_ptr<ZLColor> ZLUnderlineHighlighting::getUnderlineColor() {
    return View->getUnderlineColor(getStartPosition(), getEndPosition());
}

int ZLUnderlineHighlighting::getNoteCount() {
    return View->getNoteCount(getStartPosition(), getEndPosition());
}

bool ZLUnderlineHighlighting::isUnderline() {
    return true;
}

