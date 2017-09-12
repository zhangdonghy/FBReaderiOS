//
//  ZLiOSPaintContext.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/31.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#import <UIKit/UIKit.h>
#include "ZLiOSPaintContext.h"

#include "ZLUnicodeUtil.h"
#include "ZLiOSImageData.h"


#define PI 3.14159265358979323846


ZLiOSPaintContext::ZLiOSPaintContext() : ZLPaintContext(0) {
    myBackgroundColor = new ZLColor(0,0,0);
    myTextColor = new ZLColor(255.0,0,0);
    myLineColor = new ZLColor(255.0,0,0);
    myFillColor = new ZLColor(255.0,0,0);
    myFillAlapha = 255;
    myFontName = "";
    myFontSize = 18;
    mLineWidth = 2;
    myBold = false;
    myItalic = false;
    myUnderline = false;
    myStrikeThrough = false;
}

#pragma mark - protected begin
int ZLiOSPaintContext::getSpaceWidthInternal() {
    NSString* aString = @" ";
    UIFont* nameFont = [UIFont fontWithName:@"Helvetica" size:myFontSize];
    
    NSMutableParagraphStyle* paragraphStyle = [[NSMutableParagraphStyle alloc] init];
    paragraphStyle.lineBreakMode = NSLineBreakByCharWrapping;
    CGSize size = [aString boundingRectWithSize:CGSizeMake(237, 200)
                                        options:NSStringDrawingUsesLineFragmentOrigin
                                     attributes:@{NSFontAttributeName: nameFont, NSParagraphStyleAttributeName: paragraphStyle}
                                        context:nil].size;
    return size.width;
}

int ZLiOSPaintContext::getStringHeightInternal() {
//    NSString* value =[NSString stringWithFormat:@"此处的换行类型"];
//    CGSize sizeToFit = [value sizeWithFont:[UIFont systemFontOfSize:myFontSize] constrainedToSize:CGSizeMake(300, CGFLOAT_MAX) lineBreakMode:UILineBreakModeWordWrap];//此处的换行类型（lineBreakMode）可根据自己的实际情况进行设置
//    return sizeToFit.height;
    UIFont* fnt = [UIFont fontWithName:@"Helvetica" size:myFontSize];
    return fnt.lineHeight;
}

int ZLiOSPaintContext::getCharHeightInternal(char chr) {
//    UIFont* fnt = [UIFont fontWithName:@"Helvetica" size:myFontSize];
    UIFont* fnt = [UIFont systemFontOfSize:myFontSize];
    NSString* str = [NSString stringWithFormat:@"%c", chr];
    CGSize textSize = [str boundingRectWithSize:CGSizeMake(100, 100)
                                        options:NSStringDrawingTruncatesLastVisibleLine
                                     attributes:@{NSFontAttributeName: fnt}
                                        context:nil].size;
    return textSize.height;
}

int ZLiOSPaintContext::getDescentInternal() {
    UIFont* fnt = [UIFont fontWithName:@"Helvetica" size:myFontSize];
    return fnt.descender;
}
#pragma mark protected end -

#pragma mark - public begin
void ZLiOSPaintContext::initContext(int width, int height) {
    CGSize size = {static_cast<CGFloat>(width), static_cast<CGFloat>(height)};
    myWidth = width;
    myHeight = height;
    UIGraphicsBeginImageContextWithOptions(size, NO, 0);
}

int ZLiOSPaintContext::getWidth() {
    return myWidth;
}

int ZLiOSPaintContext::getHeight() {
    return myHeight;
}

void ZLiOSPaintContext::setTextColor(shared_ptr<ZLColor> color) {
    myTextColor->Red = color->Red;
    myTextColor->Green = color->Green;
    myTextColor->Blue = color->Blue;
}

void ZLiOSPaintContext::setLineColor(shared_ptr<ZLColor> color) {
    myLineColor->Red = color->Red;
    myLineColor->Green = color->Green;
    myLineColor->Blue = color->Blue;
}

void ZLiOSPaintContext::setLineWidth(int width) {
    mLineWidth = width;
}

void ZLiOSPaintContext::setFillColor(shared_ptr<ZLColor> color, int alpha) {
    myFillColor->Red = color->Red;
    myFillColor->Green = color->Green;
    myFillColor->Blue = color->Blue;
    myFillAlapha = alpha;
}

