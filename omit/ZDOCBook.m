//
//  ZDOCBook.m
//  ZDLibrary
//
//  Created by 蒋晨霄 on 2017/1/10.
//  Copyright © 2017年 蒋晨霄. All rights reserved.
//

#import "ZDOCBook.h"

@implementation ZDOCBook

- (id)init {
    if (self = [super init]) {
        [self initData];
    }
    return self;
}

- (void)initData {
    _lang = ZD_BOOK_LANG_CN;
    _allBookmarks = [NSMutableArray array];
    _allOpinions = [NSMutableArray array];
    _allLinedTexts = [NSMutableArray array];
    _allNotes = [NSMutableArray array];
}

- (void)updateFromBook:(ZDOCBook *)book {
    _name = book.name.length > 0 ? book.name : _name;
    _author = book.author.length > 0 ? book.author : _author;
    _desc = book.desc.length > 0 ? book.desc : _desc;
    _coverUrl = book.coverUrl.length > 0 ? book.coverUrl : _coverUrl;
    _isEndRent = book.isEndRent;
    
    _score = book.score > 0 ? book.score : _score;
    _publisher = book.publisher.length > 0 ? book.publisher : _publisher;
    _publishTime = book.publishTime > 0 ? book.publishTime : _publishTime;
    
    _catalogText = book.catalogText.length > 0 ? book.catalogText : _catalogText;
    if (book.rootCatalog) {
        _rootCatalog = [[ZDRootCatalog alloc] init];
        [_rootCatalog updateFromCatalog:book.rootCatalog];
    }
}

- (void)updateFromServer:(NSDictionary *)bookData {
    _bookId = [bookData[@"BookId"] integerValue];
    _name = [NSString forcedString:bookData[@"Name"]];
    _author = [NSString forcedString:bookData[@"Author"]];
    _desc = [NSString forcedString:bookData[@"Introduction"]];
    _coverUrl = [NSString forcedString:bookData[@"Cover"]];
    _isEndRent = [bookData[@"EndRent"] boolValue];
}

- (void)updateDetailFromServer:(NSDictionary *)detailData {
    [self updateFromServer:detailData];

    _score = [detailData[@"AvgScore"] floatValue];
    _publisher = [NSString forcedString:detailData[@"Publish"]];
    _publishTime = [detailData[@"PublishTime"] integerValue];

    [self updateCatalogFromServer:detailData];
}

#pragma mark - catalog
- (void)updateCatalogFromServer:(NSDictionary *)bookCatalogData {
    if (bookCatalogData[@"BookCatalog"]) {
        _catalogText = bookCatalogData[@"BookCatalog"];
    }
    
    NSArray* catalogsData = bookCatalogData[@"ChapterList"];
    if (catalogsData.count > 0) {
        _rootCatalog = [[ZDRootCatalog alloc] init];
        [_rootCatalog updateMultiLvCatalogsFromServer:catalogsData];
    }
}

- (NSArray *)serializeCatalogs {
    NSMutableArray* allCatalogs = [NSMutableArray array];
    [self serializeCatalog:_rootCatalog toArr:allCatalogs];
    return allCatalogs;
}

- (void)serializeCatalog:(ZDCatalog *)catalog toArr:(NSMutableArray *)allCatalogs {
    if (catalog.catalogId > 0) {
        // catalogId>0，不是rootCatalog，要进行输出
        NSLog(@"catalog: name: %@, paraIndex: %d, pageIndex: %d, cId: %d, parentCId:%d, lv: %d",
              catalog.name, catalog.paraIndex, catalog.pageIndex, catalog.catalogId, catalog.parentId, catalog.lv);
        
        [allCatalogs addObject:@{ZD_CATALOG_TYPE_KEY: @(catalog.bookType),
                                 ZD_CATALOG_NAME_KEY: catalog.name,
                                 ZD_CATALOG_PARAGRAH_INDEX_KEY: @(catalog.paraIndex),
                                 ZD_CATALOG_PAGE_KEY: @(catalog.pageIndex),
                                 ZD_CATALOG_ID_KEY: @(catalog.catalogId),
                                 ZD_CATALOG_PARENT_ID_KEY: @(catalog.parentId),
                                 ZD_CATALOG_LV_KEY: @(catalog.lv)}];
    }

    for (ZDCatalog* childCatalog in catalog.childCatalogs) {
        [self serializeCatalog:childCatalog toArr:allCatalogs];
    }
}

#pragma mark - bookmark
- (void)updateBookmarksFromServer:(NSArray *)bookmarksData {
    [_allBookmarks removeAllObjects];

    for (NSDictionary* bookmarkData in bookmarksData) {
        ZDBookmark* bookmark = [[ZDBookmark alloc] init];
        [bookmark updateFromServer:bookmarkData];
        [_allBookmarks addObject:bookmark];
    }
}

