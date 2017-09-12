//
//  ECatalogTree.hpp
//  ZDStudent
//
//  Created by 蒋晨霄 on 16/9/12.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ECatalogTree_hpp
#define ECatalogTree_hpp

#include <stdio.h>
#include <vector>
#include <string>

class ECatalog {
public:
    ECatalog();
    ~ECatalog();

    ECatalog* fatherNode;
    std::vector<ECatalog*> childNodes;
    
    std::string name;
    int pIndex;
    int cId;
    int lv;
    
    bool addChildNode(ECatalog* node);
    
    ECatalog* firstInheritor();
};


class ECatalogTree {
private:
    ECatalog* _curNode;
    int _prevCId;
    
public:
    ECatalogTree();
    ~ECatalogTree();

    ECatalog* rootNode;

    void enterNode(std::string name, int pIndex);
    void enterNode(ECatalog* node);
    void exitNode();
};

#endif /* ECatalogTree_hpp */
