//
//  ZDBook.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/1.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZDBook_hpp
#define ZDBook_hpp

#include <string>
#include "AbstractBook.hpp"

class ZDBook: public AbstractBook
{
private:
    std::string myPath;
public:
    ZDBook(long id, std::string path, std::string title, std::string encoding, std::string language)
        :AbstractBook(id, title, encoding, language)
    {
        
        myPath = path;
    }
    
};
#endif /* ZDBook_hpp */
