//
//  ZLTextViewBase.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/31.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ZLTextViewBase.hpp"
#include "ZDZLibrary.hpp"
#include "ZLTextHyperlinkControlElement.hpp"
#include "ZLTextNGStyle.hpp"

ZLTextViewBase::ZLTextViewBase(ZLApplication* application)
    :ZLView(application)
{
    
}
void
ZLTextViewBase::applyControl(shared_ptr<ZLTextElement> control) {
    ZLTextControlElement& controlElement = (ZLTextControlElement&)*control;
    if (controlElement.IsStart) {
        shared_ptr<ZLTextHyperlink> hyperlink = control->kind() == ZLTextElement::HYPER_LINK_ELEMENT// instanceof ZLTextHyperlinkControlElement
        ? ((ZLTextHyperlinkControlElement&)*control).Hyperlink : 0;
        
        shared_ptr<ZLTextNGStyleDescription> description =
        getTextStyleCollection()->getDescription(controlElement.Kind);
        if (description != 0) {
            setTextStyle(new ZLTextNGStyle(myTextStyle, description, hyperlink));
        }
    } else {
        setTextStyle(myTextStyle->Parent);
    }
}
int ZLTextViewBase::maxSelectionDistance() {
    if (myMaxSelectionDistance == 0) {
        myMaxSelectionDistance = ZDZLibrary::Instance()->getDisplayDPI() / 20;
    }
    // zhangdong
    if(myMaxSelectionDistance==0) return 150;
    return myMaxSelectionDistance;
}
void ZLTextViewBase::resetMetrics() {
    myMetrics = 0;
}

int ZLTextViewBase::getWordHeight() {
    if (myWordHeight == -1) {
       myWordHeight = getContext()->getStringHeight() * myTextStyle->getLineSpacePercent() / 100
        + myTextStyle->getVerticalAlign(metrics());
    }
    return myWordHeight;
}

int ZLTextViewBase::getElementHeight(ZLTextElement& element) {
    if (element.kind() == ZLTextElement::NB_HSPACE_ELEMENT
        || element.kind() == ZLTextElement::WORD_ELEMENT
        || element.kind() == ZLTextElement::FIXED_HSPACE_ELEMENT) {
        return getWordHeight();

    } else if (element.kind() == ZLTextElement::IMAGE_ELEMENT) {
        shared_ptr<ZLPaintContext::Size> size = getContext()->imageSize(*(((ZLTextImageElement&)element).ImageData),
                                                                        getTextAreaSize(),
                                                                        IntegerCoefficient);
        return (size != 0 ? size->Height : 0)
        + std::max(getContext()->getStringHeight() * (myTextStyle->getLineSpacePercent() - 100) / 100, 3);

    } else if (element.kind() == ZLTextElement::VIDEO_ELEMENT) {
        return 200;
//        return Math.min(Math.min(200, getTextAreaHeight()), getTextColumnWidth() * 2 / 3);

    } else if (element.kind() == ZLTextElement::EXTENSION_ELEMENT) {
        return ((ExtensionElement&)element).getHeight();
    }

    return 0;
}
int ZLTextViewBase::getElementDescent(ZLTextElement& element)
{
    if (element.kind() == ZLTextElement::WORD_ELEMENT) {
        return getContext()->getDescent();
    }
    return 0;
}
int ZLTextViewBase::getWordWidth(ZLTextWord&  word, int start, int length, bool addHyphenationSign) {
    if (length == -1) {
        if (start == 0) {
            return word.getWidth(getContext());
        }
        length = word.Length - start;
    }
    if (!addHyphenationSign) {
        return getContext()->getStringWidth(word.Data, word.Offset + start, length);
    }
    short* part = myWordPartArray;
    if (length + 1 > 20) {
        part = new short[length + 1];
        myWordPartArray = part;
    }
    //System.arraycopy(word->Data, word->Offset + start, part, 0, length);
    memcpy(part, &word.Data[word.Offset + start], length);
    
    part[length] = '-';
    return getContext()->getStringWidth(part, 0, length + 1);
}

void ZLTextViewBase::drawString(shared_ptr<ZLPaintContext> context, int x, int y, const short* str, int offset, int length,
                                ZLTextWord::Mark* mark, shared_ptr<ZLColor> color, int shift) {
    if (mark == 0) {
        context->setTextColor(color);
        context->drawString(x, y, str, offset, length);
    } else {
        int pos = 0;
        for (; (mark != 0) && (pos < length); mark = mark->getNext()) {
            int markStart = mark->Start - shift;
            int markLen = mark->Length;
            
            if (markStart < pos) {
                markLen += markStart - pos;
                markStart = pos;
            }
            
            if (markLen <= 0) {
                continue;
            }
            
            if (markStart > pos) {
                int endPos = std::min(markStart, length);
                context->setTextColor(color);
                context->drawString(x, y, str, offset + pos, endPos - pos);
                x += context->getStringWidth(str, offset + pos, endPos - pos);
            }
            
            if (markStart < length) {
                context->setFillColor(getHighlightingBackgroundColor());
                int endPos = std::min(markStart + markLen, length);
                 int endX = x + context->getStringWidth(str, offset + markStart, endPos - markStart);
                context->fillRectangle(x, y - context->getStringHeight(), endX - 1, y + context->getDescent());
                context->setTextColor(getHighlightingForegroundColor());
                context->drawString(x, y, str, offset + markStart, endPos - markStart);
                x = endX;
            }
            pos = markStart + markLen;
        }
        
        if (pos < length) {
            context->setTextColor(color);
            context->drawString(x, y, str, offset + pos, length - pos);
        }
    }
}

void ZLTextViewBase::drawWord(int x, int y, ZLTextWord& word, int start, int length,
                              bool addHyphenationSign, shared_ptr<ZLColor> color) {
     shared_ptr<ZLPaintContext> context = getContext();
    if (start == 0 && length == -1) {
        drawString(context, x, y, word.Data, word.Offset, word.Length, word.getMark(), color, 0);
    } else {
        if (length == -1) {
            length = word.Length - start;
        }
        if (!addHyphenationSign) {
            drawString(context, x, y, word.Data, word.Offset + start, length, word.getMark(), color, start);
        } else {
            //char[] part = myWordPartArray;
            short* part = myWordPartArray;
            if (length + 1 > 20) {
                part = new short[length + 1];
                myWordPartArray = part;
            }
            //System.arraycopy(word.Data, word.Offset + start, part, 0, length);
            memcpy(part, &word.Data[word.Offset + start], length);
            part[length] = '-';
            drawString(context, x, y, part, 0, length + 1, word.getMark(), color, start);
        }
    }
}

