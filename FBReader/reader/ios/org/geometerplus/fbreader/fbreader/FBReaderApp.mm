//
//  FBReaderApp.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/31.
//  Copyright © 2016年 Eagley. All rights reserved.
//


#include "FormatPlugin.h"
#include "Book.h"
#include "BookModel.h"
#include "FormatPlugin.h"
#include "ZLibrary.h"
//#include "PluginCollection.h"
#import <Foundation/Foundation.h>
#include "ZDZLTree.h"

#import "ZDPluginCollection.h"

#include "ZDBookModel.hpp"

#include "FBReaderApp.h"
#include "ZDFBReader.h"
#include "FixedTextSnippet.hpp"
#include "ZLTextWordCursor.hpp"
#include "ZLUnderlineHighlighting.hpp"
#include "ZLOpinionHighlighting.hpp"
#include "ZLSpeechHighlighting.hpp"
#include "ZLTextFixedPosition.hpp"
#include "AutoTextSnippet.hpp"
#include "Book.h"
#include "ParagraphTextSnippet.hpp"
#include "TextBuildTraverser.hpp"

FBReaderApp::FBReaderApp()
{
    new Config();
    zdBookModel = 0;
    aIsEnglishBook = false;
    BookTextView = new FBView(this);
    setView(&(*BookTextView));
    myReadStart = 0;
    
}
FBReaderApp::~FBReaderApp()
{
   //delete zdBookModel
}
static bool ct_compare(const shared_ptr<ContentsTree> &first, const shared_ptr<ContentsTree> &second) {
    return first->reference() < second->reference();
}
static void initTOC(ZDBookModel* zdBookModel, const ContentsTree& tree)
{
    std::vector<shared_ptr<ContentsTree> > children = tree.children();
    std::sort(children.begin(), children.end(), ct_compare);
    for (std::vector<shared_ptr<ContentsTree> >::const_iterator it = children.begin(); it != children.end(); ++it) {
        const ContentsTree &child = **it;
        //child.text();
        //JString text(env, child.text());
        //AndroidUtil::Method_BookModel_addTOCItem->call(javaModel, text.j(), child.reference());
        
        zdBookModel->addTOCItem(child.text(), child.reference());
        //initTOC(env, javaModel, child);
        initTOC(zdBookModel,child);
        zdBookModel->leaveTOCItem();
       // AndroidUtil::Method_BookModel_leaveTOCItem->call(javaModel);
    }
}

void FBReaderApp::getLastElementOfParagraph(int pIndex, int* eIndex, int* cIndex) {
    shared_ptr<ZLTextWordCursor> startCursor = new ZLTextWordCursor(BookTextView->getStartCursor());
    while (!startCursor->isLastParagraph()) {
        int pCurrent = startCursor->getParagraphIndex();
        if (pCurrent > pIndex) {
            *eIndex = 0;
            *cIndex = 0;
            return;
        }

        if (pCurrent == pIndex) {
            *eIndex = startCursor->getParagraphCursor()->getParagraphLength() - 1;
            *cIndex = 0;
            return;
        }

        startCursor->nextParagraph();
    }
}

