//
//  ZLTextSpecialElement.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/2.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ZLTextSpecialElement.hpp"

shared_ptr<ZLTextElement> ZLTextElement::HSpace = new ZLTextSpecialElement(ZLTextElement::HSPACE_ELEMENT);
shared_ptr<ZLTextElement> ZLTextElement::NBSpace = new ZLTextSpecialElement(ZLTextElement::NB_HSPACE_ELEMENT);
shared_ptr<ZLTextElement> ZLTextElement::AfterParagraph = new ZLTextSpecialElement(ZLTextElement::AFTER_PARAGRAPH_ELEMENT);
shared_ptr<ZLTextElement> ZLTextElement::Indent = new ZLTextSpecialElement(ZLTextElement::INDENT_ELEMENT);
shared_ptr<ZLTextElement> ZLTextElement::StyleClose = new ZLTextSpecialElement(ZLTextElement::STYLE_CLOSE_ELEMENT);

