//
//  ZLPaintContext.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/31.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLPaintContext_hpp
#define ZLPaintContext_hpp

#include <string>
#include <list>
#include <map>
#include "shared_ptr.h"

#include "SystemInfo.hpp"
#include "ZLColor.hpp"
#include "ZLSize.hpp"
#include "ZLImageData.hpp"

#include <vector>
#include "ZLRect.hpp"


enum ZLPaintContext_ScalingType {
    OriginalSize,
    IntegerCoefficient,
    FitMaximum
};

enum ZLPaintContext_FillMode {
    tile,
    tileMirror,
    fullscreen,
    stretch,
    tileVertically,
    tileHorizontally
};

class ZLPaintContext {

public:
    class Size {
    public:
        int Width;
        int Height;
        
        Size(int w, int h) {
            Width = w;
            Height = h;
        }
        
        virtual  bool equals(Size s) {
            return Width == s.Width && Height == s.Height;
        }
        
        virtual std::string toString() {
            std::string res = "ZLPaintContext.Size[";
            res += Width;
            res += "x";
            res += Height;
            res += "]";
            return res;
        }
        
        void log() {
            printf("size memory: %p\n", this);
        }
    };


private:
    shared_ptr<SystemInfo> mySystemInfo;
//    std::list<std::string> myFamilies = new std::list<std::string>();
    bool myResetFont = true;
//    std::list<FontEntry> myFontEntries;
    std::string myFontName;
    int myFontSize;
    bool myFontIsBold;
    bool myFontIsItalic;
    bool myFontIsUnderlined;
    bool myFontIsStrikedThrough;
    int mySpaceWidth = -1;
    int myStringHeight = -1;
    int myDescent = -1;
    std::map<char,int> myCharHeights;// = new TreeMap<Character,Integer>(); zhangdong , int may change to

protected:
    ZLPaintContext(shared_ptr<SystemInfo> systemInfo);
    shared_ptr<SystemInfo> getSystemInfo();
    
    virtual int getSpaceWidthInternal() = 0;
    virtual int getStringHeightInternal() = 0;
    virtual int getCharHeightInternal(char chr) = 0;
    virtual int getDescentInternal() = 0;

public:
    enum ColorAdjustingMode {
        NONE,
        DARKEN_TO_BACKGROUND,
        LIGHTEN_TO_BACKGROUND
    };

    virtual int getWidth() = 0;
    virtual int getHeight() = 0;

    ZLColor getBackgroundColor();
    virtual void setTextColor(shared_ptr<ZLColor> color) = 0;
    virtual void setLineColor(shared_ptr<ZLColor> color) = 0;
    virtual void setLineWidth(int width) = 0;
    virtual void setFillColor(shared_ptr<ZLColor> color, int alpha)=0;
    void setFillColor(shared_ptr<ZLColor> color);
    void setFont(std::string fontName, int size, bool bold, bool italic, bool underline, bool strikeThrough);

    virtual void clear(shared_ptr<ZLColor> color) = 0;

    // string
    int getStringWidth(std::string string);
    virtual int getStringWidth(const char* string2, int offset, int length) = 0;
    virtual int getStringWidth(const short* string2, int offset, int length) = 0;
    int getSpaceWidth();
    int getStringHeight();
    int getCharHeight(char chr);
    int getDescent();
    virtual void drawString(int x, int y, const short* string, int offset, int length) = 0;
    virtual void drawString(int x, int y, const char* string, int offset, int length) = 0;
    void drawString(int x, int y, std::string string2);

    // image
    virtual shared_ptr<ZLPaintContext::Size> imageSize(const ZLImageData& image,
                                                       shared_ptr<ZLPaintContext::Size> maxSize,
                                                       ZLPaintContext_ScalingType scaling) = 0;
    virtual void drawImage(int x, int y, const ZLImageData& image, shared_ptr<ZLPaintContext::Size> maxSize, ZLPaintContext_ScalingType scaling, ColorAdjustingMode adjustingMode)=0;

    // rectangle
    virtual void drawLine(int x0, int y0, int x1, int y1) = 0;
    virtual void fillRectangle(int x0, int y0, int x1, int y1) = 0;

    // polygon
    virtual void drawPolygonalLine(int* xs, int* ys,int length) = 0;
    virtual void fillPolygon(int* xs, int* ys, long length, shared_ptr<ZLColor> color) = 0;
    virtual void drawOutline(int* xs, int* ys, long length) = 0;

    // circle
    virtual void fillCircle(int x, int y, int radius) = 0;

    // underline
    virtual void drawUnderline(std::vector<shared_ptr<ZLRect>> rectangles, shared_ptr<ZLColor> color, int noteCount) = 0;
    virtual void drawOpinion(int x, int y, int width, int height, int opinionCount) = 0;
    
    virtual void setFontInternal(std::string fontName, int size, bool bold, bool italic, bool underline, bool strikeThrough) = 0;
};

inline shared_ptr<SystemInfo> ZLPaintContext::getSystemInfo() {
    return mySystemInfo;
}

#endif /* ZLPaintContext_hpp */

