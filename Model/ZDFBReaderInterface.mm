//
//  ZDFBReaderInterface.m
//  ZDLibrary
//
//  Created by 蒋晨霄 on 2017/2/10.
//  Copyright © 2017年 蒋晨霄. All rights reserved.
//

#import "ZDFBReaderInterface.h"

#include "FBReaderApp.h"
#include "BitmapManagerImpl.h"
#include "ZLiOSWindow.h"
#include "ZLiOSWidget.h"
#include "ZLiOSImageManager.h"
#include "ZLiOSLibrary.hpp"
#include "ZLiOSPaintContext.h"
#include "ActionCode.hpp"
#include "SelectionShowPanelAction.h"
#include "SelectionHidePanelAction.h"
#include "SelectionNotesAction.h"
#include "SelectionOpinionsAction.h"
#include "ProcessHyperLinkAction.h"

#import "ZDPageCountModel.h"

@interface ZDFBReaderInterface () {
    FBReaderApp* _readerApp;
    BitmapManagerImpl* _impl;
    ZLiOSWindow* _window;
    ZLiOSWidget* _widget;
    ZLiOSImageManager* _imageManager;
    // library
    ZLiOSLibrary* _library;
}

@property (weak) id<ZDFBReaderDelegate> delegate;

@end

@implementation ZDFBReaderInterface

- (id)initWithDelegate:(__weak id<ZDFBReaderDelegate>)delegate {
    if (self = [super init]) {
        [self initDataWithDelegate:delegate];
    }
    return self;
}

- (void)dealloc {
    NSLog(@"dealloc ZDFBReaderInterface");
}

- (void)initDataWithDelegate:(__weak id<ZDFBReaderDelegate>)delegate {
    _delegate = delegate;

    [self createReaderApp];
}

- (BOOL)openBook:(ZDOCBook *)book atPath:(NSString *)path jumpParagraph:(int)pIndex {
    return [self openBookAtPath:path jumpParagraph:pIndex];
}

- (void)createReaderApp {
    NSLog(@"ZDFBReaderInterface createReaderApp");

    _imageManager = new ZLiOSImageManager();
    _impl = new BitmapManagerImpl(self);
    _readerApp = new FBReaderApp();
    
    // add related actions!!
    _widget = new ZLiOSWidget(self);
    _window = new ZLiOSWindow(_widget);
    _readerApp->setWindow(_window);
    _readerApp->initWindow();
    
    _library = new ZLiOSLibrary();
}

- (BOOL)openBookAtPath:(NSString *)path jumpParagraph:(int)pIndex {
//    NSString* bookPath = [[NSBundle mainBundle] pathForResource:@"9787210084099.epub" ofType:nil];
//    return _readerApp->openBook(bookPath.UTF8String, 0);
    return _readerApp->openBook(path.UTF8String, pIndex);
}

- (BOOL)isEnglishBook {
    return _readerApp->IsEnglishBook();
}

#pragma mark - actions
- (void)addActionsWithCtrl:(id)ctrl {
    _readerApp->addAction(ActionCode::SELECTION_HIDE_PANEL, new SelectionHidePanelAction(self));
    _readerApp->addAction(ActionCode::SELECTION_SHOW_PANEL,  new SelectionShowPanelAction(_readerApp, self));
    _readerApp->addAction(ActionCode::SELECTION_NOTES,  new SelectionNotesAction(_readerApp, self));
    _readerApp->addAction(ActionCode::SELECTION_OPINIONS, new SelectionOpinionsAction(_readerApp, self));
    _readerApp->addAction(ActionCode::PROCESS_HYPERLINK, new ProcessHyperLinkAction(_readerApp, self));
}

- (void)showPanelAtLinedText:(ZDLinedText *)linedText {
    if ([_delegate respondsToSelector:@selector(showPanelFromReaderAtLinedText:)]) {
        [_delegate showPanelFromReaderAtLinedText:linedText];
    }
}

- (void)hidePanel {
    if ([_delegate respondsToSelector:@selector(hidePanelFromReader)]) {
        [_delegate hidePanelFromReader];
    }
}

