//
//  SelectionNotesAction.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/20.
//  Copyright © 2016年 Eagley. All rights reserved.
//
//#import <Foundation/Foundation.h>

#include "SelectionNotesAction.h"
#include "FBReaderApp.h"

#import "ZDFBReaderInterface.h"

void SelectionNotesAction::run(void* data) {
    printf("SelectionNotesAction\n");

    BookNoteContext* note = (BookNoteContext *)data;
    ZDLinedText* linedText = [[ZDLinedText alloc] init];
    linedText.bookType = ZD_BOOK_TYPE_EPUB;
    [linedText setStartLocWithParagraph:note->ParagraphIndex element:note->ElementIndex character:note->CharIndex];
    [linedText setEndLocWithParagraph:note->ParagraphEndIndex element:note->ElementEndIndex character:note->CharEndIndex];
    linedText.xStart = note->StartX;
    linedText.yStart = note->StartY;
    linedText.xEnd = note->EndX;
    linedText.yEnd = note->EndY;

    linedText.quote = [NSString stringWithUTF8String:myApp->getSelectStr().c_str()];
    
    linedText.percent = (int)myApp->curPercent();

    if ([_interface respondsToSelector:@selector(showPanelAtLinedText:)]) {
        [_interface showPanelAtLinedText:linedText];
    }
}

