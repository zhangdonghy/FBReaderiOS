//
//  ECatalogTree.cpp
//  ZDStudent
//
//  Created by 蒋晨霄 on 16/9/12.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ECatalogTree.hpp"

#pragma mark - ECatalog
ECatalog::ECatalog() {
    cId = 0;
}

ECatalog::~ECatalog() {
    if (childNodes.size() > 0) {
        for (std::vector<ECatalog*>::const_iterator it = childNodes.begin(); it != childNodes.end(); it++) {
            ECatalog* childNode = *it;
            delete childNode;
        }
    }
//    printf("delete: %s, node: %d\n", content.c_str(), value);
}

bool ECatalog::addChildNode(ECatalog* node) {
    if (node == NULL) {
        return false;
    }
    
    node->fatherNode = this;
    childNodes.push_back(node);
    return true;
}

ECatalog* ECatalog::firstInheritor() {
    if (childNodes.size() > 0) {
        ECatalog* firstChild = *childNodes.begin();
        return firstChild->firstInheritor();
    } else {
        return this;
    }
}

#pragma mark - ECatalogTree
ECatalogTree::ECatalogTree() {
    rootNode = new(ECatalog);
    _curNode = rootNode;
    _prevCId = rootNode->cId;
}

ECatalogTree::~ECatalogTree() {
    delete rootNode;
    rootNode = NULL;
    _curNode = NULL;
}

void ECatalogTree::enterNode(std::string name, int pIndex) {
    ECatalog* node = new(ECatalog);
    node->name = name;
    node->pIndex = pIndex;
    
    this->enterNode(node);
}

void ECatalogTree::enterNode(ECatalog* node) {
    int newCId = _prevCId + 1;
    int lv = 1;
    if (_curNode != rootNode) {
        lv = _curNode->lv + 1;
    }

    bool isSuccess = _curNode->addChildNode(node);
    node->cId = newCId;
    node->lv = lv;

    if (isSuccess) {
        _curNode = node;
        _prevCId = newCId;
    }
}

void ECatalogTree::exitNode() {
    if (_curNode == rootNode) {
        // 根节点无法继续退出
        return;
    }
    
    _curNode = _curNode->fatherNode;
}
