//
//  ZDEPubPageView.m
//  ZDLibrary
//
//  Created by 蒋晨霄 on 2017/2/10.
//  Copyright © 2017年 蒋晨霄. All rights reserved.
//

#import "ZDEPubPageView.h"

#import "ZDEPubConfig.h"

@interface ZDEPubPageView ()

// touch
@property (assign) CGPoint startPos;
@property (assign) CGPoint endPos;
@property (assign) BOOL isMoved;
@property (strong) CADisplayLink* displayLink;
@property (assign) float pressTime;
@property (assign) BOOL longPressTriggered;
@property (assign) BOOL isPressedOnCursor;
@property (assign) float centerArea;

@property (assign) BOOL canRespondTapInListening;

@end

@implementation ZDEPubPageView

- (void)dealloc {
    [self stopLongPressTiming];

    if ([_delegate respondsToSelector:@selector(pageViewDealloc)]) {
        [_delegate pageViewDealloc];
    }
}

- (void)initData {
    self.userInteractionEnabled = true;
    self.backgroundColor = [UIColor whiteColor];
    
    // touch
    _startPos = CGPointZero;
    _endPos = CGPointZero;
    _isMoved = NO;
    _pressTime = 0;
    _displayLink = nil;
    _longPressTriggered = NO;
    _isPressedOnCursor = NO;
    _centerArea = 0;
    
    _canRespondTapInListening = NO;
}

- (BOOL)canBecomeFirstResponder {
    return YES;
}

// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code

    // draw image
//    NSLog(@"draw rect of page state: %d", _pageState);
    UIImage* image = [_delegate bitmapWithViewSize:self.bounds.size];
    if (image != nil) {
        int imagewidth = self.bounds.size.width;
        int imageheight = self.bounds.size.height;
        [image drawInRect:CGRectMake(0, 0, imagewidth , imageheight)];
    }
}

#pragma mark - listening
- (void)respondTapInListening {
    int pIndex = [_delegate tapParagraphAtPos:_endPos];
    if (pIndex >= 0) {
        if ([_delegate respondsToSelector:@selector(tryStartListeningFromParagraph:)]) {
            [_delegate tryStartListeningFromParagraph:pIndex];
        }
    }
}

#pragma mark - tap
- (void)respondTap {
    BOOL isClickSwallowed = [_delegate respondSingleTapAtPos:_endPos];
    if (isClickSwallowed) {
        // 点击在划线/看法上，touch事件不响应
        NSLog(@"click on notes/opinions");
        return;
    }

//    int selectedTextCount = _manager.selectedTextCount;
//    if (selectedTextCount > 0) {
//        // 有选中的文字
//        BOOL isTapOnRegion = [_manager isTapOnSelectedRegion:_endPos];
//        NSLog(@"is tapped on selected region: %d", isTapOnRegion);
//        if (isTapOnRegion) {
//            [_manager showPanel];
//            
//        } else {
//            [_manager clearSelectedParagraph];
//        }
//        
//    } else {
        // 没有选中的文字
        if ([_delegate isCtrlBarVisible]) {
            // 控制栏处于显示状态，点击隐藏
            if ([_delegate respondsToSelector:@selector(hideCtrlBars)]) {
                [_delegate hideCtrlBars];
            }
            return;
        }
        if ([_delegate isPanelVisible]) {
            // panel处于显示状态，点击隐藏
            if ([_delegate respondsToSelector:@selector(hidePanel)]) {
                [_delegate hidePanel];
            }
            return;
        }
        
        // 控制栏未显示，点击判断翻页/显示控制栏
        float viewWidth = self.bounds.size.width;
        float pageChangeArea = (viewWidth - _centerArea) / 2;
        float leftPageLMargin = 0;
        float leftPageRMargin = pageChangeArea;
        float rightPageLMargin = viewWidth - pageChangeArea;
        float rightPageRMargin = viewWidth;
        float posX = _endPos.x;
        
        if (leftPageLMargin <= posX && posX <= leftPageRMargin) {
            // 往前翻页
            if ([_delegate respondsToSelector:@selector(tryChangePrevPage)]) {
                [_delegate tryChangePrevPage];
            }
        } else if (rightPageLMargin <= posX && posX <= rightPageRMargin) {
            // 往后翻页
            if ([_delegate respondsToSelector:@selector(tryChangeNextPage)]) {
                [_delegate tryChangeNextPage];
            }
        } else {
            // 显示控制栏
            if ([_delegate respondsToSelector:@selector(showCtrlBars)]) {
                [_delegate showCtrlBars];
            }
        }
//    }
}

#pragma mark - long press
- (void)startLongPressTiming {
    if (_displayLink) {
        return;
    }
    
    _pressTime = 0;
    
    _displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(listenLongPressTiming:)];
    [_displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
}

- (void)stopLongPressTiming {
    if (_displayLink) {
        [_displayLink invalidate];
        _displayLink = nil;
    }
}

