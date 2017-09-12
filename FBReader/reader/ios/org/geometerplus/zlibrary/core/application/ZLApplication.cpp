//
//  ZLApplication.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/31.
//  Copyright © 2016年 Eagley. All rights reserved.
//
#include "ZLView.hpp"
#include "ZLApplication.hpp"
#include "ZLApplicationWindow.hpp"
#include "ZLViewWidget.hpp"

ZLApplication* ZLApplication::ourInstance = 0x0;

ZLApplication::ZLApplication()
{
    ourInstance = this;
}
ZLApplication* ZLApplication::Instance()
{
    return ourInstance;
}

void ZLApplication::setView(shared_ptr<ZLView> view)
{
    if(view != 0) {
       myView = view;
        shared_ptr<ZLViewWidget> widget = getViewWidget();
        if(widget !=0){
            widget->reset();
            widget->repaint();
        }
    }
    
}
void ZLApplication::addAction(std::string actionId, shared_ptr<ZLAction> action)
{
    myIdToActionMap[actionId] = action;
}
void ZLApplication::runAction(std::string actionId,void* data)
{
    shared_ptr<ZLAction> action = myIdToActionMap[actionId];
    if(action != 0) {
        action->run(data);
    }
}

void ZLApplication::setWindow(shared_ptr<ZLApplicationWindow> window)
{
    myWindow = window;
}
void ZLApplication::initWindow()
{
    setView(myView);
}
shared_ptr<ZLViewWidget>
ZLApplication::getViewWidget()
{
    if(myWindow != 0) {
        return  myWindow->getViewWidget();
    }
    return 0;
}
