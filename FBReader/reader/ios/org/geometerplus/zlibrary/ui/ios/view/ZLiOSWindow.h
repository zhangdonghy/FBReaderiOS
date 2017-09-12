//
//  ZLiOSWindow.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/10.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLiOSWindow_hpp
#define ZLiOSWindow_hpp

#include <stdio.h>
#include "ZLApplicationWindow.hpp"
#import <UIKit/UIKit.h>

class ZLiOSWindow: public ZLApplicationWindow
{
    private:
    shared_ptr<ZLViewWidget> myMainView;
    public:
    ZLiOSWindow(shared_ptr<ZLViewWidget> widget)
    {
        myMainView = widget;
    }
    virtual void setWindowTitle(std::string title);
    virtual void showErrorMessage(std::string resourceKey);
    virtual void showErrorMessage(std::string resourceKey, std::string parameter);
    // ZLApplication.SynchronousExecutor createExecutor(String key);
    virtual void processException(std::string e);//(Exception e);
    
    virtual void refresh();
    
    virtual shared_ptr<ZLViewWidget> getViewWidget();
    
    virtual void close();
    
    virtual int getBatteryLevel();
};
#endif /* ZLiOSWindow_hpp */
