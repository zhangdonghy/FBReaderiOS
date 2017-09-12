//
//  ZDFBReaderDelegate.h
//  ZDLibrary
//
//  Created by 蒋晨霄 on 2017/8/28.
//  Copyright © 2017年 蒋晨霄. All rights reserved.
//

#ifndef ZDFBReaderDelegate_h
#define ZDFBReaderDelegate_h

#import "ZDUnderlineColor.h"
#import "ZDEPubLocation.h"

@protocol ZDFBReaderDelegate <NSObject>

@optional
- (void)repaintFromReader;
- (ZDUnderlineColor *)colorOfLinedTextInReaderFromStart:(ZDEPubLocation *)startLoc toEnd:(ZDEPubLocation *)endLoc;
- (void)showPanelFromReaderAtLinedText:(ZDLinedText *)linedText;
- (void)hidePanelFromReader;
- (NSInteger)countNoteInReaderFromStart:(ZDEPubLocation *)startLoc toEnd:(ZDEPubLocation *)endLoc;
- (int)countOpinionInReaderAtParagraph:(int)pIndex;
- (void)showOpinionsInReaderAtParagraph:(int)pIndex;
- (void)showFullScreenImageFromReader:(UIImage *)image;

@end

#endif /* ZDFBReaderDelegate_h */
