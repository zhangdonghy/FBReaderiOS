//
//  ZLTextParagraphImpl.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/2.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLTextParagraphImpl_hpp
#define ZLTextParagraphImpl_hpp

#include <stdio.h>
#include "shared_ptr.h"
#include "ZDZLTextParagraph.hpp"
#include "ZDZLTextPlainModel.hpp"

//class ZDZLTextPlainModel;

class ZLTextParagraphImpl :public ZDZLTextParagraph
{
private:
    shared_ptr<ZDZLTextPlainModel>  myModel;
    int myIndex;
public:
    ZLTextParagraphImpl(shared_ptr<ZDZLTextPlainModel> model,int index){
        myModel = model;
        myIndex = index;
    }
    virtual  shared_ptr<ZDZLTextParagraph::EntryIterator> iterator() {
        return new ZDZLTextPlainModel::EntryIteratorImpl(myModel.getT(), myIndex);
    }
    
    virtual uint8_t getKind() {
        return ZDZLTextParagraph::TEXT_PARAGRAPH;
    }
};
#endif /* ZLTextParagraphImpl_hpp */
