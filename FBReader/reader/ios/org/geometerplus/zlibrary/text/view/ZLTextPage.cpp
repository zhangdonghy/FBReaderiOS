//
//  ZLTextPage.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/31.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ZLTextPage.hpp"


ZLTextPage::ZLTextPage()
{
    TextElementMap = new ZLTextElementAreaVector();
    StartCursor = new ZLTextWordCursor();
    EndCursor = new ZLTextWordCursor();
}
