//
//  ZDZLTextParagraph.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/2.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZDZLTextParagraph_hpp
#define ZDZLTextParagraph_hpp

#include <string>
#include <stdio.h>
#include "shared_ptr.h"

class  ZDZLImageEntry;
class ZLVideoEntry;
class ExtensionEntry;
class ZDZLTextStyleEntry;

//#include "ZLTextParagraphCursor.hpp"



class ZDZLTextParagraph
{
public:
    enum Entry {
        TEXT = 1,
        IMAGE = 2,
        CONTROL = 3,
        HYPERLINK_CONTROL = 4,
        STYLE_CSS = 5,
        STYLE_OTHER = 6,
        STYLE_CLOSE = 7,
        FIXED_HSPACE = 8,
        RESET_BIDI = 9,
        AUDIO = 10,
        VIDEO = 11,
        EXTENSION = 12
    };
    class EntryIterator {
        virtual unsigned char getType()=0;
        
        virtual short* getTextData()=0;
        virtual int getTextOffset()=0;
        virtual int getTextLength()=0;
        
        virtual unsigned char getControlKind()=0;
        virtual bool getControlIsStart()=0;
        
        virtual unsigned char getHyperlinkType()=0;
        virtual std::string getHyperlinkId()=0;
        
        virtual shared_ptr<ZDZLImageEntry> getImageEntry()=0;
        virtual shared_ptr<ZLVideoEntry> getVideoEntry()=0;
        virtual shared_ptr<ExtensionEntry> getExtensionEntry()=0;
        virtual shared_ptr<ZDZLTextStyleEntry> getStyleEntry()=0;
        
        virtual short getFixedHSpaceLength()=0;
        
        virtual bool next()=0;
        friend class ZDZLTextParagraph;
        friend class ZLTextView;
        friend class ZLTextParagraphCursor;
    };
    
    virtual shared_ptr<EntryIterator> iterator()=0;
    virtual unsigned char getKind()=0;
    
    enum  Kind {
        TEXT_PARAGRAPH = 0,
        //byte TREE_PARAGRAPH = 1;
        EMPTY_LINE_PARAGRAPH = 2,
        BEFORE_SKIP_PARAGRAPH = 3,
        AFTER_SKIP_PARAGRAPH = 4,
        END_OF_SECTION_PARAGRAPH = 5,
        PSEUDO_END_OF_SECTION_PARAGRAPH = 6,
        END_OF_TEXT_PARAGRAPH = 7,
        ENCRYPTED_SECTION_PARAGRAPH = 8
    };
};
#endif /* ZDZLTextParagraph_hpp */
