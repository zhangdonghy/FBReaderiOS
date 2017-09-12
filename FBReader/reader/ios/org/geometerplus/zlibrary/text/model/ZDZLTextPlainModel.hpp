//
//  ZDZLTextPlainModel.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/16.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZDZLTextPlainModel_hpp
#define ZDZLTextPlainModel_hpp
#include <map>
//#include <list>
//#include <stdio.h>
#include <vector>
#include "ZDZLTextModel.hpp"
#include "ZLFileImage.h"

#include "ZDZLTextMark.hpp"
#include "ZDZLTextParagraph.hpp"
#include "CachedCharStorage.hpp"

//using namespace std;
class ZDZLTextStyleEntry;
class ZDZLImageEntry;
class ZDZLVideoEntry;
class ZDExtensionEntry;
class CachedCharStorage;

class ZDZLTextPlainModel: public ZDZLTextModel {


private:
    std::string myId;
    std::string myLaguange;
    
    int myArraySize;
    int *myTextSizes;
    uint8_t *myParagraphKinds;//[];
    int myParagraphsNumber;
  
    shared_ptr<CachedCharStorage> myStorage;
    std::map<std::string,shared_ptr<ZLFileImage>> myImageMap;
     
     /*
     
     private final FontManager myFontManager;
     */
    std::vector<shared_ptr<ZDZLTextMark>> myMarks;
    
    int binarySearch(int* array, int length, int value);

public:
    // private
    int32_t *myStartEntryOffsets2;
    int32_t *myStartEntryIndices2;

public:
    int32_t *myParagraphLengths2;
    ~ZDZLTextPlainModel();
    ZDZLTextPlainModel(std::string id,
                       std::string language,
                       int paragraphsNumber,
                       const std::size_t arraysize,
                       int* entryIndices,
                       int* entryOffsets,
                       int* paragraphLengths,
                       int* textSizes,
                       uint8_t* paragraphKinds,
                       std::string directoryName,
                       std::string fileExtension,
                       int blocksNumber,
                       std::map<std::string, shared_ptr<ZLFileImage>> imageMap/*,
                       FontManager fontManager*/);
    
    virtual std::string getId();
    virtual std::string getLanguage();
    virtual int getParagraphsNumber();
    //ZLTextParagraph getParagraph(int index);
    
    virtual void removeAllMarks();
    virtual shared_ptr<ZDZLTextMark> getFirstMark();
    virtual shared_ptr<ZDZLTextMark> getLastMark();
    virtual shared_ptr<ZDZLTextMark> getNextMark(shared_ptr<ZDZLTextMark> position);
    virtual shared_ptr<ZDZLTextMark> getPreviousMark(shared_ptr<ZDZLTextMark> position);
    
    virtual std::vector<shared_ptr<ZDZLTextMark>> getMarks();
    
    // text length for paragraphs from 0 to index
    virtual int getTextLength(int index);
    virtual int findParagraphByTextLength(int length);
    
    virtual int search(std::string text, long length, int startIndex, int endIndex, bool ignoreCase);
    
    virtual shared_ptr<ZDZLTextParagraph> getParagraph(int index) ;
    
    friend class ZLTextView;

public:
    class EntryIteratorImpl :public ZDZLTextParagraph::EntryIterator {
        private:
         int myCounter;
         int myLength;
         uint8_t myType;
        
        int32_t myDataIndex;
        int32_t myDataOffset;
        ZDZLTextPlainModel* mm;
        
        short* myTextData;
        int myTextOffset;
        int myTextLength;

        // ControlEntry data
         uint8_t myControlKind;
         bool myControlIsStart;
 
        // HyperlinkControlEntry data
         uint8_t myHyperlinkType;
        std::string myHyperlinkId;
 
        shared_ptr<ZDZLImageEntry> myImageEntry;
        shared_ptr<ZDZLVideoEntry> myVideoEntry;
        shared_ptr<ZDExtensionEntry> myExtensionEntry;
        shared_ptr<ZDZLTextStyleEntry> myStyleEntry;
        short myFixedHSpaceLength;
        
        
    public:
        friend class ZDZLTextPlainModel;
        friend class ZLTextView;
        EntryIteratorImpl(ZDZLTextPlainModel* model, int index) {
            mm = model;
            reset(index);
        }
        unsigned char getType();
        
         short* getTextData();
         int getTextOffset();
         int getTextLength();
        
         unsigned char getControlKind();
         bool getControlIsStart();
        
         unsigned char getHyperlinkType();
         std::string getHyperlinkId();
        
        shared_ptr<ZDZLImageEntry> getImageEntry();
         shared_ptr<ZLVideoEntry> getVideoEntry();
         shared_ptr<ExtensionEntry> getExtensionEntry();
         shared_ptr<ZDZLTextStyleEntry> getStyleEntry();
        
         short getFixedHSpaceLength();
        
         bool next();
        
         void reset(int index) {
            myCounter = 0;
            //uint8_t* xx = mm->myParagraphKinds;
             
            myLength = mm->myParagraphLengths2[index];
            
            myDataIndex = mm->myStartEntryIndices2[index];
            myDataOffset = mm->myStartEntryOffsets2[index];
        }
    };
    friend class EntryIteratorImpl;
};
#endif /* ZDZLTextPlainModel_hpp */
