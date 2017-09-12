//
//  ZDPluginCollection.m
//  ZDStudent
//
//  Created by zhangdong on 16/8/15.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "FormatPlugin.h"
#include "Book.h"
#include "BookModel.h"
#include "FormatPlugin.h"
#include "ZLibrary.h"
//#include "PluginCollection.h"
#import <Foundation/Foundation.h>
#include "ZDZLTree.h"

#import "ZDFBReader.h"
#import "ZDPluginCollection.h"
@interface ZDFBReader () {
  //  ZDAccountServer* _accountServer;
    FormatPlugin*l;
}

@end
@implementation ZDFBReader

- (void)readModel
{
    [ZDPluginCollection sharedInstance];
    // cache dir
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory,NSUserDomainMask, YES);
    NSString *cacheDirectory = [paths objectAtIndex:0];
    // init library
    int argc =0;
    char** argv=0;
    ZLibrary::init(argc, argv);
    ZLibrary::initApplication("FBReader");
   // Book::cre
    // book path
    NSString* bookFullPath = [[NSBundle mainBundle] pathForResource:@"从春秋到战国" ofType:@"epub" inDirectory:nil];
    shared_ptr<Book> book =  Book::loadFromBookPath([bookFullPath UTF8String]);
    shared_ptr<BookModel> bookModel = new BookModel(book,[cacheDirectory UTF8String]);
    // get plugin
    PluginCollection& p2 = PluginCollection::Instance();
    std::vector<shared_ptr<FormatPlugin> >  plugins = p2.plugins();
    shared_ptr<FormatPlugin> one = plugins[0]; // just one
    one->readModel(*bookModel);
    bookModel->flush();
    
    
}

@end


