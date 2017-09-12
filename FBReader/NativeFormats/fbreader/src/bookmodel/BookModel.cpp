/*
 * Copyright (C) 2004-2015 FBReader.ORG Limited <contact@fbreader.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

//#include <AndroidUtil.h>

#include "ZLImage.h"
#include "ZLFile.h"

#include "BookModel.h"
#include "BookReader.h"

#include "../formats/FormatPlugin.h"
#include "../library/Book.h"
#include "ZDBookModel.hpp"

// javaModel 传递下来仅仅是为了增加图片用的？？no no no
// 当然不是在，JavaNativeFormatPlugin。cpp的readModel里面可以看到详细过程

BookModel::BookModel(const shared_ptr<Book> book, /*jobject javaModel,*/ const std::string &cacheDir) : CacheDir(cacheDir), myBook(book) {
	//myJavaModel = AndroidUtil::getEnv()->NewGlobalRef(javaModel);

	myBookTextModel = new ZLTextPlainModel(std::string(), book->language(), 131072, CacheDir, "ncache", myFontManager);
	myContentsTree = new ContentsTree();
    //myJavaModel ＝ new ZDBookModel();
	/*shared_ptr<FormatPlugin> plugin = PluginCollection::Instance().plugin(book->file(), false);
	if (!plugin.isNull()) {
		plugin->readModel(*this);
	}*/
}

BookModel::~BookModel() {
	//AndroidUtil::getEnv()->DeleteGlobalRef(myJavaModel);
}

void BookModel::setHyperlinkMatcher(shared_ptr<HyperlinkMatcher> matcher) {
	myHyperlinkMatcher = matcher;
}

BookModel::Label BookModel::label(const std::string &id) const {
	if (!myHyperlinkMatcher.isNull()) {
		return myHyperlinkMatcher->match(myInternalHyperlinks, id);
	}

	std::map<std::string,Label>::const_iterator it = myInternalHyperlinks.find(id);
	return (it != myInternalHyperlinks.end()) ? it->second : Label(0, -1);
}

const shared_ptr<Book> BookModel::book() const {
	return myBook;
}

bool BookModel::flush() {
	myBookTextModel->flush();
	if (myBookTextModel->allocator().failed()) {
		return false;
	}

	std::map<std::string,shared_ptr<ZLTextModel> >::const_iterator it = myFootnotes.begin();
	for (; it != myFootnotes.end(); ++it) {
		it->second->flush();
		if (it->second->allocator().failed()) {
			return false;
		}
	}
	return true;
}
