//
//  ZDEPubReaderModel.m
//  ZDLibrary
//
//  Created by 蒋晨霄 on 2017/2/21.
//  Copyright © 2017年 蒋晨霄. All rights reserved.
//

#import "ZDEPubReaderModel.h"

#import "ZDEPubManager.h"
#import "ZDEPubMainController.h"
#import "ZDFBReaderInterface.h"
#import "ZDReaderOpinion.h"
#import "ZDPageCountModel.h"
#import "ZDEPubCatalogHandler.h"

@interface ZDEPubReaderModel () <ZDFBReaderDelegate>

@property (strong) ZDFBReaderInterface* interface;
@property (weak) ZDEPubMainController* mainCtrl;
@property (assign) BOOL isChangingPage;

@property (strong) NSMutableDictionary* readerOpinions;

@property (strong) ZDPageCountModel* pageCountModel;
@property (strong) ZDEPubCatalogHandler* catalogHandler;

@end

@implementation ZDEPubReaderModel

- (id)initWithMainCtrl:(ZDEPubMainController *__weak)mainCtrl book:(ZDOCBook *)book path:(NSString *)path {
    if (self = [super init]) {
        [self initDataWithMainCtrl:mainCtrl book:book path:path];
    }
    return self;
}

- (void)dealloc {
    NSLog(@"dealloc ZDEPubReaderModel");
}

- (void)initDataWithMainCtrl:(ZDEPubMainController *__weak)mainCtrl book:(ZDOCBook *)book path:(NSString *)path {
    _mainCtrl = mainCtrl;
    _book = book;
    _bookPath = path;
    _interface = [[ZDFBReaderInterface alloc] initWithDelegate:self];
    _listenEnabled = NO;
    _readerOpinions = [NSMutableDictionary dictionary];
    _catalogHandler = [[ZDEPubCatalogHandler alloc] init];

    [self loadLinedTextsFromLocal];
    [self loadOpinionQuantities];
}

- (BOOL)openBookAtParagraph:(int)pIndex {
    BOOL success = [_interface openBook:_book atPath:_bookPath jumpParagraph:pIndex];
    if (!success) {
        return NO;
    }
    
    [_interface addLinedTexts:_book.allLinedTexts];
    [_interface addOpinions:_readerOpinions];
    
    if (_book.lang == ZD_BOOK_LANG_CN && _book.voiceUrl.length <= 0) {
        _listenEnabled = YES;
    } else if (_book.lang == ZD_BOOK_LANG_UNKNOWN) {
        // 进入阅读时仍然未知书籍的语言，则通过epub的信息判断是否有听书功能
        _listenEnabled = !_interface.isEnglishBook;
    }

    return YES;
}

- (void)addReaderActions {
    [_interface addActionsWithCtrl:_mainCtrl];
}

#pragma mark - draw
- (UIImage *)bitmapOfPageState:(ZD_EPUB_PAGE_STATE)pageState withViewSize:(CGSize)viewSize {
    return [_interface bitmapOfPageState:pageState withViewSize:viewSize];
}

- (void)repaint {
    [_mainCtrl repaint];
}

#pragma mark - page control
- (BOOL)hasPrevPage {
    return _interface.hasPrevPage;
}

- (BOOL)hasNextPage {
    return _interface.hasNextPage;
}

- (void)changePrevPage {
    if (!self.hasPrevPage) {
        return;
    }
    if (_isChangingPage) {
        return;
    }

    _isChangingPage = YES;
    [_mainCtrl changePrevPage];
}

- (void)finishChangePrevPage:(ZD_EPUB_PAGE_STATE)pageState {
    [_interface didFinishChangePageAtState:pageState];
    [_mainCtrl didChangePrevPage];

    [self recordReadLocation:nil];

    _isChangingPage = NO;
}

- (void)changeNextPage {
    if (!self.hasNextPage) {
        return;
    }
    if (_isChangingPage) {
        return;
    }

    _isChangingPage = YES;
    [_mainCtrl changeNextPage];
}

- (void)finishChangeNextPage:(ZD_EPUB_PAGE_STATE)pageState {
    [_interface didFinishChangePageAtState:pageState];
    [_mainCtrl didChangeNextPage];

    [self recordReadLocation:nil];

    _isChangingPage = NO;
}

