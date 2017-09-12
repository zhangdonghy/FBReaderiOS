//
//  ZDZLTextPlainModel.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/16.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ZDZLTextPlainModel.hpp"
#include "ZDZLTextParagraph.hpp"
#include "ZLTextParagraphImpl.hpp"
#include "ZLTextSpecialParagraphImpl.hpp"
#include "ZLFileImage.h"
#include "ZDZLImageEntry.hpp"
#include "ZLTextStyleEntry.h"
#include "ZDZLSearchUtil.hpp"

#include "ZDZLTextStyleEntry.hpp"
#include "ZLTextCSSStyleEntry.hpp"
#include "ZLTextOtherStyleEntry.hpp"

ZDZLTextPlainModel::~ZDZLTextPlainModel()
{
    delete myStartEntryIndices2;
}
ZDZLTextPlainModel::ZDZLTextPlainModel(std::string id,
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
                   std::map<std::string,shared_ptr<ZLFileImage>> imageMap/*,
                                                  FontManager fontManager*/)
{
    myArraySize = arraysize;
    myId = id;
    //myLanguage = language;
    myParagraphsNumber = paragraphsNumber;
    myStartEntryIndices2 = entryIndices;
    myStartEntryOffsets2 = entryOffsets;
    myParagraphLengths2 = paragraphLengths;
    
   //int  myDataIndex = myStartEntryIndices2[4];
    
    myTextSizes = textSizes;
    myParagraphKinds = paragraphKinds;
    myStorage = new CachedCharStorage(directoryName, fileExtension, blocksNumber);
    
    myImageMap = imageMap;
    
  //  myImageMap = imageMap;
   // myFontManager = fontManager;
}
std::string ZDZLTextPlainModel::getId()
{
    return myId;
}
 std::string ZDZLTextPlainModel::getLanguage()
{
    return myLaguange;
}
 int ZDZLTextPlainModel::getParagraphsNumber()
{
    return myParagraphsNumber;
}
//ZLTextParagraph getParagraph(int index);

 void ZDZLTextPlainModel::removeAllMarks()
{
    myMarks.clear();
}
 shared_ptr<ZDZLTextMark> ZDZLTextPlainModel::getFirstMark()
{
    return (myMarks.empty()) ? NULL : myMarks.front();
}
 shared_ptr<ZDZLTextMark> ZDZLTextPlainModel::getLastMark()
{
    return (myMarks.empty()) ? NULL : myMarks.back();
}
 shared_ptr<ZDZLTextMark> ZDZLTextPlainModel::getNextMark(shared_ptr<ZDZLTextMark> position)
{
    if (position == NULL || myMarks.empty()) {
        return NULL;
    }
    
    shared_ptr<ZDZLTextMark> mark = NULL;
    for (std::vector<shared_ptr<ZDZLTextMark>>::const_iterator it = myMarks.begin(); it != myMarks.end(); it++) {
        shared_ptr<ZDZLTextMark> current = *it;
        if (current->compareTo(*position) >= 0) {
            if ((mark == NULL) || (mark->compareTo(*current) > 0)) {
                mark = current;
            }
        }
    }
    return mark;
}
 shared_ptr<ZDZLTextMark> ZDZLTextPlainModel::getPreviousMark(shared_ptr<ZDZLTextMark> position)
{
    if (position == NULL || myMarks.empty()) {
        return NULL;
    }
    
    shared_ptr<ZDZLTextMark> mark = NULL;
    for (std::vector<shared_ptr<ZDZLTextMark>>::const_iterator it = myMarks.begin(); it != myMarks.end(); it++) {
        shared_ptr<ZDZLTextMark> current = *it;
        if (current->compareTo(*position) < 0) {
            if ((mark == NULL) || (mark->compareTo(*current) < 0)) {
                mark = current;
            }
        }
    }
    return mark;
}

std::vector<shared_ptr<ZDZLTextMark>> ZDZLTextPlainModel::getMarks() {
   /* std::vector<shared_ptr<ZDZLTextMark>>* val =  new std::vector<shared_ptr<ZDZLTextMark>>();
    return *val;*/
    return myMarks;
}

