//
//  ZLTextHyperlink.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/2.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLTextHyperlink_hpp
#define ZLTextHyperlink_hpp

#include <vector>
#include <stdio.h>
#include <string>
#include "shared_ptr.h"

class ZLTextHyperlink {
public:
    uint8_t Type;
    std::string Id;
    
    std::vector<int> myElementIndexes;
    
    static  shared_ptr<ZLTextHyperlink> NO_LINK;
    
    ZLTextHyperlink(uint8_t type, std::string id) {
        Type = type;
        Id = id;
    }
    
    void addElementIndex(int elementIndex) {
        //if (myElementIndexes == null) {
        //    myElementIndexes = new LinkedList<Integer>();
        //}
        myElementIndexes.insert(myElementIndexes.end(),elementIndex);
    }
    
    std::vector<int> elementIndexes() {
       /* return myElementIndexes != null
        ? Collections.unmodifiableList(myElementIndexes)
        : Collections.<Integer>emptyList();*/
        return myElementIndexes;
    }
};

#endif /* ZLTextHyperlin
k_hpp */
