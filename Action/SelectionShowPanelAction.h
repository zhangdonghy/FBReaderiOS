//
//  SelectionShowPanelAction.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/12.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef SelectionShowPanelAction_hpp
#define SelectionShowPanelAction_hpp


#include <stdio.h>

#include "ZLApplication.hpp"

class FBReaderApp;
class SelectionShowPanelAction :public ZLAction {
private:
    FBReaderApp* myApp;
    __weak id _interface;

public:
    SelectionShowPanelAction(FBReaderApp* app, __weak id interface) {
        myApp = app;
        _interface = interface;
    }
    virtual void run(void* data);
};


#endif /* SelectionShowPanelAction_hpp */

