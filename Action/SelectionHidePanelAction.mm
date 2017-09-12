//
//  SelectionHidePanelAction.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/12.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "SelectionHidePanelAction.h"

#import "ZDFBReaderInterface.h"

void SelectionHidePanelAction::run(void* data) {
    printf("SelectionHidePanelAction\n");

    if ([_interface respondsToSelector:@selector(hidePanel)]) {
        [_interface hidePanel];
    }
}

