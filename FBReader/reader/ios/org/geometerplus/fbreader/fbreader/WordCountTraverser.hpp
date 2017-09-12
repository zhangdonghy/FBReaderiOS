//
//  WordCountTraverser.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/10.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef WordCountTraverser_hpp
#define WordCountTraverser_hpp

#include <stdio.h>
#include "ZLTextTraverser.hpp"

class WordCountTraverser:public ZLTextTraverser
{
    protected:
     int myCount;
    
    WordCountTraverser(ZLTextView* view)
    :ZLTextTraverser(view)
    {
        myCount=0;
    }
    
    virtual  void processWord(ZLTextWord& word) {
        ++myCount;
    }
    
    virtual void processControlElement(ZLTextControlElement& control) {
        // does nothing
    }
    
    virtual void processSpace() {
        // does nothing
    }
    
    virtual void processNbSpace() {
        // does nothing
    }
    
   virtual void processEndOfParagraph() {
        // does nothing
    }
    
    public:
    int getCount() {
        return myCount;
    }
    friend class FBView;
};
#endif /* WordCountTraverser_hpp */
