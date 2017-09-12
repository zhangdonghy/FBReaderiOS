//
//  ZDZLTree.h
//  ZDStudent
//
//  Created by zhangdong on 16/8/16.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZDZLTree_h
#define ZDZLTree_h
#include <list>
#include "Iterable.h"
#include <vector>
//: public Iterable<T>


template<class T>
class ZDZLTree  : public Iterable<T>{
    private :
    int mySize = 1;
    T* Parent;
    int Level;
    std::vector<T*>* mySubtrees;
    
protected:
    ZDZLTree()
        :Iterable<T>((T*)0)
    {
        ZDZLTree((T*)0);
    }
     ZDZLTree(T* parent)
    :Iterable<T>(parent)
    {
        ZDZLTree(parent, -1);
    }
    int getSize() {
        return mySize;
    }
    ZDZLTree(T* parent, int position)
    :Iterable<T>(parent)
    {
        if (position == -1) {
            position = parent == 0x0 ? 0 : parent->subtrees()->size();
        }
        if (parent != 0x0 && (position < 0 || position > parent->subtrees()->size())) {
            
        }
        Parent = parent;
        if (parent != 0x0) {
            Level = parent->Level + 1;
            parent->addSubtree((T*)this, position);
        } else {
            Level = 0;
        }
    }
    std::vector<T*>*  subtrees() {
        if (mySubtrees == 0) {
        //if(mySubtrees.size()==0){
           // return Collections.emptyList();
            std::vector<T*>* sub = new std::vector<T*>();
            return sub;
        }
        //synchronized (mySubtrees)
        {
            return mySubtrees; // just return pointer !!!
           // return new ArrayList<T>(mySubtrees);
        }
    }
    void addSubtree(T* subtree, int position) {
        if (mySubtrees == 0) {
            //mySubtrees = Collections.synchronizedList(new ArrayList<T>());
            mySubtrees = new std::vector<T*>();
        }
        int subtreeSize = subtree->getSize();//subtree.getSize();
        //synchronized (mySubtrees)
        {
             //int thisSubtreesSize = mySubtrees->size();
            ///while (position < thisSubtreesSize) {
              //  subtree = mySubtrees.set(position++, subtree);
           /// }
            ///mySubtrees.add(subtree);
            //mySubtrees->insert(position,subtree);
            mySubtrees->insert(mySubtrees->begin()+position,subtree);
          //  mySubtrees->
            //for(T* curr = Parent;curr!=0;curr=curr->Parent){
                
            //}
            mySize += subtreeSize;
            if(Parent != 0) {
                for (T* parent = Parent; parent != 0; parent = parent->Parent) {
                    parent->mySize += subtreeSize;
                }
            }
            
        }
    }
    public:
    
    friend class ZDBookModel;
};
/*
template<class  T>
inline ZDZLTree<T>::ZDZLTree(T* parent, int position) {
 
}*/
#endif /* ZDZLTree_h */
