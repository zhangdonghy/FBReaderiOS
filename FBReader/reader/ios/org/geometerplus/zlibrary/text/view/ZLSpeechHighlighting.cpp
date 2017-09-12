//
//  ZLSpeechHighlighting.cpp
//  ZDLibrary
//
//  Created by 蒋晨霄 on 2017/3/3.
//  Copyright © 2017年 蒋晨霄. All rights reserved.
//

#include "ZLSpeechHighlighting.hpp"

#include "ZLTextView.hpp"

ZLSpeechHighlighting::ZLSpeechHighlighting(shared_ptr<ZLTextView> view,
                                           shared_ptr<ZLTextPosition> start,
                                           shared_ptr<ZLTextPosition> end)
: ZLTextManualHighlighting(view, start, end) {
}

shared_ptr<ZLColor> ZLSpeechHighlighting::getForegroundColor() {
    return View->getHighlightingForegroundColor();
}

shared_ptr<ZLColor> ZLSpeechHighlighting::getBackgroundColor() {
    return View->getHighlightingBackgroundColor();
}

shared_ptr<ZLColor> ZLSpeechHighlighting::getOutlineColor() {
    return View->getOutlineColor();
}

shared_ptr<ZLColor> ZLSpeechHighlighting::getUnderlineColor() {
    return 0;
}

bool ZLSpeechHighlighting::isSpeech() {
    return true;
}

