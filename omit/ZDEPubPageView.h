//
//  ZDEPubPageView.h
//  ZDLibrary
//
//  Created by 蒋晨霄 on 2017/2/10.
//  Copyright © 2017年 蒋晨霄. All rights reserved.
//

#import "ZDBaseView.h"

@protocol ZDEPubPageDelegate <NSObject>

- (UIImage *)bitmapWithViewSize:(CGSize)viewSize;
- (BOOL)isPageStateCurrent;
- (BOOL)isCtrlBarVisible;
- (BOOL)isPanelVisible;
- (BOOL)isListening;
- (int)tapParagraphAtPos:(CGPoint)pos;
- (BOOL)respondSingleTapAtPos:(CGPoint)pos;
- (BOOL)respondLongPressAtPos:(CGPoint)pos fromView:(UIView *)pageView;

@optional
- (void)pageViewDealloc;
- (void)tryStartListeningFromParagraph:(int)pIndex;
- (void)hideCtrlBars;
- (void)hidePanel;
- (void)tryChangePrevPage;
- (void)tryChangeNextPage;
- (void)showCtrlBars;
- (void)respondFingerReleasedAfterLongPress:(CGPoint)pos;
- (void)respondTouchMoved:(CGPoint)pos fromView:(UIView *)pageView;
- (void)respondPressAtPos:(CGPoint)pos;
- (void)respondFingerReleased:(CGPoint)pos;

@end

@interface ZDEPubPageView : ZDBaseView

@property (weak) id<ZDEPubPageDelegate> delegate;

@end
