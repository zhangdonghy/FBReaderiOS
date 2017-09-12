//
//  SelectionNotesAction.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/20.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef SelectionNotesAction_hpp
#define SelectionNotesAction_hpp


#include <stdio.h>

#include "ZLApplication.hpp"

class FBReaderApp;
class SelectionNotesAction : public ZLAction {
private:
    FBReaderApp* myApp;
    __weak id _interface;
    
public:
    SelectionNotesAction(FBReaderApp* app, __weak id interface) {
        myApp = app;
        _interface = interface;
    }
    virtual void run(void* data);
};


#endif /* SelectionNotesAction_hpp */