void ZLiOSPaintContext::setFontInternal(std::string fontName, int size,
                                        bool bold, bool italic, bool underline, bool strikeThrough) {
    myFontName = fontName;
    if (size != 18) {
//        NSLog(@" wrong size: %d",size);
    }
    if (bold) {
        myBold = bold;
    } else {
        myBold = bold;
    }
    
    myItalic = italic;
    myUnderline = underline;
    myStrikeThrough = strikeThrough;
    myFontSize = size;
}

void ZLiOSPaintContext::clear(shared_ptr<ZLColor> color) {
    myBackgroundColor = color;

    CGContextRef context = UIGraphicsGetCurrentContext();
//    CGContextAddEllipseInRect(context, CGRectMake(0,0,100,100));
    
    // CGContextBound
    CGContextAddRect(context, CGRectMake(0, 0, myWidth, myHeight));
    UIColor* color2 = [UIColor colorWithRed:color->Red/255.0f green:color->Green/255.0f blue:color->Blue/255.0f alpha:1];
    // 填充颜色为蓝色
    CGContextSetFillColorWithColor(context, color2.CGColor);
    // 在context上绘制
    CGContextFillPath(context);
    // 把当前context的内容输出成一个UIImage图片
//    UIImage* i = UIGraphicsGetImageFromCurrentImageContext();
}

#pragma mark - string
void ZLiOSPaintContext::drawString(int x, int y, const char* string, int offset, int length) {
    NSMutableDictionary* attrs = [NSMutableDictionary dictionary];
    attrs[NSForegroundColorAttributeName] = [UIColor redColor];
    if (myFontName.length() > 0) {
        attrs[NSFontAttributeName] = [UIFont fontWithName:[NSString stringWithUTF8String:myFontName.c_str()] size:myFontSize];
    } else {
        attrs[NSFontAttributeName] = [UIFont systemFontOfSize:myFontSize];
    }
    
    NSString* aString = [NSString stringWithCString:string encoding:NSUTF8StringEncoding];
    [aString drawAtPoint:CGPointMake(x, y) withAttributes:attrs];
}

int ZLiOSPaintContext::getStringWidth(const char* string2, int offset, int length) {
    UIFont* nameFont = [UIFont fontWithName:@"Helvetica" size:myFontSize];
    NSString* aString = [NSString stringWithUTF8String:string2];
    NSMutableParagraphStyle* pStyle = [[NSMutableParagraphStyle alloc] init];
    pStyle.lineBreakMode = NSLineBreakByCharWrapping;
    CGSize size = [aString boundingRectWithSize:CGSizeMake(237, 200)
                                        options:NSStringDrawingUsesLineFragmentOrigin
                                     attributes:@{NSParagraphStyleAttributeName: pStyle, NSFontAttributeName: nameFont}
                                        context:nil].size;
    return size.width;
}

void ZLiOSPaintContext::drawString(int x, int y, const short* string, int offset, int length) {
    NSMutableDictionary* attrs = [NSMutableDictionary dictionary];
    CGFloat red = myTextColor->Red;
    CGFloat green = myTextColor->Green;
    CGFloat blue = myTextColor->Blue;
    attrs[NSForegroundColorAttributeName] = [UIColor colorWithRed:red/255.0f green:green/255.0f blue:blue/255.0f alpha:1.0f];
    
    if (myFontName.length() > 0) {
        attrs[NSFontAttributeName] = [UIFont fontWithName:[NSString stringWithUTF8String:myFontName.c_str()] size:myFontSize];
    } else {
        if (myBold) {
            attrs[NSFontAttributeName] = [UIFont boldSystemFontOfSize:myFontSize];
        } else {
            attrs[NSFontAttributeName] = [UIFont systemFontOfSize:myFontSize];
        }
    }
    
    if (myItalic) {
        attrs[NSObliquenessAttributeName] = @"0.5";
    }
    if (myUnderline) {
        attrs[NSUnderlineStyleAttributeName] = @(NSUnderlineStyleSingle);
    }
    if (myStrikeThrough) {
        attrs[NSStrikethroughStyleAttributeName] = @(NSUnderlineStyleSingle);
    }

    std::string to = "";
    ZLUnicodeUtil::Ucs2String from;
    for (int i = 0; i < length; i++) {
        from.insert(from.end(), string[i + offset]);
    }
    ZLUnicodeUtil::ucs2ToUtf8(to, from);
    
    NSString* aString = [NSString stringWithUTF8String:to.c_str()];
    NSString* str = @"段";
    if ([aString rangeOfString:str].location != NSNotFound) {
        [aString drawAtPoint:CGPointMake(x, y - getStringHeight()) withAttributes:attrs];
    } else {
        [aString drawAtPoint:CGPointMake(x, y - getStringHeight()) withAttributes:attrs];
    }
}