- (void)addBookmark:(ZDBookmark *)bookmark {
    [_allBookmarks addObject:bookmark];
}

- (void)removeBookmarks:(NSArray *)bookmarkIds {
    for (NSNumber* idNum in bookmarkIds) {
        NSInteger bookmarkId = [idNum integerValue];
        for (ZDBookmark* bookmark in _allBookmarks) {
            if (bookmark.bookmarkId == bookmarkId) {
                [_allBookmarks removeObject:bookmark];
                break;
            }
        }
    }
}

#pragma mark - lined text
- (void)loadLinedTextsFromLocal:(NSArray *)linedTexts {
    [_allLinedTexts removeAllObjects];
    [_allLinedTexts addObjectsFromArray:linedTexts];
}

- (void)addLinedText:(ZDLinedText *)linedText {
    [_allLinedTexts addObject:linedText];
}

- (void)removeLinedText:(ZDLinedText *)linedText {
    [_allLinedTexts removeObject:linedText];
}

- (ZDLinedText *)linedTextFromStart:(ZDEPubLocation *)startLoc toEnd:(ZDEPubLocation *)endLoc {
    ZDLinedText* compareLinedText = [[ZDLinedText alloc] init];
    compareLinedText.bookType = ZD_BOOK_TYPE_EPUB;
    [compareLinedText setStartLocWithParagraph:startLoc.pIndex element:startLoc.eIndex character:startLoc.cIndex];
    [compareLinedText setEndLocWithParagraph:endLoc.pIndex element:endLoc.eIndex character:endLoc.cIndex];
    
    for (ZDLinedText* linedText in _allLinedTexts) {
        if ([linedText isEqualToLinedText:compareLinedText]) {
            return linedText;
        }
    }
    return nil;
}

- (ZDLinedText *)linedTextFromStart:(ZDEPubLocation *)startLoc toEnd:(ZDEPubLocation *)endLoc inPage:(int)pageIndex {
    ZDLinedText* compareLinedText = [[ZDLinedText alloc] init];
    compareLinedText.bookType = ZD_BOOK_TYPE_PDF;
    compareLinedText.pageIndex = pageIndex;
    [compareLinedText setStartLocWithParagraph:startLoc.pIndex element:startLoc.eIndex character:startLoc.cIndex];
    [compareLinedText setEndLocWithParagraph:endLoc.pIndex element:endLoc.eIndex character:endLoc.cIndex];

    for (ZDLinedText* linedText in _allLinedTexts) {
        if ([linedText isEqualToLinedText:compareLinedText]) {
            return linedText;
        }
    }
    return nil;
}

#pragma mark - note
- (void)updateNotesFromServer:(NSArray *)notesData {
    [_allNotes removeAllObjects];
    
    for (NSDictionary* noteData in notesData) {
        ZDNote* note = [[ZDNote alloc] init];
        [note updateFromServer:noteData];
        [_allNotes addObject:note];
    }
}

- (void)addNote:(ZDNote *)note {
    [_allNotes addObject:note];
}

- (void)removeNote:(ZDNote *)note {
    for (ZDNote* curNote in _allNotes) {
        if (curNote.noteId == note.noteId) {
            [_allNotes removeObject:curNote];
            break;
        }
    }
}

- (NSArray *)notesBetweenStart:(ZDEPubLocation *)startLoc toEnd:(ZDEPubLocation *)endLoc {
//    NSLog(@"find notes between start: p:%d, e:%d, c:%d; to end: p:%d, e:%d, c:%d",
//          startLoc.pIndex, startLoc.eIndex, startLoc.cIndex, endLoc.pIndex, endLoc.eIndex, endLoc.cIndex);
    NSMutableArray* notes = [NSMutableArray array];
    for (ZDNote* note in _allNotes) {
//        NSLog(@"note start: p:%d, e:%d, c:%d; to end: p:%d, e:%d, c:%d",
//              note.startLoc.pIndex, note.startLoc.eIndex, note.startLoc.cIndex,
//              note.endLoc.pIndex, note.endLoc.eIndex, note.endLoc.cIndex);
        if ([note isBetweenStart:startLoc toEnd:endLoc]) {
            [notes addObject:note];
        }
    }
    return notes;
}