// text length for paragraphs from 0 to index
 int ZDZLTextPlainModel::getTextLength(int index)
{
    if (myArraySize == 0) {
        return 0;
    }
    return myTextSizes[std::max(std::min(index, myParagraphsNumber - 1), 0)];
}
 int ZDZLTextPlainModel::findParagraphByTextLength(int length)
{
    int index = binarySearch(myTextSizes, myParagraphsNumber, length);
    if (index >= 0) {
        return index;
    }
    return std::min(-index - 1, myParagraphsNumber - 1);
}

int ZDZLTextPlainModel::search(std::string text, long length, int startIndex, int endIndex, bool ignoreCase) {
    int count = 0;
    shared_ptr<ZDZLSearchPattern> pattern = new ZDZLSearchPattern(text, length, ignoreCase);
    myMarks.clear();
    if (startIndex > myParagraphsNumber) {
        startIndex = myParagraphsNumber;
    }
    if (endIndex > myParagraphsNumber) {
        endIndex = myParagraphsNumber;
    }
    int index = startIndex;
    shared_ptr<EntryIteratorImpl> it = new EntryIteratorImpl(this, index);

    while (true) {
        // 搜索一个paragraph
        // 开始搜索前将offset重置为0
        // 该offset是搜索的文字在paragraph里——只考虑文字类型，排除其他所有类型——的偏移坐标，该坐标是char类型字符的坐标，不是文字Element的索引
        int offset = 0;
        while (it->next()) {
            // 搜索paragraph中的一小段
//            printf("EntryIteratorImpl getType: %d\n", it->getType());
            if (it->getType() == ZDZLTextParagraph::TEXT) {
                // 只有该段为文字类型时才需要进行搜索
                short* textData = it->getTextData();
                int textOffset = it->getTextOffset();
                int textLength = it->getTextLength();
//                printf("text in charArr at index: %d, offset in charArr: %d, length of charArr: %d\n",
//                       index, textOffset, textLength);
                for (shared_ptr<ZDZLSearchUtil::Result> res = ZDZLSearchUtil::find(textData, textOffset, textLength, pattern);
                     res != NULL; res = ZDZLSearchUtil::find(textData, textOffset, textLength, pattern, res->Start + 1)) {
                    ZDZLTextMark* mark = new ZDZLTextMark(index, offset + res->Start, res->Length);
//                    printf("found result at paragraph: %d, offset of char in paragraph: %d, length of text: %d\n",
//                           index, mark->Offset, mark->Length);
                    myMarks.push_back(mark);
                    ++count;
                }
                offset += textLength;
            }
        }

        if (++index >= endIndex) {
            // 搜索结束
            break;
        }

        // 重置EntryIteratorImpl
        it->reset(index);
    }

    return count;
}

shared_ptr<ZDZLTextParagraph>
ZDZLTextPlainModel::getParagraph(int index) {
    uint8_t kind = myParagraphKinds[index];
    
    
    return (kind == ZDZLTextParagraph::TEXT_PARAGRAPH) ?
        new ZLTextParagraphImpl(this, index) :
        new ZLTextSpecialParagraphImpl(kind, this, index);
}

int ZDZLTextPlainModel::binarySearch(int* array, int length, int value) {
    int lowIndex = 0;
    int highIndex = length - 1;
    
    while (lowIndex <= highIndex) {
        int midIndex = (lowIndex + highIndex) >> 1;
        int midValue = array[midIndex];
        if (midValue > value) {
            highIndex = midIndex - 1;
        } else if (midValue < value) {
            lowIndex = midIndex + 1;
        } else {
            return midIndex;
        }
    }
    return -lowIndex - 1;
}

///////////////////
uint8_t
ZDZLTextPlainModel::EntryIteratorImpl::getType()
{
    return myType;
}

short*
ZDZLTextPlainModel::EntryIteratorImpl::getTextData()
{
    return myTextData;
}
int ZDZLTextPlainModel::EntryIteratorImpl::getTextOffset()
{
    return myTextOffset;
}
int ZDZLTextPlainModel::EntryIteratorImpl::getTextLength()
{
    return myTextLength;
}

unsigned char ZDZLTextPlainModel::EntryIteratorImpl::getControlKind()
{
   return myControlKind;
}
bool ZDZLTextPlainModel::EntryIteratorImpl::getControlIsStart()
{
    return myControlIsStart;
}

unsigned char ZDZLTextPlainModel::EntryIteratorImpl::getHyperlinkType()
{
    return myHyperlinkType;
}
std::string ZDZLTextPlainModel::EntryIteratorImpl::getHyperlinkId()
{
    return myHyperlinkId;
}