#pragma mark - text
- (NSString *)selectedText {
    return _interface.selectedText;
}

- (int)selectedTextCount {
    return _interface.selectedTextCount;
}

- (void)clearSelectedParagraph {
    [_interface clearSelectedParagraph];
}

- (ZDEPubLocation *)lastLocationOfParagraph:(int)pIndex {
    int eIndex = 0;
    int cIndex = 0;
    [_interface getElement:&eIndex character:&cIndex ofParagraph:pIndex];

    ZDEPubLocation* location = [[ZDEPubLocation alloc] init];
    location.bookType = ZD_BOOK_TYPE_EPUB;
    location.pIndex = pIndex;
    location.eIndex = eIndex;
    location.cIndex = cIndex;
    return location;
}

#pragma mark - location
- (void)recordReadLocation:(ZDEPubLocation *)location {
    if (!location) {
        ZDPageTag* pageTag = self.curPageTag;
        location = [[ZDEPubLocation alloc] init];
        location.bookId = _book.bookId;
        location.bookType = ZD_BOOK_TYPE_EPUB;
        location.pIndex = pageTag.paraIndex;
        location.eIndex = pageTag.eIndex;
        location.cIndex = pageTag.cIndex;
    }
    
    [[ZDEPubManager sharedManager] recordReadLocation:location];
}

- (void)jumpToLocation:(ZDEPubLocation *)location {
    if (location) {
        [_interface jumpToLocation:location];
        [self recordReadLocation:location];
    }

    [_mainCtrl finishJumpLocation];
}

- (NSInteger)curPercent {
    return _interface.percent;
}

- (void)jumpToPercent:(NSInteger)percent {
    [_interface jumpToPercent:percent];
    [self recordReadLocation:nil];
    [_mainCtrl finishJumpLocation];
}

- (void)jumpToPrevCatalog {
    ZDPageTag* pageTag = _interface.curPageTag;
    ZDCatalog* catalog = [_book.rootCatalog findCatalogOfParagraph:pageTag.paraIndex];
    ZDCatalog* prevCatalog = [_book.rootCatalog prevCatalog:catalog];

    ZDEPubLocation* location = [[ZDEPubLocation alloc] init];
    location.bookType = ZD_BOOK_TYPE_EPUB;
    location.pIndex = prevCatalog ? prevCatalog.paraIndex : 0;
    location.eIndex = 0;
    location.cIndex = 0;
    [self jumpToLocation:location];
}

- (void)jumpToNextCatalog {
    ZDPageTag* pageTag = _interface.curPageTag;
    ZDCatalog* catalog = [_book.rootCatalog findCatalogOfParagraph:pageTag.paraIndex];
    ZDCatalog* nextCatalog = nil;
    if (catalog) {
        nextCatalog = [_book.rootCatalog nextCatalog:catalog];
    } else if (_book.rootCatalog.childCatalogs.count > 0) {
        nextCatalog = _book.rootCatalog.childCatalogs[0];
    }
    if (!nextCatalog) {
        return;
    }

    ZDEPubLocation* location = [[ZDEPubLocation alloc] init];
    location.bookType = ZD_BOOK_TYPE_EPUB;
    location.pIndex = nextCatalog.paraIndex;
    location.eIndex = 0;
    location.cIndex = 0;
    [self jumpToLocation:location];
}

#pragma mark - catalog
- (ZDCatalog *)catalogOfCurPage {
    ZDPageTag* pageTag = self.curPageTag;
    return [_book.rootCatalog findCatalogOfParagraph:pageTag.paraIndex];
}

- (NSString *)catalogNameOfCurPage {
    ZDCatalog* catalog = self.catalogOfCurPage;
    if (catalog) {
        return catalog.name;
    } else {
        return @"正文";
    }
}

- (void)saveAllCatalogs {
    [_catalogHandler saveCatalogsOfBook:_book.bookId fromFBReader:_interface];
}

#pragma mark - font
- (void)refreshFontSize:(BOOL)hasChange {
    int fontSize = (int)[ZDEPubManager sharedManager].fontSize;
    [_interface setFontSize:fontSize];
    
    if (hasChange) {
        [self recalculatePageQuantity];
    }
}

- (void)refreshFontName:(BOOL)hasChange {
    ZDEPubFont* curFont = [ZDEPubManager sharedManager].curFont;
    [_interface setFontName:curFont.name];

    if (hasChange) {
        [self recalculatePageQuantity];
    }
}

