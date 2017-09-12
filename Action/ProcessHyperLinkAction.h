//
//  ProcessHyperLinkAction.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/23.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ProcessHyperLinkAction_hpp
#define ProcessHyperLinkAction_hpp

#include <stdio.h>
#include "ZLApplication.hpp"

class FBReaderApp;
class ProcessHyperLinkAction : public ZLAction {
private:
    shared_ptr<FBReaderApp> myApp;
    __weak id _interface;

public:
    ProcessHyperLinkAction(shared_ptr<FBReaderApp> app, __weak id interface) {
        myApp = app;
        _interface = interface;
    };
    virtual void run(void* data);
};

#endif /* ProcessHyperLinkAction_hpp */