shared_ptr<ZDZLImageEntry> ZDZLTextPlainModel::EntryIteratorImpl::getImageEntry()
{
    return myImageEntry;
}
shared_ptr<ZLVideoEntry> ZDZLTextPlainModel::EntryIteratorImpl::getVideoEntry()
{
    return 0;
}
shared_ptr<ExtensionEntry> ZDZLTextPlainModel::EntryIteratorImpl::getExtensionEntry()
{
    return 0;
}
shared_ptr<ZDZLTextStyleEntry> ZDZLTextPlainModel::EntryIteratorImpl::getStyleEntry()
{
    return myStyleEntry;
}

short ZDZLTextPlainModel::EntryIteratorImpl::getFixedHSpaceLength()
{
    return myFixedHSpaceLength;
}
std::string getString(short* value,int offset,int length)
{
    std::string res = "";
    for(int i=0;i<length;i++){
        res.append((char*)(value+offset+i),1);
    }
    return res;
}
bool ZDZLTextPlainModel::EntryIteratorImpl::next()
{
    if (myCounter >= myLength) {
        return false;
    }
    int dataOffset = myDataOffset;
   // int dataIndex = myDataIndex;
    
    int readcnt=0;
    short* data =  (short*)mm->myStorage->block(myDataIndex,&readcnt);
    readcnt = readcnt/2;
    if (data == 0) {
        return false;
    }
    if (dataOffset >= readcnt) {
        data = (short*)mm->myStorage->block(++myDataIndex,&readcnt);
        readcnt = readcnt/2;
        if (data == 0) {
            return false;
        }
        dataOffset = 0;
    }
    
    short first = (short)data[dataOffset];
    uint8_t type = (uint8_t)first;
    if (type == 0) {
        data = (short*)mm->myStorage->block(++myDataIndex,&readcnt);
        readcnt = readcnt/2;
        if (data == 0) {
            return false;
        }
        dataOffset = 0;
        first = (short)data[0];
        type = (uint8_t)first;
    }
    myType = type;
    ++dataOffset;
    //printf("myType:%d,myDataOffset%d\n",myType,myDataOffset);
    switch (type) {
        case ZDZLTextParagraph::TEXT:
        {
            int textLength = (int)data[dataOffset++];
            textLength += (((int)data[dataOffset++]) << 16);
            textLength = std::min(textLength, readcnt - dataOffset);
            myTextLength = textLength;
            myTextData =  data;
            myTextOffset = dataOffset;
            dataOffset += textLength;
        }
        break;
        case ZDZLTextParagraph::CONTROL:
        {
            short kind = (short)data[dataOffset++];
            myControlKind = (uint8_t)kind;
            myControlIsStart = (kind & 0x0100) == 0x0100;
            myHyperlinkType = 0;
            break;
        }
        case ZDZLTextParagraph::HYPERLINK_CONTROL:
        {
            short kind = (short)data[dataOffset++];
            myControlKind = (uint8_t)kind;
            myControlIsStart = true;
            myHyperlinkType = (uint8_t)(kind >> 8);
            short labelLength = (short)data[dataOffset++];
           // myHyperlinkId = std::string(data, dataOffset, labelLength);
            myHyperlinkId = "";
            //myHyperlinkId.append((char*)(data+dataOffset),labelLength);
            for(int i=0;i<labelLength;i++){
                myHyperlinkId.append((char*)(data+dataOffset+i),1);
            }
            dataOffset += labelLength;
            break;
        }
        case ZDZLTextParagraph::IMAGE:
        {
             short vOffset = (short)data[dataOffset++];
             short len = (short)data[dataOffset++];
            //std::string id(len);
            //std::string id =  std::string(data+dataOffset, len);
            std::string id = "";
            for(int i=0;i<len;i++){
                id.append((char*)(data+dataOffset+i),1);
            }
            dataOffset += len;
            bool isCover = data[dataOffset++] != 0;
            myImageEntry = new ZDZLImageEntry(mm->myImageMap, id, vOffset, isCover);
            break;
        }
        case ZDZLTextParagraph::FIXED_HSPACE:
        myFixedHSpaceLength = (short)data[dataOffset++];
        break;
        case ZDZLTextParagraph::STYLE_CSS:
        case ZDZLTextParagraph::STYLE_OTHER:
        {
            short depth = (short)((first >> 8) & 0xFF);
            shared_ptr<ZDZLTextStyleEntry> entry =
                    type == ZDZLTextParagraph::STYLE_CSS
                    ? (ZDZLTextStyleEntry*)(new ZLTextCSSStyleEntry(depth))
                        : (ZDZLTextStyleEntry*)(new ZLTextOtherStyleEntry());
        
            short mask = (short)data[dataOffset++];
            
            for (int i = 0; i < ZLTextStyleEntry::NUMBER_OF_LENGTHS; ++i) {
                if (ZDZLTextStyleEntry::isFeatureSupported(mask, i)) {
                    short size = (short)data[dataOffset++];
                    uint8_t unit = (uint8_t)data[dataOffset++];
                    if(size==150){
                       entry->setLength(i, size, unit);
                    } else {
                      entry->setLength(i, size, unit);
                    }
                    
                }
            }
            if (ZDZLTextStyleEntry::isFeatureSupported(mask, ZLTextStyleEntry::ALIGNMENT_TYPE) ||
                ZDZLTextStyleEntry::isFeatureSupported(mask, ZLTextStyleEntry::NON_LENGTH_VERTICAL_ALIGN)) {
                 short value = (short)data[dataOffset++];
                if (ZDZLTextStyleEntry::isFeatureSupported(mask, ZLTextStyleEntry::ALIGNMENT_TYPE)) {
                    entry->setAlignmentType((uint8_t)(value & 0xFF));
                }
                if (ZDZLTextStyleEntry::isFeatureSupported(mask, ZLTextStyleEntry::NON_LENGTH_VERTICAL_ALIGN)) {
                    entry->setVerticalAlignCode((uint8_t)((value >> 8) & 0xFF));
                }
            }
            if (ZDZLTextStyleEntry::isFeatureSupported(mask, ZLTextStyleEntry::FONT_FAMILY)) {
               // entry->setFontFamilies(myFontManager, (short)data[dataOffset++]);
               short xxxxxx = (short)data[dataOffset++];
            }
            if (ZDZLTextStyleEntry::isFeatureSupported(mask, ZLTextStyleEntry::FONT_STYLE_MODIFIER)) {
                 short value = (short)data[dataOffset++];
                entry->setFontModifiers((uint8_t)(value & 0xFF), (uint8_t)((value >> 8) & 0xFF));
            }
            
            myStyleEntry = entry;
        }
        
        case ZDZLTextParagraph::STYLE_CLOSE:
        // No data
        break;
        case ZDZLTextParagraph::RESET_BIDI:
        // No data
        break;
        case ZDZLTextParagraph::AUDIO:
        // No data
        break;
        case ZDZLTextParagraph::VIDEO:
        {
           // myVideoEntry = new ZLVideoEntry();
             short mapSize = (short)data[dataOffset++];
            for (short i = 0; i < mapSize; ++i) {
                short len = (short)data[dataOffset++];
                //final String mime = new String(data, dataOffset, len);
                std::string mime = getString(data,dataOffset,len);
                dataOffset += len;
                len = (short)data[dataOffset++];
                //final String src = new String(data, dataOffset, len);
                std::string src = getString(data,dataOffset,len);
                dataOffset += len;
                //myVideoEntry.addSource(mime, src);
            }
            break;
        }
        case ZDZLTextParagraph::EXTENSION:
        {
             short kindLength = (short)data[dataOffset++];
            // String kind = new String(data, dataOffset, kindLength);
            std::string kind = getString(data,dataOffset,kindLength);
            dataOffset += kindLength;
            
           // final Map<String,String> map = new HashMap<String,String>();
             short dataSize = (short)((first >> 8) & 0xFF);
            for (short i = 0; i < dataSize; ++i) {
                 short keyLength = (short)data[dataOffset++];
                // String key = new String(data, dataOffset, keyLength);
                dataOffset += keyLength;
                 short valueLength = (short)data[dataOffset++];
                //map.put(key, new String(data, dataOffset, valueLength));
                dataOffset += valueLength;
            }
            //myExtensionEntry = new ExtensionEntry(kind, map);
            break;
        }
        default:
        break;
    }
    ++myCounter;
    myDataOffset = dataOffset;
    return true;
}



