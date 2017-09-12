//
//  AbstractBook.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/1.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef AbstractBook_hpp
#define AbstractBook_hpp

#include <stdio.h>
#include <string>
class AbstractBook
{
    enum SaveState {
        Saved,
        ProgressNotSaved,
        NotSaved
    };
private:
    
protected:
    
    std::string myEncoding;
    std::string myLanguage;
    long myId;
    SaveState mySaveState;
public:
   
    static  std::string FAVORITE_LABEL ;
    AbstractBook(long id, std::string title, std::string encoding, std::string language) {
        //super(title);
        myId = id;
        myEncoding = encoding;
        myLanguage = language;
        mySaveState = Saved;
    }
};
#endif /* AbstractBook_hpp */