bool FBReaderApp::openBook(const char* bookpath, int length) {
    [ZDPluginCollection sharedInstance];

    // cache dir
    NSArray* paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
    NSString* cacheDirectory = [paths objectAtIndex:0];

    // init library
    int argc = 0;
    char** argv = 0;
    ZLibrary::init(argc, argv);
    ZLibrary::initApplication("FBReader");

    // book path
    zdBookModel = new ZDBookModel();
    shared_ptr<Book> book = Book::loadFromBookPath(bookpath);

    // get plugin
    PluginCollection& p2 = PluginCollection::Instance();
    std::vector<shared_ptr<FormatPlugin>> plugins = p2.plugins();
    shared_ptr<FormatPlugin> one = plugins[0]; // just one
    //one->readLanguageAndEncoding(*book);
    
    shared_ptr<BookModel> bookModel = new BookModel(book,[cacheDirectory UTF8String]);
    bookModel->setJavaModel(zdBookModel);
   
    one->readModel(*bookModel);
    bool flushSuccess = bookModel->flush();
    if (!flushSuccess) {
        return false;
    }
    
    // 转换oc的上下文
    //shared_ptr<ZDBookModel> zdBookModel = ZDBookModel::createModel(bookModel);
    
    // toc first
    initTOC(zdBookModel,*bookModel->contentsTree());
    
   const std::vector<std::vector<std::string>> familyLists  = bookModel->fontManager().familyLists();
    for (std::vector<std::vector<std::string>>::const_iterator it = familyLists.begin(); it != familyLists.end(); ++it) {
        const std::vector<std::string> &lst = *it;
        //jobjectArray jList = env->NewObjectArray(lst.size(), AndroidUtil::Class_java_lang_String.j(), 0);
        for (std::size_t i = 0; i < lst.size(); ++i) {
            //JString jString(env, lst[i]);
            //env->SetObjectArrayElement(jList, i, jString.j());
            printf(" List item: %s\n",lst[i].c_str());
        }
        //AndroidUtil::Method_BookModel_registerFontFamilyList->call(javaModel, jList);
        //env->DeleteLocalRef(jList);
    }
    
    const std::map<std::string,shared_ptr<FontEntry>> entries = bookModel->fontManager().entries();
    for (std::map<std::string,shared_ptr<FontEntry>>::const_iterator it = entries.begin(); it != entries.end(); ++it) {
        if (it->second.isNull()) {
            continue;
        }
        printf("family: %s normal:%s\n",it->first.c_str(),it->second->Normal->Path.c_str());
        //JString family(env, it->first);
       // jobject normal = createJavaFileInfo(env, it->second->Normal);
        //jobject bold = createJavaFileInfo(env, it->second->Bold);
       // jobject italic = createJavaFileInfo(env, it->second->Italic);
      //  jobject boldItalic = createJavaFileInfo(env, it->second->BoldItalic);
        
        //AndroidUtil::Method_BookModel_registerFontEntry->call(
            //                                                  javaModel, family.j(), normal, bold, italic, boldItalic
                                                            //  );
        
        //if (boldItalic != 0) env->DeleteLocalRef(boldItalic);
      //  if (italic != 0) env->DeleteLocalRef(italic);
      //  if (bold != 0) env->DeleteLocalRef(bold);
       // if (normal != 0) env->DeleteLocalRef(normal);
    }

    if (!bookModel->book()->language().empty()) {
        std::string str = bookModel->book()->language();
        long offset = str.find("zh");
        if (offset < 0) {
            //printf("offset is %d\n",offset);
            aIsEnglishBook = true;
        }
    }
    zdBookModel->initWithModel(bookModel);
    //zdBookModel got the data
    
    BookTextView->setModel(zdBookModel->getTextModel(), length);

    return true;
}

std::string FBReaderApp::getSelectStr() {
    shared_ptr<TextSnippet> snippet = ((FBView&)(*BookTextView)).getSelectedSnippet();
    if (snippet == 0) {
        return "";
    } else {
        return ((FixedTextSnippet&)(*(snippet))).getText();
    }
}

void FBReaderApp::clearSelectText() {
    BookTextView->clearSelection();
}

#pragma mark - catalog
ECatalogTree* FBReaderApp::getECatalogTree(){
    return zdBookModel->getECatalogTree();
}

#pragma mark - selection area
void FBReaderApp::getSelectionArea(int* startX, int* startY, int* endX, int* endY) {
    shared_ptr<ZLTextSelection> selection = BookTextView->getSelection();
    printf("selection, p:%d  e:%d, c:%d\n",
           selection->getEndPosition()->getParagraphIndex(),
           selection->getEndPosition()->getElementIndex(),
           selection->getEndPosition()->getCharIndex());

    *startX = BookTextView->getSelectionStartX();
    *startY = BookTextView->getSelectionStartY();
    *endX = BookTextView->getSelectionEndX();
    *endY = BookTextView->getSelectionEndY();
}

bool FBReaderApp::isPressedOnCursor(int x,int y)
{
    float maxDist = ZDZLibrary::Instance()->getDisplayDPI() / 4;
    SelectionCursor::Which cursor = BookTextView->findSelectionCursor(x, y, maxDist * maxDist);
    if (cursor != SelectionCursor::None) {
        return true;
    }
    /*SelectionCursor::Which cursor = BookTextView->getSelectionCursorInMovement();
    if(cursor != SelectionCursor::None) {
        return  true;
    }*/
    return  false;
}
bool FBReaderApp::isPressedOnSelectedRegion(int x,int y)
{
    return BookTextView->isPressedOnSelectedRegion(x, y);
}

