//
//  ZLTextHyperlinkControlElement.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/5.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLTextHyperlinkControlElement_hpp
#define ZLTextHyperlinkControlElement_hpp

#include <stdio.h>
#include <string>
#include "ZLTextControlElement.hpp"
#include "ZLTextHyperlink.hpp"

class ZLTextHyperlink;
class ZLTextHyperlinkControlElement : public  ZLTextControlElement{
    public:
    shared_ptr<ZLTextHyperlink> Hyperlink;
    
    ZLTextHyperlinkControlElement(unsigned char kind, unsigned char type, std::string id)
    :ZLTextControlElement(kind,true)
    {
        Hyperlink = new ZLTextHyperlink(type, id);
    }
    virtual ZLTextElement::Kind kind() const
    {
        return  HYPER_LINK_ELEMENT;
    }
    
    
};
#endif /* ZLTextHyperlinkControlElement_hpp */
