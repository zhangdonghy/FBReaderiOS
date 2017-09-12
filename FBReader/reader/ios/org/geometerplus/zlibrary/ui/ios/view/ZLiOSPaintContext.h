//
//  ZLiOSPaintContext.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/31.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLiOSPaintContext_hpp
#define ZLiOSPaintContext_hpp

#include <stdio.h>
#include "ZLPaintContext.hpp"

class ZLiOSPaintContext : public ZLPaintContext {
private:
    shared_ptr<ZLColor> myBackgroundColor;
    shared_ptr<ZLColor> myTextColor;
    shared_ptr<ZLColor> myLineColor;
    shared_ptr<ZLColor> myFillColor;
    int myFillAlapha;
    int mLineWidth;
    int myWidth;
    int myHeight;
    std::string myFontName;
    int myFontSize;
    
    // font style
    bool myBold;
    bool myItalic;
    bool myUnderline;
    bool myStrikeThrough;

protected:
    virtual int getSpaceWidthInternal();
    virtual int getStringHeightInternal();
    virtual int getCharHeightInternal(char chr);
    virtual int getDescentInternal();
    
public:
    ZLiOSPaintContext();

    void initContext(int width, int height);
    virtual int getWidth();
    virtual int getHeight();

    virtual void setTextColor(shared_ptr<ZLColor> color);
    virtual void setLineColor(shared_ptr<ZLColor> color);
    virtual void setLineWidth(int width);
    virtual void setFillColor(shared_ptr<ZLColor> color, int alpha);
    virtual void setFontInternal(std::string fontName, int size, bool bold, bool italic, bool underline, bool strikeThrough);
    
    virtual void clear(shared_ptr<ZLColor> color);

    // string
    virtual void drawString(int x, int y, const char* string, int offset, int length);
    virtual int getStringWidth(const char* string2, int offset, int length);
    virtual void drawString(int x, int y, const short* string, int offset, int length);
    virtual int getStringWidth(const short* string2, int offset, int length);

    // image
    UIImage* getLatestUIImage();
    virtual void drawImage(int x, int y, const ZLImageData& image,
                           shared_ptr<ZLPaintContext::Size> maxSize,
                           ZLPaintContext_ScalingType scaling,
                           ColorAdjustingMode adjustingMode);
    virtual shared_ptr<ZLPaintContext::Size> imageSize(const ZLImageData& image,
                                                       shared_ptr<ZLPaintContext::Size> maxSize,
                                                       ZLPaintContext_ScalingType scaling);

    // rectangle
    virtual void drawLine(int x0, int y0, int x1, int y1);
    virtual void fillRectangle(int x0, int y0, int x1, int y1);

    // polygon
    virtual void drawPolygonalLine(int* xs, int* ys,int length);
    virtual void fillPolygon(int* xs, int* ys, long length, shared_ptr<ZLColor> color);
    virtual void drawOutline(int* xs, int* ys, long length);
    
    // circle
    virtual void fillCircle(int x, int y, int radius);

    // underline
    virtual void drawUnderline(std::vector<shared_ptr<ZLRect>> rectangles, shared_ptr<ZLColor> color, int noteCount);

    // opinion
    virtual void drawOpinion(int x, int y, int height, int pIndex, int opinionCount);
};

#endif /* ZLiOSPaintContext_hpp */

