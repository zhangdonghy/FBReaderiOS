//
//  UnionHull.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/8.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef UnionHull_hpp
#define UnionHull_hpp

#include <limits>
#include <vector>
#include <stdio.h>
#include "Hull.hpp"
class UnionHull: public Hull {
private:
    std::vector<shared_ptr<Hull>> myComponents;

public:
    UnionHull(std::vector<shared_ptr<Hull>>& components) {
        myComponents = components;
    }
    void draw(shared_ptr<ZLPaintContext> context, int mode, shared_ptr<ZLColor> color, int noteCount) {
        for(int i=0;i<myComponents.size();i++){
            myComponents[i]->draw(context, mode, color);
        }
    }
    int distanceTo(int x, int y) {
        int dist = std::numeric_limits<int>::max();//Integer::MAX_VALUE;
        for(int i=0;i<myComponents.size();i++){
            dist = std::min(dist,myComponents[i]->distanceTo(x, y));
        }
        return dist;
    }
    
    bool isBefore(int x, int y) {
        for(int i=0;i<myComponents.size();i++){
            if (myComponents[i]->isBefore(x, y)) {
                return true;
            }
        }
        return false;
    }
};
#endif /* UnionHull_hpp */
