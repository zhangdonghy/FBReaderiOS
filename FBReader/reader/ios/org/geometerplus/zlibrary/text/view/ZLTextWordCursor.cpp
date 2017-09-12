//
//  ZLTextWordCursor.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/1.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ZLTextWordCursor.hpp"


ZLTextWordCursor::ZLTextWordCursor()
{
}
ZLTextWordCursor::ZLTextWordCursor(shared_ptr<ZLTextWordCursor> cursor) {
    setCursor(*cursor);
}

void
ZLTextWordCursor::setCursor(const ZLTextWordCursor& cursor) {
    myParagraphCursor = cursor.myParagraphCursor;
    myElementIndex = cursor.myElementIndex;
    myCharIndex = cursor.myCharIndex;
}
ZLTextWordCursor::ZLTextWordCursor(shared_ptr<ZLTextParagraphCursor> cursor) {
    setCursor(cursor);
}

void
ZLTextWordCursor::setCursor(shared_ptr<ZLTextParagraphCursor> cursor) {
    myParagraphCursor = cursor;
    myElementIndex = 0;
    myCharIndex = 0;
}
ZLTextWordCursor::ZLTextWordCursor(shared_ptr<ZLTextParagraphCursor> cursor,int elementIndex,int charIndex)
{
    setCursor(cursor);
    myElementIndex = elementIndex;
    myCharIndex = charIndex;
}