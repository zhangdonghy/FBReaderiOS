//
//  ZLTextViewBase.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/31.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLTextViewBase_hpp
#define ZLTextViewBase_hpp

#include <stdio.h>
#include "ZLView.hpp"
#include "ZLTextStyle.hpp"
#include "ZLTextMetrics.hpp"
#include "ZDZLibrary.hpp"
#include "ZLPaintContext.hpp"
#include "ZLTextElement.hpp"
#include "ZLTextImageElement.hpp"
#include "ZLTextParagraphCursor.hpp"
#include "ZLTextWord.hpp"
#include "ExtensionElement.hpp"
#include "ZLTextFixedHSpaceElement.hpp"
#include "ZLTextStyleCollection.hpp"
#include "ZLTextExplicitlyDecoratedStyle.hpp"
#include "ZLTextStyleElement.hpp"
#include "ZLTextControlElement.hpp"

class ZLColor;
class ZLFile;
class ZLTextImageElement;
class ZLTextStyleElement;
class ZLTextStyleCollection;
class ZLTextHyperlink;

enum ZLTextViewBase_ImageFitting {
    none, covers, all
};

class ZLTextViewBase: public ZLView {
private:
     shared_ptr<ZLTextStyle> myTextStyle;
    //ZLTextStyle* myTextStyle;
     int myWordHeight = -1;
     shared_ptr<ZLTextMetrics> myMetrics;
    int myMaxSelectionDistance = 0;
    void applyStyle(shared_ptr<ZLTextElement> element){//(ZLTextStyleElement& element) {
        setTextStyle(new ZLTextExplicitlyDecoratedStyle(myTextStyle,
                                                        ((ZLTextStyleElement&)*element).Entry
                                                        ));
    }
protected:
    ZLTextViewBase(ZLApplication*  application) ;
    int maxSelectionDistance();
    void resetMetrics() ;
    int getWordHeight();
public:
// protected
    shared_ptr<ZLTextMetrics> metrics() {
        // this local variable is used to guarantee null will not
        // be returned from this method enen in multi-thread environment
        //ZLTextMetrics* m = myMetrics;
        if (myMetrics == 0) {
            myMetrics = new ZLTextMetrics(
                                          ZDZLibrary::Instance()->getDisplayDPI(),
                                  // TODO: screen area width
                                  500,
                                  // TODO: screen area height
                                  700,
                                  18//getTextStyleCollection().getBaseStyle().getFontSize()
                                  );
        }
        return myMetrics;
    }
    
public:
    
    virtual shared_ptr<ZLColor> getBackgroundColor() = 0;
    virtual void setBackgroundColor(int r, int g, int b) = 0;
    
    shared_ptr<ZLPaintContext::Size> getTextAreaSize() {
        return new ZLPaintContext::Size(getTextColumnWidth(), getTextAreaHeight());
    }
    
    int getTextAreaHeight() {
        return getContextHeight() - getTopMargin() - getBottomMargin();
    }
    
    int getColumnIndex(int x) {
        if (!twoColumnView()) {
            return -1;
        }
        return 2 * x <= getContextWidth() + getLeftMargin() - getRightMargin() ? 0 : 1;
    }
    
    int getTextColumnWidth() {
        return twoColumnView()
        ? (getContextWidth() - getLeftMargin() - getSpaceBetweenColumns() - getRightMargin()) / 2
        : getContextWidth() - getLeftMargin() - getRightMargin();
    }
    
     shared_ptr<ZLTextStyle> getTextStyle() {
        if (myTextStyle == 0) {
            resetTextStyle();
        }
        return myTextStyle;
    }
    
     void setTextStyle( shared_ptr<ZLTextStyle> style) {
         
        if (myTextStyle != style) {
            myTextStyle = style;
            myWordHeight = -1;
        }
         
        getContext()->setFont(style->getFontName(),/*style->getFontEntries(),*/ style->getFontSize(metrics()), style->isBold(), style->isItalic(), style->isUnderline(), style->isStrikeThrough());
    }
    
    void resetTextStyle() {
        setTextStyle(getTextStyleCollection()->getBaseStyle());
    }
    
    bool isStyleChangeElement(shared_ptr<ZLTextElement> element) {
        return element == ZLTextElement::StyleClose
        || element->kind() == ZLTextElement::TEXT_STYLE_ELEMENT
        || element->kind() == ZLTextElement::TEXT_CONTROL_ELEMENT ; //element instance of ZLTextControlElement;
    }
    
    void applyStyleChangeElement(shared_ptr<ZLTextElement> element) {
       
        if (element->kind() == ZLTextElement::STYLE_CLOSE_ELEMENT){//ZLTextElement.StyleClose) {
            applyStyleClose();
        } else if (element->kind() == ZLTextElement::TEXT_STYLE_ELEMENT){//(element instanceof ZLTextStyleElement) {
            applyStyle(element);//((ZLTextStyleElement)element);
        } else if (element->kind() == ZLTextElement::TEXT_CONTROL_ELEMENT ){//instanceof ZLTextControlElement) {
            applyControl(element);
        }
    }
    
    void applyStyleChanges(shared_ptr<ZLTextParagraphCursor> cursor, int index, int end) {
        for (; index != end; ++index) {
            applyStyleChangeElement(cursor->getElement(index));
        }
    }
    
    void applyControl(shared_ptr<ZLTextElement> control);
    
    
    
    void applyStyleClose() {
        setTextStyle((myTextStyle->Parent));
    }
    
