//
//  ZDEPubCatalogHandler.m
//  ZDLibrary
//
//  Created by 蒋晨霄 on 2017/8/23.
//  Copyright © 2017年 蒋晨霄. All rights reserved.
//

#import "ZDEPubCatalogHandler.h"

#include "ECatalogTree.hpp"

@implementation ZDEPubCatalogHandler

- (void)saveCatalogsOfBook:(NSInteger)bookId fromFBReader:(ZDFBReaderInterface *)interface {
    NSMutableArray* catalogs = [NSMutableArray array];
    
    ECatalogTree* catalogTree = interface.catalogTree;
    [self serializeCatalog:catalogTree->rootNode toArr:catalogs];
    NSLog(@"save catalogs: %@", catalogs);

    [self saveAndUploadCatalogs:catalogs toBook:bookId];
}

- (void)serializeCatalog:(ECatalog *)catalog toArr:(NSMutableArray *)allCatalogs {
    if (catalog->cId > 0) {
        // cId>0，不是rootNode，要进行输出
        NSString* name = [NSString stringWithUTF8String:catalog->name.c_str()];
        [allCatalogs addObject:@{ZD_CATALOG_TYPE_KEY: @(ZD_BOOK_TYPE_EPUB),
                                 ZD_CATALOG_NAME_KEY: name,
                                 ZD_CATALOG_PARAGRAH_INDEX_KEY: @(catalog->pIndex),
                                 ZD_CATALOG_ID_KEY: @(catalog->cId),
                                 ZD_CATALOG_PARENT_ID_KEY: @(catalog->fatherNode->cId),
                                 ZD_CATALOG_LV_KEY: @(catalog->lv)}];
    }
    
    for (std::vector<ECatalog*>::iterator it = catalog->childNodes.begin(); it != catalog->childNodes.end(); it++) {
        [self serializeCatalog:*it toArr:allCatalogs];
    }
}

@end
