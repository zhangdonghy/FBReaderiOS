//
//  ZDZLBookModel.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/16.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZDBookModel_hpp
#define ZDBookModel_hpp

#include <stdio.h>
#include <map>
#include "ZDZLTextModel.hpp"
#include "ZDTOCTree.hpp"
#include "ZDZLImage.hpp"
#include "ZDBook.hpp"
#include "BookModel.h"
#include "ZLFileImage.h"

class ECatalogTree;

class ZDBookModel {

private:
    ECatalogTree* myECatalogTree;
public :
     // Book Book;
    //ZDTOCTree* TOCTree;
    /*public final FontManager FontManager = new FontManager();
    
    protected CachedCharStorage myInternalHyperlinks;
    
    
   ** protected final HashMap<String,ZLTextModel> myFootnotes = new HashMap<String,ZLTextModel>();
    */
    
protected:
     shared_ptr<ZDZLTextModel> myBookTextModel;
    
    std::map<std::string,shared_ptr<ZLFileImage>> myImageMap;// = new std::map<std::string,ZDZLImage>();
public:
    ZDBookModel();
    ~ZDBookModel();
    ECatalogTree* getECatalogTree(){
        return myECatalogTree;
    }
    shared_ptr<ZDZLTextModel> createTextModel(std::string id, std::string language, int paragraphsNumber,
                                              const std::size_t arraysize,
                                              int* entryIndices, int* entryOffsets,
                                              int* paragraphLenghts, int* textSizes, uint8_t* paragraphKinds,
                                              std::string directoryName, std::string fileExtension, int blocksNumber);
    //void addImage(std::string id,ZDZLImage* image);
    static shared_ptr<ZDBookModel> createModel(shared_ptr<ZDBook> book);
    static shared_ptr<ZDBookModel> createModel(
                                               shared_ptr<BookModel> bookModel);
    
    void initWithModel(shared_ptr<BookModel> bookModel);
    
    shared_ptr<ZDZLTextModel> getTextModel() {
        return myBookTextModel;
    }
    void setBookTextModel(shared_ptr<ZDZLTextModel> model){
        myBookTextModel = model;
    }
    void addImage(std::string id, const ZLFileImage& image);
    
    void addTOCItem(std::string text,int reference);
    void leaveTOCItem();
    
    // toc
    ZDTOCTree* myCurrentTree;
};
#endif /* ZDZLBookModel_hpp */
