//
//  SelectionOpinionsAction.cpp
//  ZDLibrary
//
//  Created by 蒋晨霄 on 2017/2/24.
//  Copyright © 2017年 蒋晨霄. All rights reserved.
//

#include "SelectionOpinionsAction.h"

#include "FBReaderApp.h"

#import "ZDFBReaderInterface.h"

void SelectionOpinionsAction::run(void* data) {
    printf("SelectionNotesAction\n");
    
    int pIndex = *(int*)data;
    printf("pIndex: %d\n", pIndex);

    if ([_interface respondsToSelector:@selector(showOpinionsInParagraph:)]) {
        [_interface showOpinionsInParagraph:pIndex];
    }
}

