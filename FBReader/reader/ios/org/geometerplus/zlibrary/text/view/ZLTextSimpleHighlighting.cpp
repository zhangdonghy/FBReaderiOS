//
//  ZLTextSimpleHighlighting.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/20.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ZLTextSimpleHighlighting.hpp"


ZLTextSimpleHighlighting::ZLTextSimpleHighlighting(
         shared_ptr<ZLTextView> view,shared_ptr<ZLTextPosition> start,shared_ptr<ZLTextPosition> end)
{
    View = view;
    myStartPosition = start;
    myEndPosition = end;
}

shared_ptr<ZLTextPosition>
ZLTextSimpleHighlighting::getStartPosition()
{
    return  myStartPosition;
}
shared_ptr<ZLTextPosition>
ZLTextSimpleHighlighting::getEndPosition()
{
    return myEndPosition;
}
shared_ptr<ZLTextElementArea>
ZLTextSimpleHighlighting::getStartArea(ZLTextPage& page)
{
    return page.TextElementMap->getFirstAfter(myStartPosition);
}
shared_ptr<ZLTextElementArea>
ZLTextSimpleHighlighting::getEndArea(ZLTextPage& page)
{
    return page.TextElementMap->getLastBefore(myEndPosition);
}