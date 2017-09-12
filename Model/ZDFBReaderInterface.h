//
//  ZDFBReaderInterface.h
//  ZDLibrary
//
//  Created by 蒋晨霄 on 2017/2/10.
//  Copyright © 2017年 蒋晨霄. All rights reserved.
//

#import <UIKit/UIKit.h>

#include "ECatalogTree.hpp"

#import "ZDEPubConfig.h"
#import "ZDOCBook.h"
#import "ZDEPubLocation.h"
#import "ZDPageTag.h"
#import "ZDLinedText.h"
#import "ZDEPubPageQuantity.h"
#import "ZDReaderOpinion.h"
#import "ZDFBReaderDelegate.h"

@class ZDPageCountModel;
@interface ZDFBReaderInterface : NSObject

- (id)initWithDelegate:(__weak id<ZDFBReaderDelegate>)delegate;

// FBReader interface
// actions
- (void)showPanelAtLinedText:(ZDLinedText *)linedText;
- (void)hidePanel;
- (void)showOpinionsInParagraph:(int)pIndex;
- (void)showFullScreenImage:(UIImage *)image;

// draw
- (UIImage *)drawOnBitmap:(UIImage *)bitmap withValue:(int)value;
- (void)repaint;

// line color
- (void)getSelectingBgColorWithRed:(int *)red green:(int *)green blue:(int *)blue;
- (void)getSelectingCursorColorWithRed:(int *)red green:(int *)green blue:(int *)blue;
- (void)getUnderlineColorWithRed:(int*)red
                           green:(int*)green
                            blue:(int*)blue
              fromStartParagraph:(int)pStart
                         Element:(int)eStart
                       Character:(int)cStart
                    endParagraph:(int)pEnd
                         Element:(int)eEnd
                       Character:(int)cEnd;

// note
- (NSInteger)countNoteBetweenStart:(ZDEPubLocation *)startLoc toEnd:(ZDEPubLocation *)endLoc;

// opinion
- (int)countOpinionInParagraph:(int)pIndex;


// ZDEPubReaderModel interface
// init FBReader
- (BOOL)openBook:(ZDOCBook *)book atPath:(NSString *)path jumpParagraph:(int)pIndex;
@property (readonly) BOOL isEnglishBook;

- (void)addActionsWithCtrl:(id)ctrl;

// draw
- (UIImage *)bitmapOfPageState:(int)pageState withViewSize:(CGSize)viewSize;

// pages
@property (readonly) BOOL hasPrevPage;
@property (readonly) BOOL hasNextPage;
- (void)didFinishChangePageAtState:(int)pageState;

// text
@property (readonly) NSString* selectedText;
@property (readonly) int selectedTextCount;
- (void)clearSelectedParagraph;
- (void)getElement:(int *)eIndex character:(int *)cIndex ofParagraph:(int)pIndex;

// location
- (void)jumpToLocation:(ZDEPubLocation *)location;
@property (readonly) NSInteger percent;
- (void)jumpToPercent:(NSInteger)percent;

// catalog
- (ECatalogTree *)catalogTree;

// font
- (void)setFontSize:(int)fontSize;
- (void)setFontName:(NSString *)fontName;

// theme
- (void)changeBackgroundColorWithRed:(int)red green:(int)green blue:(int)blue;
- (void)changeTextColorWithRed:(int)red green:(int)green blue:(int)blue;

// line space
- (void)setLineSpace:(int)lineSpace;

// bookmark
@property (readonly) ZDPageTag* curPageTag;
- (BOOL)isBookmarkInCurPage:(ZDBookmark *)bookmark;

// lined text
- (void)addLinedTexts:(NSArray *)linedTexts;
- (void)removeLinedText:(ZDLinedText *)linedText;

// note
//- (void)addNote:(ZDNote *)note;

// opinion
- (void)addOpinions:(NSDictionary *)readerOpinions;
- (void)addOpinion:(ZDReaderOpinion *)opinion;
- (void)removeOpinion:(ZDReaderOpinion *)opinion;

// panel
- (void)showPanel;

// listening
- (void)enterListeningMode;
- (void)exitListeningMode;
- (void)getParagraphToListen:(NSString**)para hasNextPara:(BOOL*)hasNextPara;
- (BOOL)hasNextPageInListening:(int)pageState;
- (int)tapParagraphAtPos:(CGPoint)tapPos;
- (void)tryStartListeningFromParagraph:(int)pIndex;

// calculate page count
- (void)connectReaderAppToPageCount:(ZDPageCountModel *)pageCountModel;
- (void)countPage:(long *)totalCount curNum:(long *)curPageNum ofState:(int)pageState;
- (void)countPage:(int *)totalCount curNum:(int *)curPageNum ofState:(int)pageState inQuantity:(ZDEPubPageQuantity *)quantity;

// touch handler
- (BOOL)respondLongPressAtPos:(CGPoint)pos;
- (void)respondPressAtPos:(CGPoint)pos;
- (void)respondTouchMoved:(CGPoint)pos;
- (BOOL)isPressedOnCursor:(CGPoint)pos;
- (void)respondFingerReleasedAfterLongPress:(CGPoint)pos;
- (void)respondFingerReleased:(CGPoint)pos;
- (BOOL)respondSingleTapAtPos:(CGPoint)pos;
- (BOOL)isTapOnSelectedRegion:(CGPoint)pos;

@end
