//
//  SelectionShowPanelAction.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/12.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "SelectionShowPanelAction.h"

#include "FBReaderApp.h"

#import "ZDFBReaderInterface.h"

void SelectionShowPanelAction::run(void* data) {
    printf("SelectionShowPanelAction\n");

    BookNoteContext* note = (BookNoteContext *)data;
    if (note == NULL) {
        return;
    }

//    NSLog(@"BookNoteContext: pStart: %d, eStart: %d, cStart: %d, pEnd: %d, eEnd: %d, cEnd: %d",
//          note->ParagraphIndex, note->ElementIndex, note->CharIndex,
//          note->ParagraphEndIndex, note->ElementEndIndex, note->CharEndIndex);
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
    
    myApp->clearSelectText();

    if ([_interface respondsToSelector:@selector(showPanelAtLinedText:)]) {
        [_interface showPanelAtLinedText:linedText];
    }
}