    ZLPaintContext_ScalingType
        getScalingType(const ZLTextImageElement& imageElement) {
        switch (getImageFitting()) {
            default:
            case none:
                return IntegerCoefficient;
            case covers:
                return imageElement.IsCover
                ? FitMaximum
                : IntegerCoefficient;
            case all:
                return FitMaximum;
        }
    }
    int getWordWidth( ZLTextWord& word, int start) {
        return
        start == 0 ?
        word.getWidth(getContext()) :
        getContext()->getStringWidth(word.Data, word.Offset + start, word.Length - start);
    }
    int getWordWidth(shared_ptr<ZLTextWord> word, int start, int length) {
        return getContext()->getStringWidth(word->Data, word->Offset + start, length);
    }
    short* myWordPartArray = new short[20];
    
    int getWordWidth(ZLTextWord& word, int start, int length, bool addHyphenationSign) ;
    
    int getElementWidth(ZLTextElement& element, int charIndex) {
         // refer zhangdong 0.99.4
         //std::string xx = typeid(element);
       // ZLTextElement* xx = element.getT();
         switch (element.kind()) {
             case ZLTextElement::WORD_ELEMENT:
                return getWordWidth((ZLTextWord&)element, charIndex);
            // case ZLTextElement.image
             case ZLTextElement::IMAGE_ELEMENT:
             {
                 // shared_ptr<ZLTextImageElement> imageElement = (ZLTextImageElement*)element.getT();
                 
                  shared_ptr<ZLPaintContext::Size> size = getContext()->imageSize(
                                                                                *(((ZLTextImageElement&)element).ImageData)
                                                                         /*imageElement->ImageData*/,
                                                                         getTextAreaSize(),
                                                                         /*getScalingType(imageElement)*/
                                                                                  IntegerCoefficient
                                                                         );
                 return size != 0 ? size->Width : 0;
             }
             //case video zhangdong not process now
             case ZLTextElement::VIDEO_ELEMENT:
                return 300;
             case  ZLTextElement::EXTENSION_ELEMENT:
                return ((ExtensionElement&)element).getWidth();
             case ZLTextElement::FIXED_HSPACE_ELEMENT:
                 return getContext()->getSpaceWidth() * ((ZLTextFixedHSpaceElement&)element).Length;
             default:
             if(element.kind() == ZLTextElement::NB_HSPACE_ELEMENT) {
                 return getContext()->getSpaceWidth();
             } else if(element.kind() == ZLTextElement::INDENT_ELEMENT){
                 return myTextStyle->getFirstLineIndent(metrics());
             }
             break;
         }
         /*
        if (element instanceof ZLTextWord) {
            return getWordWidth((ZLTextWord)element, charIndex);
        } else if (element instanceof ZLTextImageElement) {
            final ZLTextImageElement imageElement = (ZLTextImageElement)element;
            final ZLPaintContext.Size size = getContext().imageSize(
                                                                    imageElement.ImageData,
                                                                    getTextAreaSize(),
                                                                    getScalingType(imageElement)
                                                                    );
            return size != null ? size.Width : 0;
        } else if (element instanceof ZLTextVideoElement) {
            return Math.min(300, getTextColumnWidth());
        } else if (element instanceof ExtensionElement) {
            return ((ExtensionElement)element).getWidth();
        } else if (element == ZLTextElement.NBSpace) {
            return getContext().getSpaceWidth();
        } else if (element == ZLTextElement.Indent) {
            return myTextStyle.getFirstLineIndent(metrics());
        } else if (element instanceof ZLTextFixedHSpaceElement) {
            return getContext().getSpaceWidth() * ((ZLTextFixedHSpaceElement)element).Length;
        }*/
        return 0;
    }
    
    int getElementHeight(ZLTextElement& element) ;
    int getElementDescent(ZLTextElement& element) ;
    
    void drawWord(int x, int y,  ZLTextWord& word, int start, int length, bool addHyphenationSign, shared_ptr<ZLColor> color) ;
        
    void drawString(shared_ptr<ZLPaintContext> context, int x, int y, const short* str, int offset, int length, ZLTextWord::Mark* mark, shared_ptr<ZLColor> color, int shift) ;
    // virtual funtion
    virtual  shared_ptr<ZLTextStyleCollection> getTextStyleCollection()=0;
    
    virtual ZLTextViewBase_ImageFitting getImageFitting()=0;
    
    virtual int getLeftMargin()=0;
    virtual int getRightMargin()=0;
    virtual int getTopMargin()=0;
    virtual int getBottomMargin()=0;
    virtual int getSpaceBetweenColumns()=0;
    
    virtual bool twoColumnView()=0;
    
    virtual shared_ptr<ZLFile> getWallpaperFile()=0;
    virtual ZLPaintContext_FillMode getFillMode()=0;
    
    virtual shared_ptr<ZLColor> getSelectionBackgroundColor() = 0;
    virtual shared_ptr<ZLColor> getSelectionCursorColor() = 0;
    virtual shared_ptr<ZLColor> getSelectionForegroundColor() = 0;
    virtual shared_ptr<ZLColor> getHighlightingBackgroundColor() = 0;
    virtual shared_ptr<ZLColor> getHighlightingForegroundColor() = 0;
    virtual shared_ptr<ZLColor> getOutlineColor() = 0;
    virtual shared_ptr<ZLColor> getUnderlineColor(shared_ptr<ZLTextPosition> start, shared_ptr<ZLTextPosition> end) = 0;
    virtual shared_ptr<ZLColor> getTextColor(shared_ptr<ZLTextHyperlink> hyperlink) = 0;
    virtual void setTextColor(int r, int g, int b) = 0;
    
    virtual int getNoteCount(shared_ptr<ZLTextPosition> start, shared_ptr<ZLTextPosition> end) = 0;

    // opinion
    virtual int getOpinionCount(int pIndex) = 0;
    
    friend class ZLTextSelection;
    
};
#endif /* ZLTextViewBase_hpp */