- (NSArray *)notesBetweenStart:(ZDEPubLocation *)startLoc toEnd:(ZDEPubLocation *)endLoc inPage:(int)pageIndex {
//    NSLog(@"find notes between start: p:%d, e:%d, c:%d; to end: p:%d, e:%d, c:%d, in page: %d",
//          startLoc.pIndex, startLoc.eIndex, startLoc.cIndex, endLoc.pIndex, endLoc.eIndex, endLoc.cIndex, pageIndex);
    NSMutableArray* notes = [NSMutableArray array];
    for (ZDNote* note in _allNotes) {
//        NSLog(@"note start: p:%d, e:%d, c:%d; to end: p:%d, e:%d, c:%d page: %d",
//              note.startLoc.pIndex, note.startLoc.eIndex, note.startLoc.cIndex,
//              note.endLoc.pIndex, note.endLoc.eIndex, note.endLoc.cIndex, note.pageIndex);
        if ([note isBetweenStart:startLoc toEnd:endLoc inPage:pageIndex]) {
            [notes addObject:note];
        }
    }
    return notes;
}

#pragma mark - opinion
- (void)updateOpinionsFromServer:(NSArray *)opinionsData {
    [_allOpinions removeAllObjects];
    
    for (NSDictionary* opinionData in opinionsData) {
        ZDOpinion* opinion = [[ZDOpinion alloc] init];
        [opinion updateFromServer:opinionData];
        [_allOpinions addObject:opinion];
    }
}

- (NSArray *)opinionsInCatalog:(NSInteger)catalogId {
    NSMutableArray* opinions = [NSMutableArray array];
    for (ZDOpinion* opinion in _allOpinions) {
        if (opinion.catalogId == catalogId) {
            [opinions addObject:opinion];
        }
    }
    return opinions;
}

- (NSArray *)opinionsInParagraph:(int)pIndex {
    NSMutableArray* opinions = [NSMutableArray array];
    for (ZDOpinion* opinion in _allOpinions) {
        if (opinion.startLoc.pIndex == pIndex) {
            [opinions addObject:opinion];
        }
    }
    return opinions;
}

- (NSArray *)opinionsInPage:(int)pageIndex {
    NSMutableArray* opinions = [NSMutableArray array];
    for (ZDOpinion* opinion in _allOpinions) {
        if (opinion.page == pageIndex) {
            [opinions addObject:opinion];
        }
    }
    return opinions;
}

- (void)addOpinion:(ZDOpinion *)opinion {
    [_allOpinions addObject:opinion];
}

- (void)removeOpinion:(ZDOpinion *)opinion {
    [_allOpinions removeObject:opinion];
}

#pragma mark - serialize/deserialize
- (NSDictionary *)serializeToDict {
    NSMutableDictionary* bookData = [NSMutableDictionary dictionary];
    
    bookData[@"BookId"] = @(_bookId);
    bookData[@"Name"] = _name;
    bookData[@"Author"] = _author;
    bookData[@"Introduction"] = _desc;
    bookData[@"Cover"] = _coverUrl;
    bookData[@"EndRent"] = @(_isEndRent);
    bookData[@"AvgScore"] = @(_score);
    bookData[@"Publish"] = _publisher;
    bookData[@"PublishTime"] = @(_publishTime);
    bookData[@"BookCatalog"] = _catalogText;
    bookData[@"ChapterList"] = [self serializeCatalogs];

    if (_allBookmarks.count) {
        NSMutableArray* bookmarksData = [NSMutableArray array];
        for (ZDBookmark* bookmark in _allBookmarks) {
            [bookmarksData addObject:[bookmark serializeToDict]];
        }
        bookData[@"BookmarkList"] = bookmarksData;
    }
    if (_allNotes.count) {
        NSMutableArray* notesData = [NSMutableArray array];
        for (ZDNote* note in _allNotes) {
            [notesData addObject:[note serializeToDict]];
        }
        bookData[@"NoteList"] = notesData;
    }
    if (_allOpinions.count) {
        NSMutableArray* opinionsData = [NSMutableArray array];
        for (ZDOpinion* opinion in _allOpinions) {
            [opinionsData addObject:[opinion serializeToDict]];
        }
        bookData[@"OpinionList"] = opinionsData;
    }

    return bookData;
}

- (void)deserializeFromDict:(NSDictionary *)bookDict {
    if (bookDict.count <= 0) {
        return;
    }

    [self updateDetailFromServer:bookDict];
    [self updateBookmarksFromServer:bookDict[@"BookmarkList"]];
    [self updateOpinionsFromServer:bookDict[@"OpinionList"]];

    NSArray* notesData = bookDict[@"NoteList"];
    [_allNotes removeAllObjects];
    for (NSDictionary* noteData in notesData) {
        ZDNote* note = [[ZDNote alloc] init];
        [note deserializeFromDict:noteData];
        [_allNotes addObject:note];
    }
}

@end