#pragma mark - theme
- (void)refreshTheme {
    if (![ZDEPubManager sharedManager].isLightOn) {
        ZDEPubTheme* curTheme = [ZDEPubManager sharedManager].curTheme;
        [_interface changeBackgroundColorWithRed:curTheme.red green:curTheme.green blue:curTheme.blue];
        [_interface changeTextColorWithRed:0 green:0 blue:0];
    }
}

- (void)refreshLightState {
    if ([ZDEPubManager sharedManager].isLightOn) {
        [_interface changeBackgroundColorWithRed:0 green:0 blue:0];
        [_interface changeTextColorWithRed:255 green:255 blue:255];
    } else {
        [self refreshTheme];
    }
}

#pragma mark - line space
- (void)refreshLayout:(BOOL)hasChange {
    ZDEPubLayout* curLayout = [ZDEPubManager sharedManager].curLayout;
    [_interface setLineSpace:curLayout.lineSpace];

    if (hasChange) {
        [self recalculatePageQuantity];
    }
}

#pragma mark - bookmark
- (ZDPageTag *)curPageTag {
    return _interface.curPageTag;
}

- (BOOL)isBookmarkInCurPage:(ZDBookmark *)bookmark {
    return [_interface isBookmarkInCurPage:bookmark];
}

- (NSArray *)bookmarksOnCurPage {
    NSMutableArray* bookmarkIds = [NSMutableArray array];
    for (ZDBookmark* bookmark in _book.allBookmarks) {
        if ([self isBookmarkInCurPage:bookmark]) {
            [bookmarkIds addObject:[NSNumber numberWithInteger:bookmark.bookmarkId]];
        }
    }
    return bookmarkIds;
}

- (ZDBookmark *)createBookmarkOnCurPage {
    ZDPageTag* pageTag = self.curPageTag;
    ZDCatalog* catalog = [_book.rootCatalog findCatalogOfParagraph:pageTag.paraIndex];
    
    ZDBookmark* bookmark = [[ZDBookmark alloc] init];
    [bookmark updateFromPageTag:pageTag];
    bookmark.catalogId = catalog.catalogId;
    
    return bookmark;
}

#pragma mark - lined text
- (ZDUnderlineColor *)colorOfLinedTextFromStart:(ZDEPubLocation *)start toEnd:(ZDEPubLocation *)end {
    NSInteger colorId = 0;
    ZDLinedText* linedText = [_book linedTextFromStart:start toEnd:end];
    if (linedText) {
        colorId = linedText.colorId;
    }
    return [[ZDEPubManager sharedManager] underlineColorOfId:colorId];
}

- (ZDUnderlineColor *)curUnderline {
    return [ZDEPubManager sharedManager].curUnderlineColor;
}

- (void)loadLinedTextsFromLocal {
    // 先用笔记同步本地划线
    [self synchronizeLinedTextsWithNotes];

    // 重新读取本地划线
    NSArray* linedTexts = [[ZDEPubManager sharedManager] linedTextsOfBook:_book.bookId];
    [_book loadLinedTextsFromLocal:linedTexts];
}

- (void)addNewLinedText:(ZDLinedText *)linedText {
    [_book addLinedText:linedText];
    [[ZDEPubManager sharedManager] addLinedText:linedText];
}

- (void)removeLinedText:(ZDLinedText *)linedText {
    [_book removeLinedText:linedText];
    [[ZDEPubManager sharedManager] removeLinedText:linedText];

    [_interface removeLinedText:linedText];
}

