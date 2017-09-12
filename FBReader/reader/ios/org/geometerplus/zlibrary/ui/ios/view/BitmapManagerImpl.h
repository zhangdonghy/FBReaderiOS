//
//  BitmapManager.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/31.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef BitmapManager_hpp
#define BitmapManager_hpp

#import <UIKit/UIKit.h>
#include "ZLViewEnums_PageIndex.hpp"
#import "ZDFBReaderInterface.h"

class  BitmapManagerImpl {
private:
    static const int SIZE = 2;
    NSMutableArray* myBitmaps;
    ZLViewEnums_PageIndex* myIndexes[2];
    int myWidth;
    int myHeight;
    
    ZDFBReaderInterface* myWidget;
    
    int getInternalIndex(ZLViewEnums_PageIndex index);

public:
    BitmapManagerImpl(ZDFBReaderInterface* widget);
    ~BitmapManagerImpl();

    int getWidth() {
        return myWidth;
    }
    int getHeight() {
        return  myHeight;
    }
    void setSize(int w, int h);

    virtual UIImage* getBitmap(ZLViewEnums_PageIndex* index);
    virtual void drawBitmap(/*Canvas canvas, */int x, int y, ZLViewEnums_PageIndex index/*, Paint paint*/);
};

#endif /* BitmapManager_hpp */
