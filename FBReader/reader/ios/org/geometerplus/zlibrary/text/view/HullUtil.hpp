//
//  HullUtil.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/8.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef HullUtil_hpp
#define HullUtil_hpp

#include <stdio.h>
#include <vector>
#include "Hull.hpp"
#include "ZLTextElementArea.hpp"
#include "ZLRect.hpp"


class HullUtil
{
    public:
    static shared_ptr<Hull> hull(shared_ptr<ZLTextElementArea>* list, int length);
    
    static shared_ptr<Hull> hull(std::vector<shared_ptr<ZLTextElementArea>>& list);
};
#endif /* HullUtil_hpp */
