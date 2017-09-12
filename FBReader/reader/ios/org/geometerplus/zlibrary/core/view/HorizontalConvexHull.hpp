//
//  HorizontalConvexHull.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/8.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef HorizontalConvexHull_hpp
#define HorizontalConvexHull_hpp

#include <vector>
#include <stdio.h>
#include "Hull.hpp"
#include "ZLRect.hpp"
#include <limits>
class HorizontalConvexHull: public Hull
{
    private:
    std::vector<shared_ptr<ZLRect>> myRectangles;
    void addRect(shared_ptr<ZLRect> rectangle) {
        if (myRectangles.size()==0) {
            myRectangles.insert(myRectangles.end(), new ZLRect(*rectangle));
            return;
        }
        int top = rectangle->Top;
        int bottom = rectangle->Bottom;
        
        for (std::vector<shared_ptr<ZLRect>>::const_iterator it = myRectangles.begin(); it != myRectangles.end(); ++it) {
           // myCurrentTextModel->addControl(*it, true);
            shared_ptr<ZLRect> r = *it;
            if (r->Bottom <= top) {
                continue;
            }
            if (r->Top >= bottom) {
                break;
            }
            if (r->Top < top) {
                ZLRect* before = new ZLRect(*r);
                before->Bottom = top;
                r->Top = top;
                // insert
                //  myRectangles[i] = before;
                it = myRectangles.insert(it, before);
                ++it;
                /*iter.previous();
                 iter.add(before);
                 iter.next();*/
            }
            if (r->Bottom > bottom) {
                ZLRect* after = new ZLRect(*r);
                after->Top = bottom;
                r->Bottom = bottom;
                it = myRectangles.insert(++it, after);
                //iter.add(after);
            }
            r->Left = std::min(r->Left, rectangle->Left);
            r->Right = std::max(r->Right, rectangle->Right);
            //myRectangles.insert(it, 0);
        }
 
        
        shared_ptr<ZLRect> first = myRectangles[0];
        if (top < first->Top) {
            //myRectangles.add(0, new Rect(rectangle.left, top, rectangle.right, Math.min(bottom, first.top)));
            myRectangles.insert(myRectangles.begin(), new ZLRect(rectangle->Left, top, rectangle->Right, std::min(bottom, first->Top)));
        }
        
          shared_ptr<ZLRect> last = myRectangles[myRectangles.size()-1];
        if (bottom > last->Bottom) {
            //myRectangles.add(new Rect(rectangle.left, Math.max(top, last.bottom), rectangle.right, bottom));
            myRectangles.insert(myRectangles.end(), new ZLRect(rectangle->Left, std::max(top, last->Bottom), rectangle->Right, bottom));
        }
    }
    void normalize() {
        shared_ptr<ZLRect> previous = 0;
        int i=0;
        for (std::vector<shared_ptr<ZLRect>>::const_iterator it = myRectangles.begin(); it != myRectangles.end(); /*++it*/) {
            //final Rect current = iter.next();
            i++;
            shared_ptr<ZLRect> current = *it;
            if(current == 0) {
               // break;
            }
            if (previous != 0) {
                if ((previous->Left == current->Left) && (previous->Right == current->Right)) {
                    previous->Bottom = current->Bottom;
                    //iter.remove();
                    it = myRectangles.erase(it);
                    continue;
                }
                if ((previous->Bottom != current->Top) &&
                    (current->Left <= previous->Right) &&
                    (previous->Left <= current->Right)) {
                    it = myRectangles.insert(it, new ZLRect(
                                                       std::max(previous->Left, current->Left),
                                                       previous->Bottom,
                                                       std::min(previous->Right, current->Right),
                                                       current->Top
                                                    ));
                    ++it;
                    /*iter.previous();
                    iter.add(new Rect(
                                      Math.max(previous.left, current.left),
                                      previous.bottom,
                                      Math.min(previous.right, current.right),
                                      current.top
                                      ));
                    iter.next();*/
                }
            }
            ++it;
            previous = current;
        }
    }
    public:
    HorizontalConvexHull(std::vector<shared_ptr<ZLRect>>& rects) {
        /*for (Rect r : rects) {
            addRect(r);
        }*/
        for(int i=0;i<rects.size();i++){
            //shared_ptr<ZLRect> rect = rects[i];
            if(rects[i] != 0) {
                    addRect(rects[i]);
            } else {
                addRect(rects[i]);
            }
            
            //myRectangles.insert(myRectangles.end(), rect);
        }
        normalize();
    }
     int distanceTo(int x, int y) {
        int distance  = std::numeric_limits<int>::max();
        //for (Rect r : myRectangles) {
         for(int i=0;i<myRectangles.size();i++){
             int xd = myRectangles[i]->Left > x ? myRectangles[i]->Left - x : (myRectangles[i]->Right < x ? x - myRectangles[i]->Right : 0);
             int yd = myRectangles[i]->Top > y ? myRectangles[i]->Top - y : (myRectangles[i]->Bottom < y ? y - myRectangles[i]->Bottom : 0);
            distance = std::min(distance, std::max(xd, yd));
            if (distance == 0) {
                break;
            }
        }
        return distance;
    }
    
     bool isBefore(int x, int y) {
        //for (Rect r : myRectangles) {
         for(int i=0;i<myRectangles.size();i++){
            if (myRectangles[i]->Bottom < y || (myRectangles[i]->Top < y && myRectangles[i]->Right < x)) {
                return true;
            }
        }
        // std::list<int> c;
        // c::const_iterator x;
         
        return false;
    }

    void draw(shared_ptr<ZLPaintContext> context, int mode, shared_ptr<ZLColor> color, int noteCount);
};
#endif /* HorizontalConvexHull_hpp */
