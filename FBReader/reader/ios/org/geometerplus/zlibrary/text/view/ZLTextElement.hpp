//
//  ZLTextElement.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/1.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLTextElement_hpp
#define ZLTextElement_hpp

#include <stdio.h>
#include "shared_ptr.h"

class ZLTextElement
{
public:
   
    enum Kind {
        UNKOWN_ELEMENT,
        
        WORD_ELEMENT,
        
        IMAGE_ELEMENT,
        
        CONTROL_ELEMENT,
        
        FORCED_CONTROL_ELEMENT,
        
        INDENT_ELEMENT,
        
        HSPACE_ELEMENT,
        
        NB_HSPACE_ELEMENT,
        
        FIXED_HSPACE_ELEMENT,
        
        BEFORE_PARAGRAPH_ELEMENT,
        
        AFTER_PARAGRAPH_ELEMENT,
        
        EMPTY_LINE_ELEMENT,
        
        START_REVERSED_SEQUENCE_ELEMENT,
        
        END_REVERSED_SEQUENCE_ELEMENT,
        STYLE_CLOSE_ELEMENT,
        VIDEO_ELEMENT,
        EXTENSION_ELEMENT,
        TEXT_CONTROL_ELEMENT,
        TEXT_STYLE_ELEMENT,
        HYPER_LINK_ELEMENT,
        
    };
    //ZLTextHyperlinkControlElement
    ~ZLTextElement();
    static shared_ptr<ZLTextElement> HSpace ;
    static shared_ptr<ZLTextElement> NBSpace;
    static shared_ptr<ZLTextElement> AfterParagraph ;
    static shared_ptr<ZLTextElement> Indent ;
    static shared_ptr<ZLTextElement> StyleClose ;
    
    virtual Kind kind() const =0;
};
#endif /* ZLTextElement_hpp */