int ZLiOSPaintContext::getStringWidth(const short* string2, int offset, int length) {
    NSMutableDictionary* attrs = [NSMutableDictionary dictionary];
    
    if (myFontName.length() > 0) {
        attrs[NSFontAttributeName] = [UIFont fontWithName:[NSString stringWithUTF8String:myFontName.c_str()] size:myFontSize];
    } else {
        if (myBold) {
            attrs[NSFontAttributeName] = [UIFont boldSystemFontOfSize:myFontSize];
        } else {
            attrs[NSFontAttributeName] = [UIFont systemFontOfSize:myFontSize];
        }
    }
    
    if (myItalic) {
        attrs[NSObliquenessAttributeName] = @"0.5";
    }
    if (myUnderline) {
        attrs[NSUnderlineStyleAttributeName] = @(NSUnderlineStyleSingle);
    }
    if (myStrikeThrough) {
        attrs[NSStrikethroughStyleAttributeName] = @(NSUnderlineStyleSingle);
    }

    std::string to = "";
    ZLUnicodeUtil::Ucs2String from;
    for (int i = 0; i < length; i++) {
        from.insert(from.end(), string2[i + offset]);
    }
    ZLUnicodeUtil::ucs2ToUtf8(to, from);
    
    NSString* aString = [NSString stringWithUTF8String:to.c_str()];
    CGSize size2 = [aString sizeWithAttributes:attrs];
    return size2.width;
}

#pragma mark - image
UIImage* ZLiOSPaintContext::getLatestUIImage() {
    UIImage* res = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return res;
}

void ZLiOSPaintContext::drawImage(int x, int y, const ZLImageData& image,
                                  shared_ptr<ZLPaintContext::Size> maxSize,
                                  ZLPaintContext_ScalingType scaling,
                                  ColorAdjustingMode adjustingMode) {
    ZLiOSImageData data = (const ZLiOSImageData&)image;
    UIImage* image2 = data.getBitmap(maxSize->Width, maxSize->Height, scaling);
    if (image2 != nil) {
        [image2 drawInRect:CGRectMake(x,y-image2.size.height, image2.size.width, image2.size.height)];
    }
}

shared_ptr<ZLPaintContext::Size> ZLiOSPaintContext::imageSize(const ZLImageData& image,
                                                              shared_ptr<ZLPaintContext::Size> maxSize,
                                                              ZLPaintContext_ScalingType scaling) {
    ZLiOSImageData data = (const ZLiOSImageData&)image;
    UIImage* image2 = data.getBitmap(maxSize->Width, maxSize->Height, scaling);
    if (image2 != 0) {
        return new ZLPaintContext::Size(image2.size.width, image2.size.height);
    }
    return 0;
}

#pragma mark - rectangle
void ZLiOSPaintContext::drawLine(int x0, int y0, int x1, int y1) {
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextSetLineCap(context, kCGLineCapSquare);
    CGContextSetLineWidth(context, mLineWidth);

    CGContextSetRGBStrokeColor(context, myLineColor->Red/255.0f, myLineColor->Green/255.0f, myLineColor->Blue/255.0f, 1.0f);

    CGContextBeginPath(context);
    CGContextMoveToPoint(context, x0, y0);
    CGContextAddLineToPoint(context, x1, y1);
    CGContextStrokePath(context);
}

void ZLiOSPaintContext::fillRectangle(int x0, int y0, int x1, int y1) {
     CGContextRef context = UIGraphicsGetCurrentContext();
    
    UIColor* color = [UIColor colorWithRed:myFillColor->Red / 255.0f
                                     green:myFillColor->Green / 255.0f
                                      blue:myFillColor->Blue / 255.0f
                                     alpha:1.0f];
    CGContextSetFillColorWithColor(context,color.CGColor);

    int x = x0;
    int width = 0;
    int height = 0;
    if (x > x1) {
        x = x1;
    }
    width = x1 - x0;
    if (width < 0) {
        width = -width;
    }
    int y = y0;
    if (y > y1) {
        y = y1;
    }
    height = y1 - y0;
    if (height < 0) {
        height = -height;
    }
    if (width > 0 && height > 0) {
//        printf("x:%d, y:%d, width%d,height%d\n",x,y,width,height);
        CGContextFillRect(context, CGRectMake(x, y, width, height));
    }
}

