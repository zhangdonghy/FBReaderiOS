//
//  ZDEPubReaderModel.h
//  ZDLibrary
//
//  Created by 蒋晨霄 on 2017/2/21.
//  Copyright © 2017年 蒋晨霄. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "ZDOCBook.h"
#import "ZDEPubConfig.h"
#import "ZDEPubLocation.h"
#import "ZDUnderlineColor.h"

@class ZDEPubMainController;
@interface ZDEPubReaderModel : NSObject

@property (readonly) ZDOCBook* book;
@property (readonly) NSString* bookPath;

- (id)initWithMainCtrl:(ZDEPubMainController *__weak)mainCtrl book:(ZDOCBook *)book path:(NSString *)path;
- (BOOL)openBookAtParagraph:(int)pIndex;
- (void)addReaderActions;

// draw
- (UIImage *)bitmapOfPageState:(ZD_EPUB_PAGE_STATE)pageState withViewSize:(CGSize)viewSize;
- (void)repaint;

// pages
@property (readonly) BOOL hasPrevPage;
@property (readonly) BOOL hasNextPage;
- (void)changePrevPage;
- (void)finishChangePrevPage:(ZD_EPUB_PAGE_STATE)pageState;
- (void)changeNextPage;
- (void)finishChangeNextPage:(ZD_EPUB_PAGE_STATE)pageState;

// text
@property (readonly) NSString* selectedText;
@property (readonly) int selectedTextCount;
- (void)clearSelectedParagraph;
- (ZDEPubLocation *)lastLocationOfParagraph:(int)pIndex;

// location
- (void)jumpToLocation:(ZDEPubLocation *)location;
@property (readonly) NSInteger curPercent;
- (void)jumpToPercent:(NSInteger)percent;
- (void)jumpToPrevCatalog;
- (void)jumpToNextCatalog;

// catalog
- (ZDCatalog *)catalogOfCurPage;
- (NSString *)catalogNameOfCurPage;
- (void)saveAllCatalogs;

// font
- (void)refreshFontSize:(BOOL)hasChange;
- (void)refreshFontName:(BOOL)hasChange;

// theme
- (void)refreshTheme;
- (void)refreshLightState;

// layout
- (void)refreshLayout:(BOOL)hasChange;

// bookmark
@property (readonly) ZDPageTag* curPageTag;
- (BOOL)isBookmarkInCurPage:(ZDBookmark *)bookmark;
- (NSArray *)bookmarksOnCurPage;
- (ZDBookmark *)createBookmarkOnCurPage;

// linedText
- (ZDUnderlineColor *)colorOfLinedTextFromStart:(ZDEPubLocation *)start toEnd:(ZDEPubLocation *)end;
- (void)removeLinedText:(ZDLinedText *)linedText;

// note
//- (void)addNote:(ZDNote *)note;
- (void)removeNote:(ZDNote *)note;
- (NSInteger)countNoteBetweenStart:(ZDEPubLocation *)startLoc toEnd:(ZDEPubLocation *)endLoc;

// opinion
@property (readonly) BOOL opinionEnabled;
- (void)addOpinion:(ZDOpinion *)opinion;
- (void)removeOpinion:(ZDOpinion *)opinion;
- (int)countOpinionInParagraph:(int)pIndex;
- (void)showOpinionsInParagraph:(int)pIndex;

// panel
- (void)showPanelAtLinedText:(ZDLinedText *)linedText;
- (void)showPanel;

// fullscreen image
- (void)showFullScreenImage:(UIImage *)image;

// listen
@property (readonly) BOOL listenEnabled;
- (void)enterListeningMode;
- (void)exitListeningMode;
- (void)getParagraphToListen:(NSString**)para hasNextPara:(BOOL*)hasNextPara;
- (BOOL)tryChangeNextPageInListening;
- (int)tapParagraphAtPos:(CGPoint)tapPos;
- (void)tryStartListeningFromParagraph:(int)pIndex;

// voice
@property (readonly) BOOL hasVoiceBar;

// calculate page count
@property (readonly) BOOL hasPageQuantity;
@property (assign) BOOL isCalculatingPageQuantity;
- (void)getPageQuantity:(long *)totalCount curNum:(long *)curPageNum ofState:(ZD_EPUB_PAGE_STATE)pageState;
- (void)tryToCountPageWithViewSize:(CGSize)viewSize;
- (void)stopCalculatePageQuantity;

// touch handler
- (BOOL)respondLongPressAtPos:(CGPoint)pos fromView:(UIView *)pageView;
- (void)respondPressAtPos:(CGPoint)pos;
- (void)respondTouchMoved:(CGPoint)pos fromView:(UIView *)pageView;
- (BOOL)isPressedOnCursor:(CGPoint)pos;
- (void)respondFingerReleasedAfterLongPress:(CGPoint)pos;
- (void)respondFingerReleased:(CGPoint)pos;
- (BOOL)respondSingleTapAtPos:(CGPoint)pos;
- (BOOL)isTapOnSelectedRegion:(CGPoint)pos;

@end
