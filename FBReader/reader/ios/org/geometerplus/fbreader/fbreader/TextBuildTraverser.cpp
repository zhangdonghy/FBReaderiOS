//
//  TextBuildTraverser.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/10.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "TextBuildTraverser.hpp"

std::string TextBuildTraverser::getText() {
    std::string to = "";
    ZLUnicodeUtil::Ucs2String from;
    for (int i = 0; i < myBuffer.size(); i++){
        from.insert(from.end(), myBuffer[i]);
    }
    ZLUnicodeUtil::ucs2ToUtf8(to, from);
    return to;
}