void FBReaderApp::getSelectionLocation(int* startPIndex, int* startEIndex, int* startCIndex,
                                       int* endPIndex, int* endEIndex, int* endCIndex,
                                       int* percent, std::string* word) {
    shared_ptr<ZLTextPosition> startPos = BookTextView->getSelectionStartPosition();
    if (startPos != 0) {
        *startPIndex = startPos->getParagraphIndex();
        *startEIndex = startPos->getElementIndex();
        *startCIndex = startPos->getCharIndex();
    }
    shared_ptr<ZLTextPosition> endPos = BookTextView->getSelectionEndPosition();
    if (endPos != 0) {
        *endPIndex = endPos->getParagraphIndex();
        *endEIndex = endPos->getElementIndex();
        *endCIndex = endPos->getCharIndex();
    }
    *percent = (int)curPercent();
    *word = getSelectStr();
}

void
FBReaderApp::getPageStart(int* ParagraphIndex,int* ElementIndex,int* CharIndex,int* Percent,std::string* word)
{
    shared_ptr<ZLTextWordCursor> start = BookTextView->getStartCursor();
    if(start != 0) {
        *ParagraphIndex = start->getParagraphIndex();
         *ElementIndex = start->getElementIndex();
         *CharIndex = start->getCharIndex();
        
        *Percent = (int)curPercent();
        
        *word = "";
        AutoTextSnippet xx(start,80);
        *word = xx.getText();
    }
}
void
FBReaderApp::jumpToLocation(int ParagraphIndex,int ElementIndex,int CharIndex)
{
    BookTextView->gotoPosition(ParagraphIndex, ElementIndex, CharIndex);
    getViewWidget()->repaint();
}

bool FBReaderApp::isInCurPage(int curPIndex, int curEIndex, int curCIndex) {
    int startPIndex = 0, startEIndex = 0, startCIndex = 0;
    int endPIndex = 0, endEIndex = 0, endCIndex = 0;
    shared_ptr<ZLTextWordCursor> start = BookTextView->getStartCursor();
    if (start != 0) {
        startPIndex = start->getParagraphIndex();
        startEIndex = start->getElementIndex();
        startCIndex = start->getCharIndex();
//        NSLog(@"page start: %d, %d, %d", startPIndex, startEIndex, startCIndex);
    }
    shared_ptr<ZLTextWordCursor> end = BookTextView->getEndCursor();
    if (end != 0) {
        endPIndex = end->getParagraphIndex();
        endEIndex = end->getElementIndex();
        endCIndex = end->getCharIndex();
//        NSLog(@"page end: %d, %d, %d", endPIndex, endEIndex, endCIndex);
    }
    
    NSInteger curELen = [NSString stringWithFormat:@"%d", curEIndex].length;
    NSInteger curCLen = [NSString stringWithFormat:@"%d", curCIndex].length;
    NSInteger startELen = [NSString stringWithFormat:@"%d", startEIndex].length;
    NSInteger startCLen = [NSString stringWithFormat:@"%d", startCIndex].length;
    NSInteger endELen = [NSString stringWithFormat:@"%d", endEIndex].length;
    NSInteger endCLen = [NSString stringWithFormat:@"%d", endCIndex].length;
    NSInteger maxELen = MAX(MAX(curELen, startELen), endELen);
    NSInteger maxCLen = MAX(MAX(curCLen, startCLen), endCLen);
    
    NSInteger pBits = powf(10, maxELen + maxCLen);
    NSInteger eBits = powf(10, maxCLen);
    NSInteger curValue = curPIndex * pBits + curEIndex * eBits + curCIndex;
    NSInteger startValue = startPIndex * pBits + startEIndex * eBits + startCIndex;
    NSInteger endValue = endPIndex * pBits + endEIndex * eBits + endCIndex;
    
    if (startValue <= curValue && curValue <= endValue) {
        return YES;
    } else {
        return NO;
    }
}

void FBReaderApp::changeBackgroundColor(int r, int g, int b) {
    BookTextView->setBackgroundColor(r, g, b);
    getViewWidget()->repaint();
}

void FBReaderApp::changeTextColor(int r, int g, int b) {
    BookTextView->setTextColor(r, g, b);
    getViewWidget()->repaint();
}

/*bool FBReaderApp::checkHightlighting(int x, int y)
{
    
}*/