- (void)synchronizeLinedTextsWithNotes {
    NSArray* localLinedTexts = [[ZDEPubManager sharedManager] linedTextsOfBook:_book.bookId];
    NSArray* notes = _book.allNotes;
    
    // 每一条笔记相当于有一条划线，因此需与本地划线同步
    NSMutableArray* newLinedTexts = [NSMutableArray array];
    for (ZDNote* note in notes) {
        BOOL hasSameLinedText = NO;
        for (ZDLinedText* linedText in localLinedTexts) {
            if ([linedText isEqualToLinedText:note]) {
                hasSameLinedText = YES;
                break;
            }
        }
        
        // 笔记的坐标与划线不相等，添加划线
        if (!hasSameLinedText) {
            ZDLinedText* linedText = [[ZDLinedText alloc] init];
            linedText.bookId = note.bookId;
            linedText.bookType = note.bookType;
            linedText.quote = note.quote;
            linedText.catalogId = note.catalogId;
            linedText.percent = note.percent;
            linedText.colorId = note.colorId;
            [linedText setStartLocWithParagraph:note.startLoc.pIndex element:note.startLoc.eIndex character:note.startLoc.cIndex];
            [linedText setEndLocWithParagraph:note.endLoc.pIndex element:note.endLoc.eIndex character:note.endLoc.cIndex];
            [newLinedTexts addObject:linedText];
        }
    }
    
    for (ZDLinedText* linedText in newLinedTexts) {
        [[ZDEPubManager sharedManager] addLinedText:linedText];
    }
}

#pragma mark - note
- (void)removeNote:(ZDNote *)note {
    [_book removeNote:note];
    [self repaint];
}

- (NSInteger)countNoteBetweenStart:(ZDEPubLocation *)startLoc toEnd:(ZDEPubLocation *)endLoc {
    NSArray* notes = [_book notesBetweenStart:startLoc toEnd:endLoc];
//    NSLog(@"notes count: %zd", notes.count);
    return notes.count;
}

#pragma mark - opinion
- (BOOL)opinionEnabled {
    return [ZDEPubManager sharedManager].opinionEnabled;
}

- (void)loadOpinionQuantities {
    [_readerOpinions removeAllObjects];
    for (ZDOpinion* opinion in _book.allOpinions) {
        NSString* pKey = [NSString stringWithFormat:@"%d", opinion.startLoc.pIndex];
        ZDReaderOpinion* readerOpinion = _readerOpinions[pKey];
        if (!readerOpinion) {
            readerOpinion = [[ZDReaderOpinion alloc] init];
            readerOpinion.pIndex = opinion.startLoc.pIndex;
            readerOpinion.eStart = opinion.startLoc.eIndex;
            readerOpinion.cStart = opinion.startLoc.cIndex;
            readerOpinion.eEnd = opinion.endLoc.eIndex;
            readerOpinion.cEnd = opinion.endLoc.cIndex;
            readerOpinion.opinionCount = 0;
            _readerOpinions[pKey] = readerOpinion;
        }
        readerOpinion.opinionCount++;
    }
}

- (void)addOpinion:(ZDOpinion *)opinion {
    [_book addOpinion:opinion];

    NSString* pKey = [NSString stringWithFormat:@"%d", opinion.startLoc.pIndex];
    ZDReaderOpinion* readerOpinion = _readerOpinions[pKey];
    ZDFBReaderInterface* model = _interface;
    if (!readerOpinion) {
        readerOpinion = [[ZDReaderOpinion alloc] init];
        readerOpinion.pIndex = opinion.startLoc.pIndex;
        readerOpinion.eStart = opinion.startLoc.eIndex;
        readerOpinion.cStart = opinion.startLoc.cIndex;
        readerOpinion.eEnd = opinion.endLoc.eIndex;
        readerOpinion.cEnd = opinion.endLoc.cIndex;
        readerOpinion.opinionCount = 0;
        _readerOpinions[pKey] = readerOpinion;

        // add opinion to reader
        [model addOpinion:readerOpinion];
    }
    readerOpinion.opinionCount++;

    [self repaint];
}

- (void)removeOpinion:(ZDOpinion *)opinion {
    [_book removeOpinion:opinion];

    NSString* pKey = [NSString stringWithFormat:@"%d", opinion.startLoc.pIndex];
    ZDReaderOpinion* readerOpinion = _readerOpinions[pKey];
    if (!readerOpinion) {
        return;
    }
    readerOpinion.opinionCount--;
    if (readerOpinion.opinionCount <= 0) {
        [_readerOpinions removeObjectForKey:pKey];

        // remove opinion from reader
        [_interface removeOpinion:readerOpinion];
    }

    [self repaint];
}

- (int)countOpinionInParagraph:(int)pIndex {
    if (!self.opinionEnabled) {
        return 0;
    }

    return [self opinionQuantityAtParagraph:pIndex];
}

- (int)opinionQuantityAtParagraph:(int)pIndex {
    ZDReaderOpinion* readerOpinion = _readerOpinions[[NSString stringWithFormat:@"%d", pIndex]];
    if (!readerOpinion) {
        return 0;
    }
    return readerOpinion.opinionCount;
}

