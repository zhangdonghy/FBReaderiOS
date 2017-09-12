//
//  ZDZLBookModel.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/16.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ZDBookModel.hpp"
#include "ZDZLTextPlainModel.hpp"
#include "ZDTOCTree.hpp"
#include "ECatalogTree.hpp"

shared_ptr<ZDZLTextModel> ZDBookModel::createTextModel(std::string id, std::string language, int paragraphsNumber,
                                                       const std::size_t arraysize,
                                                       int* entryIndices, int* entryOffsets,
                                                       int* paragraphLenghts, int* textSizes, uint8_t* paragraphKinds,
                                                       std::string directoryName, std::string fileExtension, int blocksNumber) {
    return new ZDZLTextPlainModel(id, language, paragraphsNumber,arraysize,
                                  entryIndices, entryOffsets,
                                  paragraphLenghts, textSizes, paragraphKinds,
                                  directoryName, fileExtension, blocksNumber, myImageMap/*, FontManager*/);
}

ZDBookModel::ZDBookModel() {
    printf("new ZDBookModel\n");
    myECatalogTree = new ECatalogTree();
}

ZDBookModel::~ZDBookModel() {
    printf("~ZDBookModel\n");
    if (myECatalogTree != 0) {
        delete myECatalogTree;
        myECatalogTree = 0;
    }
}

/*
void ZDBookModel::addImage(std::string id,ZDZLImage* image)
{
    myImageMap.insert(make_pair(id, *image));
}*/


////// static function
shared_ptr<ZDBookModel>
ZDBookModel::createModel(shared_ptr<ZDBook> book)
{
    return  0;
}
 void
ZDBookModel::initWithModel(shared_ptr<BookModel> bookModel)
{
    shared_ptr<ZLTextModel> textModel = bookModel->bookTextModel();
    std::string id = textModel->id();
    std::string language = textModel->language();
    int paragraphsNumber = textModel->paragraphsNumber();
    const std::size_t arraysSize = textModel->startEntryIndices().size();
    
    int* entryIndices = new int[arraysSize];
    int* entryOffsets = new int[arraysSize];
    int* paragraphLenghts = new int[arraysSize];
    int* textSizes = new int[arraysSize];
    uint8_t* paragraphKinds2 = new uint8_t[arraysSize];
    
    for(int i=0;i<arraysSize;i++){
        int entryIndicesItem = textModel->startEntryIndices()[i];
        entryIndices[i] = entryIndicesItem;
        
        int entryOffsetsItem = textModel->startEntryOffsets()[i];
        entryOffsets[i] = entryOffsetsItem;
        
        int paralenitem = textModel->paragraphLengths()[i];
        paragraphLenghts[i] = paralenitem;
        
        int textsizeitem = textModel->textSizes()[i];
        textSizes[i] = textsizeitem;
        
        uint8_t para = textModel->paragraphKinds()[i];
        paragraphKinds2[i] = para;
    }
    
    setBookTextModel(createTextModel(id, language, paragraphsNumber,arraysSize,
                                                           entryIndices, entryOffsets,
                                                           paragraphLenghts,textSizes, paragraphKinds2, textModel->allocator().directoryName(), textModel->allocator().fileExtension(), (int)textModel->allocator().blocksNumber()));
   // return thismodel;
}
shared_ptr<ZDBookModel>
ZDBookModel::createModel(shared_ptr<BookModel> bookModel)
{
    // create text model from bookModel
    ZDBookModel* thismodel = new ZDBookModel();
    shared_ptr<ZLTextModel> textModel = bookModel->bookTextModel();
    std::string id = textModel->id();
    std::string language = textModel->language();
    int paragraphsNumber = textModel->paragraphsNumber();
    const std::size_t arraysSize = textModel->startEntryIndices().size();
    
    int* entryIndices = new int[arraysSize];
    int* entryOffsets = new int[arraysSize];
    int* paragraphLenghts = new int[arraysSize];
    int* textSizes = new int[arraysSize];
    uint8_t* paragraphKinds2 = new uint8_t[arraysSize];
    
    
    for(int i=0;i<arraysSize;i++){
        int entryIndicesItem = textModel->startEntryIndices()[i];
        entryIndices[i] = entryIndicesItem;
        
        int entryOffsetsItem = textModel->startEntryOffsets()[i];
        entryOffsets[i] = entryOffsetsItem;
        
        int paralenitem = textModel->paragraphLengths()[i];
        paragraphLenghts[i] = paralenitem;
        
        int textsizeitem = textModel->textSizes()[i];
        textSizes[i] = textsizeitem;
        
        uint8_t para = textModel->paragraphKinds()[i];
        paragraphKinds2[i] = para;
    }
    
    thismodel->setBookTextModel(thismodel->createTextModel(id, language, paragraphsNumber,
                                                           arraysSize,
                                                           entryIndices, entryOffsets,
                                                           paragraphLenghts,textSizes, paragraphKinds2,
                                                           textModel->allocator().directoryName(),
                                                           textModel->allocator().fileExtension(),
                                                           (int)textModel->allocator().blocksNumber()));
    return thismodel;
    /*shared_ptr<ZDZLTextModel> textModel = new ZDZLTextModel
    (
    
    );*/
    
    
    // set text model
    
    return  0;
}

void ZDBookModel::addImage(std::string id, const ZLFileImage& image)
{
    // shared_ptr<ZLImage> xx = (ZLImage*)&image;
    //const ZLImage &x = image;
    ZLFileImage::Blocks x;
    //shared_ptr<ZLFileImage> xx = new ZLFileImage(image.file(),image.encoding(),x);
    //myImageMap.insert(std::make_pair(id, xx));
    myImageMap[id] = new ZLFileImage(image.file(),image.encoding(),x);
    
    //myImageMap[id] = image;
}

void ZDBookModel::addTOCItem(std::string text,int reference)
{
//    printf(">>>>>>>>>addTOCItem text: %s, reference: %d\n",text.c_str(), reference);
    myECatalogTree->enterNode(text, reference);
    //myCurrentTree = new ZDTOCTree(myCurrentTree);
   // myCurrentTree->setText(text);
   // myCurrentTree->setReference(myBookTextModel, reference);
}
void ZDBookModel::leaveTOCItem()
{
//    printf("<<<<<<<<leaveTOCItem\n");
    myECatalogTree->exitNode();
   // myCurrentTree = myCurrentTree->Parent;
   // if (myCurrentTree == 0) {
   //     myCurrentTree = TOCTree;
   // }
}
