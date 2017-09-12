//
//  ZDEPubCatalogHandler.h
//  ZDLibrary
//
//  Created by 蒋晨霄 on 2017/8/23.
//  Copyright © 2017年 蒋晨霄. All rights reserved.
//

#import "ZDReaderCatalogHandler.h"

#import "ZDFBReaderInterface.h"

@interface ZDEPubCatalogHandler : ZDReaderCatalogHandler

- (void)saveCatalogsOfBook:(NSInteger)bookId fromFBReader:(ZDFBReaderInterface *)interface;

@end
