//
//  BitmapManager.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/31.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "BitmapManagerImpl.h"
#include "ZLViewEnums_PageIndex.hpp"


BitmapManagerImpl::BitmapManagerImpl(ZDFBReaderInterface* widget) {
    myBitmaps = [NSMutableArray arrayWithCapacity:SIZE];
    UIImage* one = [[UIImage alloc] init];
    [myBitmaps addObject:one];
    one = [[UIImage alloc] init];
    [myBitmaps addObject:one];
    
    
    myIndexes[0] = new ZLViewEnums_PageIndex(-1);
    myIndexes[1] = new ZLViewEnums_PageIndex(-1);
    
    myWidget = widget;
}

BitmapManagerImpl::~BitmapManagerImpl() {
    printf("~BitmapManagerImpl\n");
}

void BitmapManagerImpl::setSize(int w, int h)
{
    if (myWidth != w || myHeight != h) {
        myWidth = w;
        myHeight = h;
        for (int i = 0; i < SIZE; ++i) {
            //myBitmaps[i] = 0x0;
            myIndexes[i] = 0x0;
        }
        //System.gc();
       // System.gc();
        //System.gc();
    }
}
int BitmapManagerImpl::getInternalIndex(ZLViewEnums_PageIndex index) {
    for (int i = 0; i < SIZE; ++i) {
        if (myIndexes[i] == 0x0) {
            return i;
        }
    }
    for (int i = 0; i < SIZE; ++i) {
        if (myIndexes[i]->getValue() != ZLViewEnums_PageIndex::current) {
            return i;
        }
    }
    return 0;
    //throw new RuntimeException("That's impossible");
}
UIImage*
BitmapManagerImpl::getBitmap(ZLViewEnums_PageIndex* index)
{
    /*for (int i = 0; i < SIZE; ++i) {
        if(myIndexes[i] == 0) continue;
        if (index->getValue() == myIndexes[i]->getValue()) {
            return myBitmaps[i];
        }
    }*/
     int iIndex = getInternalIndex(*index);
   // new ZLViewEnums_PageIndex();
    myIndexes[iIndex] = new ZLViewEnums_PageIndex(*index);//index;
    
    if (myBitmaps[iIndex] == 0x0) {
        myBitmaps[iIndex] = [[UIImage alloc] init];
        //Bitmap.createBitmap(myWidth, myHeight, Bitmap.Config.RGB_565);
    }
    //myWidget.drawOnBitmap(myBitmaps[iIndex], index);
   return  [myWidget drawOnBitmap:myBitmaps[iIndex] withValue:index->getValue()];
    
   // return myBitmaps[iIndex];
}
void
BitmapManagerImpl::drawBitmap(/*Canvas canvas, */int x, int y, ZLViewEnums_PageIndex index/*, Paint paint*/)
{
    
}
