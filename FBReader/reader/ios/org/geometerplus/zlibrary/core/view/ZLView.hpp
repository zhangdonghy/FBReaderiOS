//
//  ZLView.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/31.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLView_hpp
#define ZLView_hpp

#include "shared_ptr.h"

#include "ZLApplication.hpp"
#include "ZLPaintContext.hpp"
#include "ZLViewEnums_PageIndex.hpp"

class ZLView {
private:
    shared_ptr<ZLPaintContext> myViewContext;
protected:
    ZLView(ZLApplication*  application);
public:
    shared_ptr<ZLApplication>  Application;
    //private ZLPaintContext myViewContext = new DummyPaintContext();
    
  
    void setContext(shared_ptr<ZLPaintContext> context) {
        myViewContext = context;
    }
    
    shared_ptr<ZLPaintContext> getContext() {
        return myViewContext;
    }
    
    int getContextWidth() {
        return myViewContext->getWidth();
    }
    int getContextHeight() {
        return myViewContext->getHeight();
    }
    /*
    abstract public interface FooterArea {
        int getHeight();
        void paint(ZLPaintContext context);
    }
    
    abstract public FooterArea getFooterArea();
    
    public abstract Animation getAnimationType();
    
    abstract public void preparePage(ZLPaintContext context, PageIndex pageIndex);
    
    abstract public
    */
    virtual void onScrollingFinished(int pageIndex)=0;
    virtual  void onFingerPress(int x, int y)=0;
    virtual void onFingerRelease(int x, int y)=0;
    virtual void onFingerMove(int x, int y)=0;
    virtual bool onFingerLongPress(int x, int y)=0;
    virtual void onFingerReleaseAfterLongPress(int x, int y)=0;
    virtual void onFingerMoveAfterLongPress(int x, int y)=0;
    virtual bool onFingerSingleTap(int x, int y)=0;
    virtual void onFingerDoubleTap(int x, int y)=0;
    virtual void onFingerEventCancelled()=0;
    
    virtual bool isDoubleTapSupported();
    
    virtual bool onTrackballRotated(int diffX, int diffY) ;
    
    virtual bool isScrollbarShown()=0;
    virtual int getScrollbarFullSize()=0;
    virtual void paint(shared_ptr<ZLPaintContext> context,int value)=0;
                        //shared_ptr<ZLViewEnums_PageIndex> pageIndex)=0;
    //virtual int getScrollbarThumbPosition(PageIndex pageIndex)=0;
   // virtual int getScrollbarThumbLength(PageIndex pageIndex)=0;
    
   // virtual boolean canScroll(PageIndex index)=0;
};
#endif /* ZLView_hpp */