#pragma mark - polygon
void ZLiOSPaintContext::drawPolygonalLine(int* xs, int* ys, int length) {
//    NSLog(@"drawPolygonalLine");
    CGContextRef context = UIGraphicsGetCurrentContext();

    CGContextSetRGBStrokeColor(context, myLineColor->Red/255.0f, myLineColor->Green/255.0f, myLineColor->Blue/255.0f, 1.0f);
    
    CGContextMoveToPoint(context, xs[length-1], ys[length-1]);
    for (int i = 0; i < length; i++) {
        CGContextAddLineToPoint(context, xs[i], ys[i]);
//        NSLog(@"X:%d, Y%d",xs[i],ys[i]);
    }
//    NSLog(@"<<<<<<<<<<<<<<<<<<<");
    CGContextClosePath(context);
    CGContextStrokePath(context);
}

void ZLiOSPaintContext::fillPolygon(int* xs, int* ys, long length, shared_ptr<ZLColor> color) {
//    NSLog(@"fillPolygon");
//    NSLog(@">>>>>>>>>>>>>>>>,legnth:%d",length);
    CGContextRef context = UIGraphicsGetCurrentContext();

    UIColor* bgColor = [UIColor colorWithRed:color->Red / 255.0f
                                       green:color->Green / 255.0f
                                        blue:color->Blue / 255.0f
                                       alpha:myFillAlapha / 255.0f];
    CGContextSetFillColorWithColor(context, bgColor.CGColor);

    long last = length - 1;
    CGContextMoveToPoint(context, xs[last], ys[last]);
    for (int i = 0; i < length; i++) {
        CGContextAddLineToPoint(context, xs[i], ys[i]);
//        NSLog(@"X:%d, Y%d",xs[i],ys[i]);
    }
//    NSLog(@"<<<<<<<<<<<<<<<<<<<");
    CGContextClosePath(context);
    CGContextFillPath(context);
}

void ZLiOSPaintContext::drawOutline(int* xs, int* ys, long length) {
//    NSLog(@"drawOutline");
//    NSLog(@">>>>>>>>>>>>>>>>,legnth:%d",length);
    long last = length - 1;
    int xStart = (xs[0] + xs[last]) / 2;
    int yStart = (ys[0] + ys[last]) / 2;
    int xEnd = xStart;
    int yEnd = yStart;
    if (xs[0] != xs[last]) {
        if (xs[0] > xs[last]) {
            xStart -= 5;
            xEnd += 5;
        } else {
            xStart += 5;
            xEnd -= 5;
        }
    } else {
        if (ys[0] > ys[last]) {
            yStart -= 5;
            yEnd += 5;
        } else {
            yStart += 5;
            yEnd -= 5;
        }
    }

    CGContextRef context = UIGraphicsGetCurrentContext();

    UIColor* color = [UIColor colorWithRed:myLineColor->Red / 255.0f
                                     green:myLineColor->Green / 255.0f
                                      blue:myLineColor->Blue / 255.0f
                                     alpha:1.0f];
    CGContextSetStrokeColorWithColor(context, color.CGColor);
    
    CGContextMoveToPoint(context, xStart, yStart);
//    NSLog(@"xStart:%d, yStart:%d", xStart, yStart);
    for (int i = 0; i < length; i++) {
        CGContextAddLineToPoint(context, xs[i], ys[i]);
//        NSLog(@"X:%d, Y:%d", xs[i], ys[i]);
    }
//    NSLog(@"<<<<<<<<<<<<<<<<<<<");
    CGContextClosePath(context);
    CGContextStrokePath(context);
}

#pragma mark - circle
void ZLiOSPaintContext::fillCircle(int x, int y, int radius) {
    CGContextRef context = UIGraphicsGetCurrentContext();

    UIColor* color = [UIColor colorWithRed:myFillColor->Red / 255.0f
                                     green:myFillColor->Green / 255.0f
                                      blue:myFillColor->Blue / 255.0f
                                     alpha:myFillAlapha / 255.0f];
    CGContextSetFillColorWithColor(context, color.CGColor);

    CGContextAddArc(context, x, y, radius, 0, 2 * PI, 0);
    CGContextFillPath(context);
}

