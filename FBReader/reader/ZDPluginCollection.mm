//
//  ZDPluginCollection.m
//  ZDStudent
//
//  Created by zhangdong on 16/8/15.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "FormatPlugin.h"
//#include "PluginCollection.h"
#import <Foundation/Foundation.h>


#import "ZDPluginCollection.h"

@interface ZDPluginCollection () {
  //  ZDAccountServer* _accountServer;
    FormatPlugin*l;
}

@end
@implementation ZDPluginCollection

static ZDPluginCollection * _Insance = nil;
static std::vector<shared_ptr<FormatPlugin> >  plugins;
+ (ZDPluginCollection*) sharedInstance
{
    if(_Insance == nil) {
        _Insance = [[ZDPluginCollection alloc] init];
        PluginCollection p = PluginCollection::Instance();
       plugins =  p.plugins();
        
    }
    return _Insance;
}

@end


