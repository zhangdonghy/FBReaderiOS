//
//  ZLOpinionHighlighting.cpp
//  ZDLibrary
//
//  Created by 蒋晨霄 on 2017/3/7.
//  Copyright © 2017年 蒋晨霄. All rights reserved.
//

#include "ZLOpinionHighlighting.hpp"

#include "ZLTextView.hpp"

ZLOpinionHighlighting::ZLOpinionHighlighting(shared_ptr<ZLTextView> view,
                                             shared_ptr<ZLTextPosition> start,
                                             shared_ptr<ZLTextPosition> end)
: ZLTextManualHighlighting(view, start, end) {
}

bool ZLOpinionHighlighting::isOpinion() {
    return true;
}
