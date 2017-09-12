//
//  ZLApplicationWindow.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/10.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLApplicationWindow_hpp
#define ZLApplicationWindow_hpp

#include <stdio.h>
#include <string>
#include "shared_ptr.h"


class ZLViewWidget;
class ZLApplicationWindow
{
    public:
    virtual void setWindowTitle(std::string title)=0;
    virtual void showErrorMessage(std::string resourceKey)=0;
    virtual void showErrorMessage(std::string resourceKey, std::string parameter)=0;
   // ZLApplication.SynchronousExecutor createExecutor(String key);
    virtual void processException(std::string e)=0;//(Exception e);
    
    virtual void refresh()=0;
    
    virtual shared_ptr<ZLViewWidget> getViewWidget()=0;
    
    virtual void close()=0;
    
    virtual int getBatteryLevel()=0;
};
#endif /* ZLApplicationWindow_hpp */
