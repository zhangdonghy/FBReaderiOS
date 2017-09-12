//
//  ZLViewEnums_PageIndex.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/31.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLViewEnums_PageIndex_hpp
#define ZLViewEnums_PageIndex_hpp


class ZLViewEnums_PageIndex
{
public:
    static const int invalid = -1;
    static const int previous = 0;
    static const int current = 1;
    static const int next = 2;
    int getNext();
    int getPrevious();
    int getValue();
    ZLViewEnums_PageIndex(int v);
    
    
    /*static const bool leftToRight =true;
    static const bool rightToLeft =true;
    static const bool up =false;
    static const bool down =false;*/
    enum Direction {
        //leftToRight(true), rightToLeft(true), up(false), down(false);
        leftToRight,
        rightToLeft,
        up,
        down
       /* public:
        bool IsHorizontal;
        
        Direction(bool isHorizontal) {
            IsHorizontal = isHorizontal;
        }*/
        /*bool IsHorizontal(){
            return false;
        }*/
    };
    static bool IsHorizontal(Direction d){
        if(d == leftToRight || d == rightToLeft) return true;
        return  false;
    }
    enum Animation {
        none, curl, slide, slideOldStyle, shift
    };
    private:
    int value;
};


#endif /* ZLViewEnums_PageIndex_hpp */