- (void)showOpinionsInParagraph:(int)pIndex {
//    NSLog(@"show opinion at p:%d", pIndex);
    [_mainCtrl showOpinionsInParagraph:pIndex];
}

#pragma mark - panel
- (void)showPanelAtLinedText:(ZDLinedText *)linedText {
    // TODO: 检查是否与其他划线合并

    // 补全划线信息
    if (linedText.bookId <= 0) {
        linedText.bookId = _book.bookId;
        linedText.bookType = ZD_BOOK_TYPE_EPUB;
    }

    // 寻找已存在的划线
    ZDLinedText* existedLinedText = [[ZDEPubManager sharedManager] existedSameLinedText:linedText];
    if (!existedLinedText) {
        // 新建划线，添加当前目录信息
        ZDCatalog* catalog = [_book.rootCatalog findCatalogOfParagraph:linedText.endLoc.pIndex];
        linedText.catalogId = catalog.catalogId;
        // 颜色
        linedText.colorId = [ZDEPubManager sharedManager].curUnderlineColorId;

        existedLinedText = linedText;
        [self addNewLinedText:linedText];

    } else {
        // 将点击的划线的坐标信息复制到已存在的划线上
        existedLinedText.xStart = linedText.xStart;
        existedLinedText.yStart = linedText.yStart;
        existedLinedText.xEnd = linedText.xEnd;
        existedLinedText.yEnd = linedText.yEnd;
    }


    // 获取划线范围内的笔记
    NSArray* notes = [_book notesBetweenStart:existedLinedText.startLoc toEnd:existedLinedText.endLoc];

    [_mainCtrl showPanelAtLinedText:existedLinedText withNotes:notes];
}

- (void)showPanel {
    [_interface showPanel];
}

- (void)hidePanel {
    [_mainCtrl hidePanel];
}

#pragma mark - fullscreen image
- (void)showFullScreenImage:(UIImage *)image {
    [_mainCtrl showFullScreenImage:image];
}

#pragma mark - listen
- (void)enterListeningMode {
    [_interface enterListeningMode];
}

- (void)exitListeningMode {
    [_interface exitListeningMode];
}

- (void)getParagraphToListen:(NSString**)para hasNextPara:(BOOL*)hasNextPara {
    [_interface getParagraphToListen:para hasNextPara:hasNextPara];
}

- (BOOL)tryChangeNextPageInListening {
    BOOL hasNextPage = [_interface hasNextPageInListening:_mainCtrl.pageState];
    if (!hasNextPage) {
        return NO;
    }
    
    [_mainCtrl changeNextPageInListening];
    
    return YES;
}

- (int)tapParagraphAtPos:(CGPoint)tapPos {
    return [_interface tapParagraphAtPos:tapPos];
}

- (void)tryStartListeningFromParagraph:(int)pIndex {
    [_interface tryStartListeningFromParagraph:pIndex];

    [_mainCtrl restartListening];
}

#pragma mark - voice
- (BOOL)hasVoiceBar {
    return _book.voiceUrl.length > 0;
}

#pragma mark - magnifier
- (void)showMagnifierAtPos:(CGPoint)pos fromView:(UIView *)pageView {
    CGPoint posInCtrl = [pageView convertPoint:pos toView:_mainCtrl.view];
    [_mainCtrl showMagnifierAtPos:posInCtrl];
}

- (void)moveMagnifierToPos:(CGPoint)pos fromView:(UIView *)pageView {
    CGPoint posInCtrl = [pageView convertPoint:pos toView:_mainCtrl.view];
    [_mainCtrl moveMagnifierToPos:posInCtrl];
}

#pragma mark - calculate page count
- (BOOL)hasPageQuantity {
    return YES;
//    ZDEPubPageQuantity* quantity = [[ZDEPubManager sharedManager] pageQuantityOfBook:_book.bookId];
//    return quantity != nil;
}

- (void)getPageQuantity:(long *)totalCount curNum:(long *)curPageNum ofState:(ZD_EPUB_PAGE_STATE)pageState {
    [_interface countPage:totalCount curNum:curPageNum ofState:pageState];

//    ZDEPubPageQuantity* quantity = [[ZDEPubManager sharedManager] pageQuantityOfBook:_book.bookId];
//    if (!quantity) {
//        *totalCount = 0;
//        *curPageNum = 0;
//        return;
//    }

//    [_interface countPage:totalCount curNum:curPageNum ofState:pageState inQuantity:quantity];
}

