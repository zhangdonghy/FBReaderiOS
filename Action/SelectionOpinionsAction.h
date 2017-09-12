//
//  SelectionOpinionsAction.hpp
//  ZDLibrary
//
//  Created by 蒋晨霄 on 2017/2/24.
//  Copyright © 2017年 蒋晨霄. All rights reserved.
//

#ifndef SelectionOpinionsAction_hpp
#define SelectionOpinionsAction_hpp


#include <stdio.h>

#include "ZLApplication.hpp"

class FBReaderApp;
class SelectionOpinionsAction : public ZLAction {
private:
    FBReaderApp* myApp;
    __weak id _interface;
    
public:
    SelectionOpinionsAction(FBReaderApp* app, __weak id interface) {
        myApp = app;
        _interface = interface;
    }
    virtual void run(void* data);
};


#endif /* SelectionOpinionsAction_hpp */
