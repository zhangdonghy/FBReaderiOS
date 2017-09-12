//
//  ZDOCBook.h
//  ZDLibrary
//
//  Created by 蒋晨霄 on 2017/1/10.
//  Copyright © 2017年 蒋晨霄. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "ZDEPubConfig.h"
#import "ZDRootCatalog.h"
#import "ZDBookmark.h"
#import "ZDNote.h"
#import "ZDOpinion.h"

@interface ZDOCBook : NSObject

- (void)updateFromBook:(ZDOCBook *)book;
- (void)updateFromServer:(NSDictionary *)bookData;
- (void)updateDetailFromServer:(NSDictionary *)detailData;

@property (assign) NSInteger bookId;
@property (copy) NSString* name;
@property (copy) NSString* author;
@property (copy) NSString* desc;
@property (copy) NSString* coverUrl;
@property (assign) float score;
@property (copy) NSString* publisher;
@property (assign) NSInteger publishTime;
@property (assign) BOOL isEndRent;
@property (assign) ZD_BOOK_LANG lang;
@property (copy) NSString* voiceUrl;

- (NSDictionary *)serializeToDict;
- (void)deserializeFromDict:(NSDictionary *)bookDict;

// 目录(文字)
@property (copy) NSString* catalogText;

// 目录
@property (strong) ZDRootCatalog* rootCatalog;
- (void)updateCatalogFromServer:(NSDictionary *)bookCatalogData;

// 书签
@property (readonly) NSMutableArray* allBookmarks;
- (void)updateBookmarksFromServer:(NSArray *)bookmarksData;
- (void)addBookmark:(ZDBookmark *)bookmark;
- (void)removeBookmarks:(NSArray *)bookmarkIds;

// 划线
@property (readonly) NSMutableArray* allLinedTexts;
- (void)loadLinedTextsFromLocal:(NSArray *)linedTexts;
- (void)addLinedText:(ZDLinedText *)linedText;
- (void)removeLinedText:(ZDLinedText *)linedText;
// EPub
- (ZDLinedText *)linedTextFromStart:(ZDEPubLocation *)startLoc toEnd:(ZDEPubLocation *)endLoc;
// PDF
- (ZDLinedText *)linedTextFromStart:(ZDEPubLocation *)startLoc toEnd:(ZDEPubLocation *)endLoc inPage:(int)pageIndex;

// 笔记
@property (readonly) NSMutableArray* allNotes;
- (void)updateNotesFromServer:(NSArray *)notesData;
- (void)addNote:(ZDNote *)note;
- (void)removeNote:(ZDNote *)note;
// EPub
- (NSArray *)notesBetweenStart:(ZDEPubLocation *)startLoc toEnd:(ZDEPubLocation *)endLoc;
// PDF
- (NSArray *)notesBetweenStart:(ZDEPubLocation *)startLoc toEnd:(ZDEPubLocation *)endLoc inPage:(int)pageIndex;

// 看法
@property (readonly) NSMutableArray* allOpinions;
- (void)updateOpinionsFromServer:(NSArray *)opinionsData;
- (NSArray *)opinionsInCatalog:(NSInteger)catalogId;
- (NSArray *)opinionsInParagraph:(int)pIndex;
- (NSArray *)opinionsInPage:(int)pageIndex;
- (void)addOpinion:(ZDOpinion *)opinion;
- (void)removeOpinion:(ZDOpinion *)opinion;

@end