bool FBReaderApp::setFontSize(int fontSize) {
    shared_ptr<ZLTextStyle> baseStyle = BookTextView->getTextStyleCollection()->getBaseStyle();
    ((ZLTextBaseStyle&)*baseStyle).FontSizeOption->setValue(fontSize);
    BookTextView->clearCaches();
    
    getViewWidget()->repaint();
    return true;
}

bool FBReaderApp::setFontName(std::string fontName) {
    shared_ptr<ZLTextStyle> baseStyle = BookTextView->getTextStyleCollection()->getBaseStyle();
    ((ZLTextBaseStyle&)*baseStyle).FontName = fontName;
    BookTextView->clearCaches();
    
    getViewWidget()->repaint();
    return true;
}

bool FBReaderApp::setLineSpace(int lineSpace) {
    shared_ptr<ZLTextStyle> baseStyle = BookTextView->getTextStyleCollection()->getBaseStyle();
    ((ZLTextBaseStyle&)*baseStyle).LineSpaceOption->setValue(lineSpace);
    BookTextView->clearCaches();
    
    getViewWidget()->repaint();
    return true;
}

bool FBReaderApp::hasPrevPage() {
    //return !BookTextView->getStartCursor()->isStartOfText();
    shared_ptr<ZLTextWordCursor> cursor = BookTextView->getStartCursor();
    bool res =  cursor != 0 && !cursor->isNull()&&!cursor->isStartOfText();
    return res;
}

bool FBReaderApp::hasNextPage() {
    return !BookTextView->getEndCursor()->isEndOfText();
}

long FBReaderApp::curPercent() {
    long percent = 0;
    shared_ptr<RationalNumber> xxx = BookTextView->getProgress();
    if (xxx->Denominator != 0) {
        percent = xxx->Numerator * 100 / xxx->Denominator;
    }
    return percent;
}

void FBReaderApp::jumpToPercent(long percent) {
    BookTextView->gotoProgress(percent);
    getViewWidget()->repaint();
}

shared_ptr<PagePosition> FBReaderApp::pagePosition() {
    return BookTextView->pagePosition();
}

#pragma mark - listening
void FBReaderApp::BeginRead() {
    myReadStart = BookTextView->getStartCursor();
}

bool FBReaderApp::NextParagraph(std::string* outWord) {
    //shared_ptr<ZLTextWordCursor> start = BookTextView->getStartCursor();
    //if(start != 0)
    {
        shared_ptr<ZLTextWordCursor>  cachedBegin = new ZLTextWordCursor(myReadStart);
        ParagraphTextSnippet xx(myReadStart,BookTextView->getEndCursor());
        
        bool res = xx.getParagraphText(outWord);
        myReadStart =  xx.getReadStart(); // move to end
        BookTextView->clearReadHightLighting();
        // refresh cachedBegin myReadStart
        ZLTextFixedPosition* fixedStart = new ZLTextFixedPosition(cachedBegin->getParagraphIndex(),
                                                                  cachedBegin->getElementIndex(),
                                                                  cachedBegin->getCharIndex());
        ZLTextFixedPosition* fixedEnd = new ZLTextFixedPosition(myReadStart->getParagraphIndex(),
                                                                myReadStart->getElementIndex(),
                                                                myReadStart->getCharIndex());
        BookTextView->appendReadHightLighting(new ZLSpeechHighlighting(BookTextView, fixedStart, fixedEnd));
//        BookTextView->appendReadHightLighting(new ZLTextManualHighlighting(BookTextView, fixedStart, fixedEnd));
        
       //printf(" res:%d, string:%s\n",res,(*outWord).c_str());
        //printf(" res:%d \n",res);
        getViewWidget()->repaint();
        return res;
    }
    //return true;
}

bool FBReaderApp::hasNextPageInReading(int curPageIndex) {
    return !(myReadStart->isLastParagraph());
}

void FBReaderApp::moveReadStartToParagraph(int pIndex) {
    myReadStart->moveToParagraph(pIndex);
}

void FBReaderApp::ExitRead() {
    BookTextView->clearReadHightLighting();
    getViewWidget()->repaint();
}

