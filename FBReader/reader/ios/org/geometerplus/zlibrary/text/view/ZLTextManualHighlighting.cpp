//
//  ZLTextManualHighlighting.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/20.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ZLTextManualHighlighting.hpp"
#include "ZLTextView.hpp"

ZLTextManualHighlighting::ZLTextManualHighlighting (shared_ptr<ZLTextView> view,
                                                    shared_ptr<ZLTextPosition> start, shared_ptr<ZLTextPosition> end)
: ZLTextSimpleHighlighting(view, start, end) {
}

shared_ptr<ZLColor> ZLTextManualHighlighting::getForegroundColor() {
    return View->getHighlightingForegroundColor();
}

shared_ptr<ZLColor> ZLTextManualHighlighting::getBackgroundColor() {
    return View->getHighlightingBackgroundColor();
}

shared_ptr<ZLColor> ZLTextManualHighlighting::getOutlineColor() {
    return View->getOutlineColor();
}

shared_ptr<ZLColor> ZLTextManualHighlighting::getUnderlineColor() {
    return 0;
}

int ZLTextManualHighlighting::getNoteCount() {
    return 0;
}

int ZLTextManualHighlighting::getOpinionCount() {
    return 0;
}

