//
//  ZLTextPage.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/31.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLTextPage_hpp
#define ZLTextPage_hpp

#include <vector>
#include <stdio.h>
#include "shared_ptr.h"
#include "ZLTextElementAreaVector.hpp"
#include "ZLTextWordCursor.hpp"
#include "ZLTextLineInfo.hpp"
//#include "ZLTextView.hpp"

enum  PaintStateEnum {
     NOTHING_TO_PAINT = 0,
     READY = 1,
     START_IS_KNOWN = 2,
     END_IS_KNOWN = 3,
     TO_SCROLL_FORWARD = 4,
     TO_SCROLL_BACKWARD = 5
};

class ZLTextView;
class ZLTextPage {
private:
    int Column0Height;
    
    int myColumnWidth;
    int myHeight;
    bool myTwoColumnView;
    
protected:

public:
    bool twoColumnView() {
        return myTwoColumnView;
    }
    int getColumn0Height() {
        return Column0Height;
    };
    void setColumn0Height(int height) {
        Column0Height = height;
    };
    
    shared_ptr<ZLTextWordCursor> StartCursor;// = new ZLTextWordCursor();
    shared_ptr<ZLTextWordCursor> EndCursor ;//= new ZLTextWordCursor();
    
    //std::list<shared_ptr<ZLTextLineInfo>> LineInfos ;//= new ArrayList<ZLTextLineInfo>();
    std::vector<shared_ptr<ZLTextLineInfo>> LineInfos ;
    
    int PaintState = NOTHING_TO_PAINT;
    shared_ptr<ZLTextElementAreaVector> TextElementMap;
    ZLTextPage();
    void setSize(int columnWidth, int height, bool twoColumnView, bool keepEndNotStart) {
        if (myColumnWidth == columnWidth && myHeight == height && myColumnWidth == columnWidth) {
            return;
        }
        myColumnWidth = columnWidth;
        myHeight = height;
        myTwoColumnView = twoColumnView;
        
        if (PaintState != NOTHING_TO_PAINT) {
            LineInfos.clear();
            if (keepEndNotStart) {
                if (!EndCursor->isNull()) {
                    StartCursor->reset();
                    PaintState = END_IS_KNOWN;
                } else if (!StartCursor.isNull()) {
                    EndCursor->reset();
                    PaintState = START_IS_KNOWN;
                }
            } else {
                if (!StartCursor->isNull()) {
                    EndCursor->reset();
                    PaintState = START_IS_KNOWN;
                } else if (!EndCursor->isNull()) {
                    StartCursor->reset();
                    PaintState = END_IS_KNOWN;
                }
            }
        }
    }
    
    
    void findLineFromEnd(shared_ptr<ZLTextWordCursor> cursor, int overlappingValue) {
        if (LineInfos.size()==0 || (overlappingValue == 0)) {
            cursor->reset();
            return;
        }
        /*final ArrayList<ZLTextLineInfo> infos = LineInfos;
        final int size = infos.size();
        ZLTextLineInfo info = null;
        for (int i = size - 1; i >= 0; --i) {
            info = infos.get(i);
            if (info.IsVisible) {
                --overlappingValue;
                if (overlappingValue == 0) {
                    break;
                }
            }
        }
        cursor.setCursor(info.ParagraphCursor);
        cursor.moveTo(info.StartElementIndex, info.StartCharIndex);*/
    }
    
    void findPercentFromStart(shared_ptr<ZLTextWordCursor> cursor, int percent) {
        if (LineInfos.size() ==0 ) {
            cursor->reset();
            return;
        }
        /*int height = myHeight * percent / 100;
        bool visibleLineOccured = false;
        ZLTextLineInfo info = null;
        for (ZLTextLineInfo i : LineInfos) {
            info = i;
            if (info.IsVisible) {
                visibleLineOccured = true;
            }
            height -= info.Height + info.Descent + info.VSpaceAfter;
            if (visibleLineOccured && (height <= 0)) {
                break;
            }
        }
        cursor->setCursor(info.ParagraphCursor);
        cursor.moveTo(info.EndElementIndex, info.EndCharIndex);*/
    }
    void findLineFromStart(shared_ptr<ZLTextWordCursor> cursor, int overlappingValue) {
       /* if (LineInfos.isEmpty() || (overlappingValue == 0)) {
            cursor.reset();
            return;
        }
        ZLTextLineInfo info = null;
        for (ZLTextLineInfo i : LineInfos) {
            info = i;
            if (info.IsVisible) {
                --overlappingValue;
                if (overlappingValue == 0) {
                    break;
                }
            }
        }
        cursor.setCursor(info.ParagraphCursor);
        cursor.moveTo(info.EndElementIndex, info.EndCharIndex);*/
    }
    bool isEmptyPage() {
        /*for (ZLTextLineInfo info : LineInfos) {
            if (info.IsVisible) {
                return false;
            }
        }*/
        /*for(std::list<shared_ptr<ZLTextLineInfo>>::const_iterator it = LineInfos.begin();it != LineInfos.end();++it){
            if((*it)->IsVisible){
                return false;
            }
            
        }*/
        for(int i=0;i<LineInfos.size();i++){
            if(LineInfos[i]->IsVisible) {
                return false;
            }
        }
        return true;
    }
    int getTextWidth() {
        return myColumnWidth;
    }
    
    int getTextHeight() {
        return myHeight;
    }
    void reset() {
        StartCursor->reset();
        EndCursor->reset();
        LineInfos.clear();
        PaintState = NOTHING_TO_PAINT;
    }
    void moveStartCursor(shared_ptr<ZLTextParagraphCursor> cursor) {
        StartCursor->setCursor(cursor);
        EndCursor->reset();
        LineInfos.clear();
        PaintState = START_IS_KNOWN;
    }
    
    void moveStartCursor(int paragraphIndex, int wordIndex, int charIndex) {
        if (StartCursor->isNull()) {
            StartCursor->setCursor(EndCursor);
        }
        StartCursor->moveToParagraph(paragraphIndex);
        StartCursor->moveTo(wordIndex, charIndex);
        EndCursor->reset();
        LineInfos.clear();
        PaintState = START_IS_KNOWN;
    }
    void moveEndCursor(int paragraphIndex, int wordIndex, int charIndex) {
        if (EndCursor->isNull()) {
            EndCursor->setCursor(StartCursor);
        }
        EndCursor->moveToParagraph(paragraphIndex);
        if ((paragraphIndex > 0) && (wordIndex == 0) && (charIndex == 0)) {
            EndCursor->previousParagraph();
            EndCursor->moveToParagraphEnd();
        } else {
            EndCursor->moveTo(wordIndex, charIndex);
        }
        StartCursor->reset();
        LineInfos.clear();
        PaintState = END_IS_KNOWN;
    }

    friend class ZLTextView;
};
#endif /* ZLTextPage_hpp */