- (void)tryToCountPageWithViewSize:(CGSize)viewSize {
    if (_isCalculatingPageQuantity) {
        return;
    }
    
    _isCalculatingPageQuantity = YES;
    if (!_pageCountModel) {
        _pageCountModel = [[ZDPageCountModel alloc] initWithBook:_book.bookId viewSize:viewSize];
        [_interface connectReaderAppToPageCount:_pageCountModel];
    }
    
    [NSTimer scheduledTimerWithTimeInterval:0.5
                                     target:self
                                   selector:@selector(beginCalculatePageQuantity)
                                   userInfo:nil
                                    repeats:NO];
}

- (void)beginCalculatePageQuantity {
    [_pageCountModel calculatePageCountWithHandler:^(BOOL isSuccess) {
        _isCalculatingPageQuantity = NO;
        
//        NSLog(@"finish calculate with isSuccess: %d", isSuccess);
        if (isSuccess) {
        }
    }];
}

- (void)stopCalculatePageQuantity {
    [_pageCountModel stopCalculate:NO];
}

- (void)recalculatePageQuantity {
    if (_isCalculatingPageQuantity) {
        [_pageCountModel stopCalculate:YES];
        
    } else {
        [_mainCtrl refreshFooterView];
    }
}

#pragma mark - touch handler
- (BOOL)respondLongPressAtPos:(CGPoint)pos fromView:(UIView *)pageView {
//    NSLog(@"respond long press in ZDEPubReaderModel");
    BOOL isSuccess = [_interface respondLongPressAtPos:pos];

    if (isSuccess) {
        [self showMagnifierAtPos:pos fromView:pageView];
    }

    return isSuccess;
}

- (void)respondPressAtPos:(CGPoint)pos {
    [_interface respondPressAtPos:pos];
}

- (void)respondTouchMoved:(CGPoint)pos fromView:(UIView *)pageView {
    NSLog(@"respond touch moved in ZDEPubReaderModel");
    [_interface respondTouchMoved:pos];

    [self moveMagnifierToPos:pos fromView:pageView];
}

- (BOOL)isPressedOnCursor:(CGPoint)pos {
    return [_interface isPressedOnCursor:pos];
}

- (void)respondFingerReleasedAfterLongPress:(CGPoint)pos {
    [_interface respondFingerReleasedAfterLongPress:pos];
    
    [_mainCtrl hideMagnifier];
}

- (void)respondFingerReleased:(CGPoint)pos {
    [_interface respondFingerReleased:pos];
    
    [_mainCtrl hideMagnifier];
}

- (BOOL)respondSingleTapAtPos:(CGPoint)pos {
    return [_interface respondSingleTapAtPos:pos];
}

- (BOOL)isTapOnSelectedRegion:(CGPoint)pos {
    return [_interface isTapOnSelectedRegion:pos];
}

#pragma mark - ZDFBReaderDelegate
- (void)repaintFromReader {
    [self repaint];
}

- (ZDUnderlineColor *)colorOfLinedTextInReaderFromStart:(ZDEPubLocation *)startLoc toEnd:(ZDEPubLocation *)endLoc {
    return [self colorOfLinedTextFromStart:startLoc toEnd:endLoc];
}

- (void)showPanelFromReaderAtLinedText:(ZDLinedText *)linedText {
    [self showPanelAtLinedText:linedText];
}

- (void)hidePanelFromReader {
    [self hidePanel];
}

- (NSInteger)countNoteInReaderFromStart:(ZDEPubLocation *)startLoc toEnd:(ZDEPubLocation *)endLoc {
    return [self countNoteBetweenStart:startLoc toEnd:endLoc];
}

- (int)countOpinionInReaderAtParagraph:(int)pIndex {
    return [self countOpinionInParagraph:pIndex];
}

- (void)showOpinionsInReaderAtParagraph:(int)pIndex {
    [self showOpinionsInParagraph:pIndex];
}

- (void)showFullScreenImageFromReader:(UIImage *)image {
    [self showFullScreenImage:image];
}

@end
