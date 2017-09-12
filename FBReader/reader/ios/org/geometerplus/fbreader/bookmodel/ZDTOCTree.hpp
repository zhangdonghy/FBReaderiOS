//
//  ZDTOCTree.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/16.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZDTOCTree_hpp
#define ZDTOCTree_hpp

#include <string>
//#include <stdio.h>

#include "shared_ptr.h"
#include "ZDZLTree.h"
#include "ZDZLTextModel.hpp"


//using namespace std;
class Reference;

class Reference {
public:
    int ParagraphIndex;
    shared_ptr<ZDZLTextModel> Model;
    
    Reference( int paragraphIndex,  shared_ptr<ZDZLTextModel>  model) {
        ParagraphIndex = paragraphIndex;
        Model = model;
    }
};

class ZDTOCTree :public ZDZLTree<ZDTOCTree>{
private:
    std::string myText;
    Reference* myReference;
public:
    ZDTOCTree();
    ZDTOCTree(ZDTOCTree* parent)
    :ZDZLTree(parent)
    {
        
    }
    std::string getText();
    void setText(std::string text)
    {
        myText = text;
    }
     Reference* getReference();
    void setReference(shared_ptr<ZDZLTextModel> model,int reference);
};

inline Reference* ZDTOCTree::getReference() {
    return myReference;
}

inline void ZDTOCTree::setReference(shared_ptr<ZDZLTextModel> model, int reference) {
    myReference = new Reference(reference, model);
}


#endif /* ZDTOC
Tree_hpp */