#pragma mark - underline
void ZLiOSPaintContext::drawUnderline(std::vector<shared_ptr<ZLRect>> rectangles, shared_ptr<ZLColor> color, int noteCount) {
    // 获取每行的高度，用来画线
//    NSLog(@"drawUnderLine");
//    NSLog(@">>>>>>>>>>>>>>>>");
    int minAvailableH = floorf(getStringHeight() * 1);
    int min = 10000;
    for (std::vector<shared_ptr<ZLRect>>::const_iterator it = rectangles.begin(); it != rectangles.end(); it++) {
        shared_ptr<ZLRect> current = *it;
//        NSLog(@"Rect left:%d, right:%d, top:%d, bottom:%d, minAvailableH:%d, descent:%d",
//              current->Left, current->Right, current->Top, current->Bottom, minAvailableH, getDescent());
        int height = current->Bottom - current->Top;
        if (height > 1) {
            if (min > height && height > minAvailableH) {
                min = height;
            }
        }
    }
//    NSLog(@"min:%d", min);
    if (min >= 10000) {
        return;
    }
    
    CGContextRef context = UIGraphicsGetCurrentContext();

    UIColor* lineColor = [UIColor colorWithRed:color->Red/255.0f green:color->Green/255.0f blue:color->Blue/255.0f alpha:1.0f];
    CGContextSetStrokeColorWithColor(context, lineColor.CGColor);

    CGPoint tailPos = CGPointZero;
    for (std::vector<shared_ptr<ZLRect>>::const_iterator it = rectangles.begin(); it != rectangles.end(); it++) {
        shared_ptr<ZLRect> current = *it;
        
        int xStart = current->Left;
        int yStart;
        int xEnd = current->Right;
        int yEnd;
        int realHeight = current->Bottom - current->Top;
        int count = realHeight / min;
        float gap = realHeight / (float)count;
//        NSLog(@"real height: %d, count: %d, gap: %f", realHeight, count, gap);
        for (int i = 0; i < count; i++) {
            yEnd = yStart = current->Bottom - gap * i + 2;
            if (current->Bottom > tailPos.y) {
                tailPos = CGPointMake(current->Right, current->Bottom + 2);
            }

            CGContextMoveToPoint(context, xStart, yStart);
//            NSLog(@"xStart:%d, yStart:%d", xStart, yStart);
            CGContextAddLineToPoint(context, xEnd, yEnd);
//            NSLog(@"xEnd:%d, yEnd:%d", xEnd, yEnd);
            CGContextClosePath(context);
            CGContextStrokePath(context);
        }
    }

//    NSLog(@"tail pos: %@", NSStringFromCGPoint(tailPos));
    
    if (noteCount > 0) {
        NSString* text = @"我的笔记";
        float scale = 0.5;
        UIFont* font = [UIFont systemFontOfSize:myFontSize * scale];
        CGSize textSize = [text sizeWithAttributes:@{NSFontAttributeName: font}];
        
        UIImage* bg = [UIImage imageNamed:@"icon_pl_123_edited.png"];
        CGRect bgRect = CGRectMake(tailPos.x - textSize.width - 4, tailPos.y, textSize.width + 4, textSize.height);
        bg = [bg resizableImageWithCapInsets:UIEdgeInsetsMake(bg.size.height / 2 - 1, bg.size.width / 2 - 1,
                                                              bg.size.height / 2, bg.size.width / 2)];
        [bg drawInRect:bgRect];
        
        CGRect textRect = CGRectMake(tailPos.x - textSize.width - 2, tailPos.y, textSize.width, textSize.height);
        [text drawInRect:textRect
          withAttributes:@{NSForegroundColorAttributeName: [UIColor whiteColor], NSFontAttributeName: font}];
    }
//    NSLog(@"<<<<<<<<<<<<<<<<<<<");
}

void ZLiOSPaintContext::drawOpinion(int x, int y, int height, int pIndex, int opinionCount) {
    NSString* text = opinionCount > 999 ? @"999+" : [NSString stringWithFormat:@"%d", opinionCount];

    int stringHeight = getStringHeight();
    float centerY = y - stringHeight / 2.0;
    UIFont* font = [UIFont systemFontOfSize:myFontSize * 0.5];
    CGSize textSize = [text sizeWithAttributes:@{NSFontAttributeName: font}];
    float bgWidth = textSize.width + 2 > stringHeight ? textSize.width + 2 : stringHeight;
    float bgTop = centerY - bgWidth / 2;

    UIImage* bg = [UIImage imageNamed:@"kf_bg.png"];
    CGRect bgRect = CGRectMake(x, bgTop, bgWidth, bgWidth);
    [bg drawInRect:bgRect];

    float leftOffset = (bgWidth - textSize.width) / 2;
    float topOffset = (bgWidth - textSize.height) * 0.4;
    [text drawInRect:CGRectMake(x + leftOffset, bgTop + topOffset, textSize.width, textSize.height)
      withAttributes:@{NSForegroundColorAttributeName: [UIColor whiteColor], NSFontAttributeName: font}];
}
#pragma mark public end -