- (void)showOpinionsInParagraph:(int)pIndex {
    if ([_delegate respondsToSelector:@selector(showOpinionsInReaderAtParagraph:)]) {
        [_delegate showOpinionsInReaderAtParagraph:pIndex];
    }
}

- (void)showFullScreenImage:(UIImage *)image {
    if ([_delegate respondsToSelector:@selector(showFullScreenImageFromReader:)]) {
        [_delegate showFullScreenImageFromReader:image];
    }
}

#pragma mark - draw
- (UIImage *)bitmapOfPageState:(int)pageState withViewSize:(CGSize)viewSize {
    _impl->setSize((int)viewSize.width, (int)viewSize.height);
    return _impl->getBitmap(new ZLViewEnums_PageIndex(pageState));
}

- (UIImage *)drawOnBitmap:(UIImage *)bitmap withValue:(int)value {
    shared_ptr<ZLTextView> view = _readerApp->BookTextView;
    if (view == 0x0) {
        return nil;
    }

    ZLiOSPaintContext* context = new ZLiOSPaintContext();
    context->initContext(_impl->getWidth(), _impl->getHeight());
    view->paint(context, value);
    UIImage* image = context->getLatestUIImage();

    return image;
}

- (void)repaint {
    if ([_delegate respondsToSelector:@selector(repaintFromReader)]) {
        [_delegate repaintFromReader];
    }
}

#pragma mark - page control
- (BOOL)hasPrevPage {
    return _readerApp->hasPrevPage();
}

- (BOOL)hasNextPage {
    return _readerApp->hasNextPage();
}

- (void)didFinishChangePageAtState:(int)pageState {
    // 重新绘制阅读视图
    _readerApp->onScrollingFinished(pageState);
}

#pragma mark - text
- (NSString *)selectedText {
    return [NSString stringWithUTF8String:_readerApp->getSelectStr().c_str()];
}

- (int)selectedTextCount {
    return _readerApp->getCountOfSelectedWords();
}

- (void)clearSelectedParagraph {
    _readerApp->runAction(ActionCode::SELECTION_HIDE_PANEL, 0);
    _readerApp->clearSelectText();
}

- (void)getElement:(int *)eIndex character:(int *)cIndex ofParagraph:(int)pIndex {
    _readerApp->getLastElementOfParagraph(pIndex, eIndex, cIndex);
}

#pragma mark - location
- (void)jumpToLocation:(ZDEPubLocation *)location {
    NSLog(@"jump to location: %d, %d, %d", location.pIndex, location.eIndex, location.cIndex);
    _readerApp->jumpToLocation(location.pIndex, location.eIndex, location.cIndex);
}

- (NSInteger)percent {
    return _readerApp->curPercent();
}

- (void)jumpToPercent:(NSInteger)percent {
    _readerApp->jumpToPercent(percent);
}

#pragma mark - catalog
- (ECatalogTree *)catalogTree {
    return _readerApp->getECatalogTree();
}

#pragma mark - font
- (void)setFontSize:(int)fontSize {
    _readerApp->setFontSize(fontSize);
}

- (void)setFontName:(NSString *)fontName {
    _readerApp->setFontName(fontName.UTF8String);
}

#pragma mark - theme
- (void)changeBackgroundColorWithRed:(int)red green:(int)green blue:(int)blue {
    _readerApp->changeBackgroundColor(red, green, blue);
}

- (void)changeTextColorWithRed:(int)red green:(int)green blue:(int)blue {
    _readerApp->changeTextColor(red, green, blue);
}

#pragma mark - line space
- (void)setLineSpace:(int)lineSpace {
    _readerApp->setLineSpace(lineSpace);
}

