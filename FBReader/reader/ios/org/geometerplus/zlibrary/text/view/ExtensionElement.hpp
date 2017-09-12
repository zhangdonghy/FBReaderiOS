//
//  ExtensionElement.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/1.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ExtensionElement_hpp
#define ExtensionElement_hpp

#include <stdio.h>
#include "ZLTextElement.hpp"
//class ZLTextElement;
#include "ZLTextElementArea.hpp"
#include "ZLPaintContext.hpp"

class ZLTextViewBase;
class ExtensionElement : public ZLTextElement
{
protected:
    virtual void draw(shared_ptr<ZLPaintContext> context,shared_ptr<ZLTextElementArea> area) const=0 ;
    virtual Kind kind() const;
public:
    // protected
    virtual int getWidth()=0;
    virtual int getHeight()=0;
public:
    friend class ZLTextViewBase;
    friend class ZLTextView;
};
#endif /* ExtensionElement_hpp */
