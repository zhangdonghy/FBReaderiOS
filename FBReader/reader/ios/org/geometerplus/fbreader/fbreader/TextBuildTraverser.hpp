//
//  TextBuildTraverser.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/10.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef TextBuildTraverser_hpp
#define TextBuildTraverser_hpp

#include <stdio.h>
#include <string>
#include "ZLTextTraverser.hpp"
#include "ZLTextWord.hpp"

class TextBuildTraverser : public ZLTextTraverser {
protected:
    //StringBuilder myBuffer = new StringBuilder();
    std::vector<short> myBuffer;
    TextBuildTraverser(ZLTextView* view) : ZLTextTraverser(view) {
    }
    
    virtual void processWord(ZLTextWord& word) {
        //myBuffer.append(word.Data, word.Offset, word.Length);
        // short oo=0;
        
        for(int i=0;i<word.Length;i++){
            myBuffer.insert(myBuffer.end(), word.Data[word.Offset+i]);
        }
    }
    
    virtual void processControlElement(ZLTextControlElement& control) {
        // does nothing
    }
    
    virtual void processSpace() {
        //myBuffer.append(" ");
        myBuffer.insert(myBuffer.end(), ' ');
    }
    
    virtual void processNbSpace() {
        //myBuffer.append("\240");  zhangdong
        myBuffer.insert(myBuffer.end(), '\240');
    }
    
    virtual void processEndOfParagraph() {
        //myBuffer.append("\n");
        myBuffer.insert(myBuffer.end(), '\n');
    }
    
public:
    std::string getText();

    friend class FBView;
};
#endif /* TextBuildTraverser_hpp */
