//
//  ZDZLSearchResult.cpp
//  ZDStudent
//
//  Created by 蒋晨霄 on 2016/12/27.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ZDZLSearchResult.hpp"

ZDZLSearchParagraph::ZDZLSearchParagraph(int paragraphIndex, std::string paragraphText) {
    pIndex = paragraphIndex;
    pText = paragraphText;
}

bool ZDZLSearchResult::isParagraphExisted(int pIndex) {
    for (std::vector<shared_ptr<ZDZLSearchParagraph>>::iterator it = paragraphs.begin(); it != paragraphs.end(); it++) {
        shared_ptr<ZDZLSearchParagraph> paragraph = *it;
        if (paragraph->pIndex == pIndex) {
            return true;
        }
    }
    return false;
}

void ZDZLSearchResult::addParagraph(int pIndex, std::string pText) {
    shared_ptr<ZDZLSearchParagraph> paragraph = new ZDZLSearchParagraph(pIndex, pText);
    paragraphs.push_back(paragraph);
}

