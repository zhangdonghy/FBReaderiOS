//
//  ZLPaintContext.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/31.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ZLPaintContext.hpp"


ZLPaintContext::ZLPaintContext(shared_ptr<SystemInfo> systemInfo) {
    mySystemInfo = systemInfo;
}

void ZLPaintContext::setFillColor(shared_ptr<ZLColor> color) {
    setFillColor(color, 0xFF);
}

void ZLPaintContext::setFont(std::string fontName, int size, bool bold, bool italic, bool underline, bool strikeThrough) {
//    if (entries.size != 0 && !entries.equals(myFontEntries)) {
//        myFontEntries = entries;
//        myResetFont = true;
//    }
    if (strcmp(myFontName.c_str(), fontName.c_str()) != 0) {
        myFontName = fontName;
        myResetFont = true;
    }
    if (myFontSize != size) {
        myFontSize = size;
        myResetFont = true;
    }
    if (myFontIsBold != bold) {
        myFontIsBold = bold;
        myResetFont = true;
    }
    if (myFontIsItalic != italic) {
        myFontIsItalic = italic;
        myResetFont = true;
    }
    if (myFontIsUnderlined != underline) {
        myFontIsUnderlined = underline;
        myResetFont = true;
    }
    if (myFontIsStrikedThrough != strikeThrough) {
        myFontIsStrikedThrough = strikeThrough;
        myResetFont = true;
    }
    if (myResetFont) {
        myResetFont = false;
        setFontInternal(myFontName, size, bold, italic, underline, strikeThrough);
        mySpaceWidth = -1;
        myStringHeight = -1;
        myDescent = -1;
        myCharHeights.clear();
    }
}

#pragma mark - string
int ZLPaintContext::getStringWidth(std::string string1) {
    return getStringWidth((const char*)string1.c_str(), 0, (int)string1.length());
}

int ZLPaintContext::getSpaceWidth() {
    int spaceWidth = mySpaceWidth;
    if (spaceWidth == -1) {
        spaceWidth = getSpaceWidthInternal();
        mySpaceWidth = spaceWidth;
    }
    return spaceWidth;
}

int ZLPaintContext::getStringHeight() {
    int stringHeight = myStringHeight;
    if (stringHeight == -1) {
        stringHeight = getStringHeightInternal();
        myStringHeight = stringHeight;
    }
    return stringHeight;
}

int ZLPaintContext::getCharHeight(char chr) {
    for (std::map<char, int>::const_iterator it = myCharHeights.begin(); it != myCharHeights.end(); ++it) {
        if (it->first == chr) {
            return it->second;
        }
    }
    int he = getCharHeightInternal(chr);
    myCharHeights.insert(std::make_pair(chr, he));
    return myCharHeights[chr];
}

int ZLPaintContext::getDescent() {
    int descent = myDescent;
    if (descent == -1) {
        descent = getDescentInternal();
        myDescent = descent;
    }
    return descent;
}

void ZLPaintContext::drawString(int x, int y, std::string string2) {
    drawString(x, y, (const char*)string2.c_str(), 0, (int)string2.length());
}