- (void)listenLongPressTiming:(CADisplayLink*)displayLink {
    _pressTime += displayLink.duration;
    
    if (_pressTime > LONG_PRESS_TRIGGER_TIME) {
//        NSLog(@"press time passed trigger: %f", _pressTime);
        [self stopLongPressTiming];
        
        // 开始长按
        [self respondLongPress];
    }
}

- (void)respondLongPress {
//    NSLog(@"respond long press: %p, pageState: %d", self, _pageState);
    if (![_delegate isPageStateCurrent]) {
        // 不是当前页的pageView不响应长按
        return;
    }

    _longPressTriggered = [_delegate respondLongPressAtPos:_startPos fromView:self];

    // 是否点击到光标
//    _isPressedOnCursor = [_model isPressedOnCursor:_startPos];
}

#pragma mark - gesture
- (BOOL)gestureRecognizerShouldBegin:(UIGestureRecognizer *)gestureRecognizer {
//    NSLog(@"gestureRecognizerShouldBegin: %@", [gestureRecognizer class]);
//    NSLog(@"longPressTriggered: %d, isPressedOnCursor: %d", _longPressTriggered, _isPressedOnCursor);
    // 不响应单击
    if ([gestureRecognizer isKindOfClass:[UITapGestureRecognizer class]]) {
        return NO;
    }
    if (_longPressTriggered) {
        return NO;
    }
    if (_isPressedOnCursor) {
        return NO;
    }
    
    // 控制栏处于显示状态时，手势不响应
    if ([_delegate isCtrlBarVisible]) {
        return NO;
    }
    
    // Panel处于显示状态时，手势不响应
    if ([_delegate isPanelVisible]) {
        return NO;
    }
    
    return YES;
}

#pragma mark - touch
- (void)prepareToListenTouches {
    _startPos = CGPointZero;
    _endPos = CGPointZero;
    _isMoved = NO;
    _pressTime = 0;
    _longPressTriggered = NO;
    [self stopLongPressTiming];
    _isPressedOnCursor = NO;
    
    if (_centerArea <= 0) {
        _centerArea = self.bounds.size.width * PAGE_CENTER_PERCENT;
    }
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    [self prepareToListenTouches];
    
    _startPos = [[touches anyObject] locationInView:self];
//    NSLog(@"touchesBegan: %@", NSStringFromCGPoint(_startPos));

    if ([_delegate isListening]) {
        // 听书模式中，只响应单击
        _canRespondTapInListening = YES;

    } else {
//        // 一按下，立即隐藏panel
//        [_manager hidePanel];
    
        [self startLongPressTiming];
    }
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
    CGPoint curPos = [[touches anyObject] locationInView:self];
//    NSLog(@"touchesMoved: %@", NSStringFromCGPoint(curPos));
    float distance = distanceBetweenPoints(curPos, _startPos);
//    NSLog(@"distance: %f", distance);
    if (distance < MOVE_START_DISTANCE) {
        // 移动距离太小，不响应移动
        return;
    }
    
    if ([_delegate isListening]) {
        // 朗读模式中移动，取消对单击的响应
        _canRespondTapInListening = NO;
        return;
    }
    
    [self stopLongPressTiming];
    
    if (_longPressTriggered) {
        // 处于长按下
        if (!CGRectContainsPoint(self.bounds, curPos)) {
            // 坐标已超出文本，不响应
            return;
        }

        // 正常情况下长按状态下必然有文字被选中，且按在光标上
        _isMoved = YES;

        if ([_delegate respondsToSelector:@selector(respondTouchMoved:fromView:)]) {
            [_delegate respondTouchMoved:curPos fromView:self];
        }
        
    } else {
        // 未达到长按条件
        if (!_isMoved) {
            if ([_delegate respondsToSelector:@selector(respondPressAtPos:)]) {
                [_delegate respondPressAtPos:curPos];
            }

            // 是否点击到光标
//            _isPressedOnCursor = [_manager isPressedOnCursor:_startPos];
            _isMoved = YES;
            return;
        }
        
        if ([_delegate respondsToSelector:@selector(respondTouchMoved:fromView:)]) {
            [_delegate respondTouchMoved:curPos fromView:self];
        }
    }
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
    _endPos = [[touches anyObject] locationInView:self];
//    NSLog(@"touchesEnded: %@", NSStringFromCGPoint(_endPos));
    
    if ([_delegate isListening]) {
        if (_canRespondTapInListening) {
            [self respondTapInListening];
        }
        return;
    }
    
    [self stopLongPressTiming];
    
    if (_longPressTriggered) {
        if ([_delegate respondsToSelector:@selector(respondFingerReleasedAfterLongPress:)]) {
            [_delegate respondFingerReleasedAfterLongPress:_endPos];
        }
        
    } else {
        if (_isMoved) {
            if ([_delegate respondsToSelector:@selector(respondFingerReleased:)]) {
                [_delegate respondFingerReleased:_endPos];
            }
            
        } else {
            // 未移动过，响应单击
            [self respondTap];
        }
    }
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
//    CGPoint touchPos = [[touches anyObject] locationInView:self];
//    NSLog(@"touchesCancelled: %@", NSStringFromCGPoint(touchPos));

    [self stopLongPressTiming];
}

@end