#pragma mark - bookmark
- (ZDPageTag *)curPageTag {
    int pIndex = 0, eIndex = 0, cIndex = 0, percent = 0;
    std::string word;
    _readerApp->getPageStart(&pIndex, &eIndex, &cIndex, &percent, &word);

    ZDPageTag* pageTag = [[ZDPageTag alloc] init];
    pageTag.bookType = ZD_BOOK_TYPE_EPUB;
    pageTag.paraIndex = pIndex;
    pageTag.eIndex = eIndex;
    pageTag.cIndex = cIndex;
    pageTag.percent = percent;
    pageTag.text = [NSString stringWithUTF8String:word.c_str()];

    return pageTag;
}

- (BOOL)isBookmarkInCurPage:(ZDBookmark *)bookmark {
    return _readerApp->isInCurPage(bookmark.paraIndex, bookmark.eIndex, bookmark.cIndex);
}

#pragma mark - lined text
- (void)getSelectingBgColorWithRed:(int *)red green:(int *)green blue:(int *)blue {
    *red = 113;
    *green = 62;
    *blue = 62;
}

- (void)getSelectingCursorColorWithRed:(int *)red green:(int *)green blue:(int *)blue {
    *red = 209;
    *green = 57;
    *blue = 57;
}

- (void)getUnderlineColorWithRed:(int *)red
                           green:(int *)green
                            blue:(int *)blue
                       fromStartParagraph:(int)pStart
                         Element:(int)eStart
                       Character:(int)cStart
                    endParagraph:(int)pEnd
                         Element:(int)eEnd
                       Character:(int)cEnd {
    ZDEPubLocation* startLoc = [[ZDEPubLocation alloc] init];
    startLoc.bookType = ZD_BOOK_TYPE_EPUB;
    startLoc.pIndex = pStart;
    startLoc.eIndex = eStart;
    startLoc.cIndex = cStart;
    ZDEPubLocation* endLoc = [[ZDEPubLocation alloc] init];
    endLoc.bookType = ZD_BOOK_TYPE_EPUB;
    endLoc.pIndex = pEnd;
    endLoc.eIndex = eEnd;
    endLoc.cIndex = cEnd;

    ZDUnderlineColor* underlineColor = nil;
    if ([_delegate respondsToSelector:@selector(colorOfLinedTextInReaderFromStart:toEnd:)]) {
        underlineColor = [_delegate colorOfLinedTextInReaderFromStart:startLoc toEnd:endLoc];
    }
    *red = underlineColor.red;
    *green = underlineColor.green;
    *blue = underlineColor.blue;
}

- (void)addLinedTexts:(NSArray *)linedTexts {
    for (int i = 0; i < linedTexts.count; i++) {
        ZDLinedText* linedText = linedTexts[i];
        BookNote bookNote = [self convertLinedTextToBookNote:linedText];
        _readerApp->addUnderline(bookNote);
    }
}

- (void)removeLinedText:(ZDLinedText *)linedText {
    BookNote bookNote = [self convertLinedTextToBookNote:linedText];
    _readerApp->removeUnderline(bookNote);

    [self repaint];
}

- (BookNote)convertLinedTextToBookNote:(ZDLinedText *)linedText {
    BookNote bookNote;
    bookNote.ParagraphIndex = linedText.startLoc.pIndex;
    bookNote.ElementIndex = linedText.startLoc.eIndex;
    bookNote.CharIndex = linedText.startLoc.cIndex;
    bookNote.ParagraphEndIndex = linedText.endLoc.pIndex;
    bookNote.ElementEndIndex = linedText.endLoc.eIndex;
    bookNote.CharEndIndex = linedText.endLoc.cIndex;
    return bookNote;
}

#pragma mark - note
//- (BookNote)convertZDNoteToBookNote:(ZDNote *)originNote {
//    BookNote bookNote;
//    bookNote.ParagraphIndex = originNote.startLoc.pIndex;
//    bookNote.ElementIndex = originNote.startLoc.eIndex;
//    bookNote.CharIndex = originNote.startLoc.cIndex;
//    bookNote.ParagraphEndIndex = originNote.endLoc.pIndex;
//    bookNote.ElementEndIndex = originNote.endLoc.eIndex;
//    bookNote.CharEndIndex = originNote.endLoc.cIndex;
//    return bookNote;
//}

