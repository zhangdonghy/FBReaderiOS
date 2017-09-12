//
//  CursorManager.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/1.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "CursorManager.hpp"

#include <map>
#include "ZLTextParagraphCursor.hpp"

shared_ptr<ZLTextParagraphCursor>
CursorManager::get(int index)
{
    
   // bool find = false;
    for(std::map<int,shared_ptr<ZLTextParagraphCursor>>::const_iterator it=lruCache.begin();
        it!=lruCache.end();++it)
    {
        if(it->first == index){
            return it->second;
        }
    }
    //
    //shared_ptr<CursorManager> xxx = this;
    //lruCache[index] =new ZLTextParagraphCursor(this, myModel, index);//newCursor;
    //shared_ptr<ZLTextParagraphCursor> xx =
    //new ZLTextParagraphCursor(this, myModel, index);
    lruCache.insert(std::make_pair(index,new ZLTextParagraphCursor(this, myModel, index)));
    return lruCache[index] ;
    
    //ZLTextParagraphCursor* xx = lruCache[index];//.at(index);
   // std::map<int,shared_ptr<ZLTextParagraphCursor>>::const_iterator xx2 = lruCache.find(index);
    
    //shared_ptr<ZLTextParagraphCursor> result = xx2->second;
    //if(/*xx2 != lruCache.end()*/result !=0) {
        
     //   return result;
    //} else {
     //   shared_ptr<CursorManager> xxx = this;
     ///   lruCache[index] =new ZLTextParagraphCursor(xxx, myModel, index);//newCursor;
     //   return  lruCache[index] ;
   // }
    
   // if(xx == 0){
       // shared_ptr<CursorManager> xxx = this;
       // shared_ptr<ZLTextParagraphCursor>  newCursor =new ZLTextParagraphCursor(xxx, myModel, index);
        //lruCache.insert(std::make_pair(index,newCursor));
       // lruCache[index] =new ZLTextParagraphCursor(xxx, myModel, index);//newCursor;
        //return  lruCache[index] ;//newCursor;
   // }
    //int realindex = index%200;
    //shared_ptr<ZLTextParagraphCursor> xx = lruCache[realindex];
    //return xx;
}
CursorManager::CursorManager(shared_ptr<ZDZLTextModel> model, shared_ptr<ExtensionElementManager> extManager){
    myModel = model;
    ExtensionManager = extManager;
    maxSize = 200;
    // init with 200
   // for(int i=0;i<200;i++){
       // shared_ptr<CursorManager> xx = this;
       // shared_ptr<ZLTextParagraphCursor>  newCursor =new ZLTextParagraphCursor(xx, myModel, i);
       // lruCache.insert(std::make_pair(i,newCursor));
        //lruCache.insert(lruCache.end(), newCursor);
        
   // }
    //shared_ptr<ZLTextParagraphCursor> xx = lruCache[index];
  //  ZLTextParagraphCursor* xx = lruCache[0].getT();

}
void CursorManager::evictAll()
{
    lruCache.clear();
}