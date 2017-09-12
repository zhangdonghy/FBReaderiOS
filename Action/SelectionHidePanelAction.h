//
//  SelectionHidePanelAction.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/12.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef SelectionHidePanelAction_hpp
#define SelectionHidePanelAction_hpp

#include <stdio.h>
#include "ZLApplication.hpp"

class SelectionHidePanelAction : public ZLAction {
private:
    __weak id _interface;

public:
    SelectionHidePanelAction(__weak id interface) {
        _interface = interface;
    }
    virtual void run(void* data);
};

#endif /* SelectionHidePanelAction_hpp */