//- (void)addNote:(ZDNote *)note {
//    BookNote bookNote = [self convertZDNoteToBookNote:note];
//    _readerApp->addUnderline(bookNote);
//    
//    [self clearSelectedParagraph];
//}

- (NSInteger)countNoteBetweenStart:(ZDEPubLocation *)startLoc toEnd:(ZDEPubLocation *)endLoc {
    if ([_delegate respondsToSelector:@selector(countNoteInReaderFromStart:toEnd:)]) {
        return [_delegate countNoteInReaderFromStart:startLoc toEnd:endLoc];
    }
    return 0;
}

#pragma mark - opinion
- (int)countOpinionInParagraph:(int)pIndex {
    if ([_delegate respondsToSelector:@selector(countOpinionInReaderAtParagraph:)]) {
        return [_delegate countOpinionInReaderAtParagraph:pIndex];
    }
    return 0;
}

- (void)addOpinions:(NSDictionary *)readerOpinions {
    for (NSString* pKey in readerOpinions.allKeys) {
        [self addOpinion:readerOpinions[pKey]];
    }
}

- (void)addOpinion:(ZDReaderOpinion *)opinion {
    BookNote bookOpinion = [self convertReaderOpinionToBookNote:opinion];
    _readerApp->addOpinion(bookOpinion);
}

- (void)removeOpinion:(ZDReaderOpinion *)opinion {
    BookNote bookOpinion = [self convertReaderOpinionToBookNote:opinion];
    _readerApp->removeOpinion(bookOpinion);
}

- (BookNote)convertReaderOpinionToBookNote:(ZDReaderOpinion *)opinion {
    BookNote bookNote;
    bookNote.ParagraphIndex = opinion.pIndex;
    bookNote.ElementIndex = opinion.eStart;
    bookNote.CharIndex = opinion.cStart;
    bookNote.ParagraphEndIndex = opinion.pIndex;
    bookNote.ElementEndIndex = opinion.eEnd;
    bookNote.CharEndIndex = opinion.cEnd;
    return bookNote;
}

#pragma mark - panel
- (void)showPanel {
    _readerApp->runAction(ActionCode::SELECTION_SHOW_PANEL, 0);
}

#pragma mark - listening
- (void)enterListeningMode {
    _readerApp->clearSelectText();
    _readerApp->BeginRead();
}

- (void)exitListeningMode {
    _readerApp->ExitRead();
}

- (void)getParagraphToListen:(NSString *__autoreleasing *)para hasNextPara:(BOOL *)hasNextPara {
    std::string xx;
    bool hasNextPInCurPage = _readerApp->NextParagraph(&xx);
    
    *para = [NSString stringWithUTF8String:xx.c_str()];
    *hasNextPara = hasNextPInCurPage;
}

- (BOOL)hasNextPageInListening:(int)pageState {
    return _readerApp->hasNextPageInReading(pageState);
}

- (int)tapParagraphAtPos:(CGPoint)tapPos {
    return _readerApp->paragraphIndexOfPos(tapPos.x, tapPos.y);
}

- (void)tryStartListeningFromParagraph:(int)pIndex {
    _readerApp->moveReadStartToParagraph(pIndex);
}

#pragma mark - calculate page count
- (void)connectReaderAppToPageCount:(ZDPageCountModel *)pageCountModel {
    pageCountModel.readerApp = _readerApp;
}

- (void)countPage:(long *)totalCount curNum:(long *)curPageNum ofState:(int)pageState {
    shared_ptr<PagePosition> pagePosition = _readerApp->pagePosition();

    int pageTotal = pagePosition->Total;

    int pageIndex = pagePosition->Current;
    if (pageState == ZLViewEnums_PageIndex::next) {
        pageIndex++;
        pageIndex = pageIndex <= pageTotal ? pageIndex : pageTotal;
    } else if (pageState == ZLViewEnums_PageIndex::previous) {
        pageIndex--;
        pageIndex = pageIndex > 0 ? pageIndex : 1;
    }
    
    *totalCount = pageTotal;
    *curPageNum = pageIndex;
}

