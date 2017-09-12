//
//  CursorManager.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/1.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef CursorManager_hpp
#define CursorManager_hpp

#include <map>
#include <vector>
#include <stdio.h>
#include "ZDZLTextModel.hpp"
#include "ExtensionElementManager.hpp"
//#include "ZLTextParagraphCursor.hpp"

class ZLTextParagraphCursor;

class CursorManager // TODO extend lru cache
{
private:
    shared_ptr<ZDZLTextModel> myModel;
    shared_ptr<ExtensionElementManager> ExtensionManager;
    std::map<int,shared_ptr<ZLTextParagraphCursor>> lruCache;
    int maxSize;
   // std::vector<shared_ptr<ZLTextParagraphCursor>> lruCache;
public:
    CursorManager(shared_ptr<ZDZLTextModel> model, shared_ptr<ExtensionElementManager> extManager);
    
    /*CursorManager(ZLTextModel model, ExtensionElementManager extManager) {
     super(200); // max 200 cursors in the cache
     myModel = model;
     ExtensionManager = extManager;
     }
     
     @Override
     protected ZLTextParagraphCursor create(Integer index) {
     return new ZLTextParagraphCursor(this, myModel, index);
     }*/
    shared_ptr<ZLTextParagraphCursor> get(int index);
    void evictAll();
    friend class ZLTextParagraphCursor;
};
#endif /* CursorManager_hpp */