#pragma mark - search
ZDZLSearchResult* FBReaderApp::searchText(std::string text, long length, bool ignoreCase, bool wholeText, bool backward,
                                          bool thisSectionOnly) {
    int count = BookTextView->searchText(text, length, ignoreCase, wholeText, backward, thisSectionOnly);

    // 获取搜索到的所有marks
    std::vector<shared_ptr<ZDZLTextMark>> marks = BookTextView->getMarks();
    if (marks.size() <= 0) {
        return NULL;
    }

    ZDZLSearchResult* result = new ZDZLSearchResult();
    result->count = count;

    // 获取mark所在段落的文字内容
    for (std::vector<shared_ptr<ZDZLTextMark>>::iterator it = marks.begin(); it != marks.end(); it++) {
        shared_ptr<ZDZLTextMark> mark = *it;
        int pIndex = mark->ParagraphIndex;
        
        if (!result->isParagraphExisted(pIndex)) {
            std::string paragraphText = ((FBView*)(BookTextView.getT()))->textInParagraph(pIndex);
            result->addParagraph(pIndex, paragraphText);
        }
    }

    return result;
}

#pragma mark - selecting
shared_ptr<ZLColor> FBReaderApp::selectingBgColor() {
    return getViewWidget()->selectingBgColor();
}

shared_ptr<ZLColor> FBReaderApp::selectingCursorColor() {
    return getViewWidget()->selectingCursorColor();
}

#pragma mark - lined text
shared_ptr<ZLColor> FBReaderApp::underlineColor(shared_ptr<ZLTextPosition> startPos,
                                                shared_ptr<ZLTextPosition> endPos) {
    return getViewWidget()->underlineColor(startPos->getParagraphIndex(), startPos->getElementIndex(), startPos->getCharIndex(),
                                           endPos->getParagraphIndex(), endPos->getElementIndex(), endPos->getCharIndex());
}

void FBReaderApp::addUnderline(BookNote note) {
    ZLTextFixedPosition* fixedStart = new ZLTextFixedPosition(note.ParagraphIndex, note.ElementIndex, note.CharIndex);
    ZLTextFixedPosition* fixedEnd = new ZLTextFixedPosition(note.ParagraphEndIndex, note.ElementEndIndex, note.CharEndIndex);
    BookTextView->appendHightLighting(new ZLUnderlineHighlighting(BookTextView, fixedStart, fixedEnd));
//    BookTextView->appendHightLighting(new ZLTextManualHighlighting(BookTextView, fixedStart, fixedEnd));
}

void FBReaderApp::removeUnderline(BookNote note) {
    ZLTextFixedPosition* fixedStart = new ZLTextFixedPosition(note.ParagraphIndex, note.ElementIndex, note.CharIndex);
    ZLTextFixedPosition* fixedEnd = new ZLTextFixedPosition(note.ParagraphEndIndex, note.ElementEndIndex, note.CharEndIndex);
    BookTextView->removeHightLighting(new ZLUnderlineHighlighting(BookTextView, fixedStart, fixedEnd));
    //    BookTextView->removeHightLighting(new ZLTextManualHighlighting(BookTextView, fixedStart, fixedEnd));
}

#pragma mark - note
int FBReaderApp::getNoteCount(shared_ptr<ZLTextPosition> start, shared_ptr<ZLTextPosition> end) {
    return getViewWidget()->countNote(start->getParagraphIndex(), start->getElementIndex(), start->getCharIndex(),
                                      end->getParagraphIndex(), end->getElementIndex(), end->getCharIndex());
}

#pragma mark - opinion
void FBReaderApp::addOpinion(BookNote opinion) {
    ZLTextFixedPosition* fixedStart = new ZLTextFixedPosition(opinion.ParagraphIndex, opinion.ElementIndex, opinion.CharIndex);
    ZLTextFixedPosition* fixedEnd = new ZLTextFixedPosition(opinion.ParagraphEndIndex, opinion.ElementEndIndex, opinion.CharEndIndex);
    BookTextView->appendHightLighting(new ZLOpinionHighlighting(BookTextView, fixedStart, fixedEnd));
}

void FBReaderApp::removeOpinion(BookNote opinion) {
    ZLTextFixedPosition* fixedStart = new ZLTextFixedPosition(opinion.ParagraphIndex, opinion.ElementIndex, opinion.CharIndex);
    ZLTextFixedPosition* fixedEnd = new ZLTextFixedPosition(opinion.ParagraphEndIndex, opinion.ElementEndIndex, opinion.CharEndIndex);
    BookTextView->removeHightLighting(new ZLOpinionHighlighting(BookTextView, fixedStart, fixedEnd));
}

int FBReaderApp::getOpinionCount(int pIndex) {
    return getViewWidget()->countOpinionInParagraph(pIndex);
}