- (void)countPage:(int *)totalCount curNum:(int *)curPageNum ofState:(int)pageState inQuantity:(ZDEPubPageQuantity *)quantity {
    *totalCount = quantity.quantity;
    
    int pageIndex = 0;
    ZDPageTag* pageTag = self.curPageTag;
    for (ZDEPubPageLocation* location in quantity.locations) {
        BOOL isInPage = [self isCurrentPage:pageTag betweenStartLoc:location.startLoc endLoc:location.endLoc];
        if (isInPage) {
            pageIndex = location.pageIndex;
            break;
        }
    }
    if (pageIndex <= 0) {
        *totalCount = 0;
        *curPageNum = 0;
        return;
    }
    
    if (pageState == ZLViewEnums_PageIndex::next) {
        pageIndex++;
        pageIndex = pageIndex <= quantity.quantity ? pageIndex : quantity.quantity;
    } else if (pageState == ZLViewEnums_PageIndex::previous) {
        pageIndex--;
        pageIndex = pageIndex > 0 ? pageIndex : 1;
    }
    
    *curPageNum = pageIndex;
}

- (BOOL)isCurrentPage:(ZDPageTag *)pageTag
      betweenStartLoc:(ZDEPubLocation *)startLoc
               endLoc:(ZDEPubLocation *)endLoc {
    NSInteger curELen = [NSString stringWithFormat:@"%d", pageTag.eIndex].length;
    NSInteger curCLen = [NSString stringWithFormat:@"%d", pageTag.cIndex].length;
    NSInteger startELen = [NSString stringWithFormat:@"%d", startLoc.eIndex].length;
    NSInteger startCLen = [NSString stringWithFormat:@"%d", startLoc.cIndex].length;
    NSInteger endELen = [NSString stringWithFormat:@"%d", endLoc.eIndex].length;
    NSInteger endCLen = [NSString stringWithFormat:@"%d", endLoc.cIndex].length;
    NSInteger maxELen = MAX(MAX(curELen, startELen), endELen);
    NSInteger maxCLen = MAX(MAX(curCLen, startCLen), endCLen);
    
    NSInteger pBits = powf(10, maxELen + maxCLen);
    NSInteger eBits = powf(10, maxCLen);
    NSInteger curValue = pageTag.paraIndex * pBits + pageTag.eIndex * eBits + pageTag.cIndex;
    NSInteger startValue = startLoc.pIndex * pBits + startLoc.eIndex * eBits + startLoc.cIndex;
    NSInteger endValue = endLoc.pIndex * pBits + endLoc.eIndex * eBits + endLoc.cIndex;
    
    if (startValue <= curValue && curValue < endValue) {
        return YES;
    } else {
        return NO;
    }
}

#pragma mark - touch handler
- (BOOL)respondLongPressAtPos:(CGPoint)pos {
    return _readerApp->onFingerLongPress(pos.x, pos.y);
}

- (void)respondPressAtPos:(CGPoint)pos {
    _readerApp->onFingerPress(pos.x, pos.y);
}

- (void)respondTouchMoved:(CGPoint)pos {
    _readerApp->onFingerMove(pos.x, pos.y);
}

- (BOOL)isPressedOnCursor:(CGPoint)pos {
    return _readerApp->isPressedOnCursor(pos.x, pos.y);
}

- (void)respondFingerReleasedAfterLongPress:(CGPoint)pos {
    _readerApp->onFingerReleaseAfterLongPress(pos.x, pos.y);
}

- (void)respondFingerReleased:(CGPoint)pos {
    _readerApp->onFingerRelease(pos.x, pos.y);
}

- (BOOL)respondSingleTapAtPos:(CGPoint)pos {
    return _readerApp->onFingerSingleTap(pos.x, pos.y);
}

- (BOOL)isTapOnSelectedRegion:(CGPoint)pos {
    return _readerApp->isPressedOnSelectedRegion(pos.x, pos.y);
}

@end
