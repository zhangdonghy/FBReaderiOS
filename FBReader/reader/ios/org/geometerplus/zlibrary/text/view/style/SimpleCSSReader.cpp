//
//  SimpleCSSReader.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/13.
//  Copyright © 2016年 Eagley. All rights reserved.
//
#include <stdlib.h>
#include "SimpleCSSReader.hpp"
#include "ZLFile.h"
#include "ZLInputStream.h"
#include "IOSUtil.h"
#include "ZLTextNGStyleDescription.hpp"

SimpleCSSReader::SimpleCSSReader()
{
    myState = EXPECT_SELECTOR;
    mySavedState = EXPECT_SELECTOR;
    mySelector = "";
    myName = "";
    myCurrentMap = 0;
}
void readLine(ZLInputStream& stream,std::string& buffer){
    buffer.clear();
    char ch;
    while(1) {
        if(stream.read(&ch,1) != 1) {
            return;
        }
        if(ch == 10 || ch == 13){
            if(!(buffer.size()==0)) {
                return;
            }
        } else {
            buffer += ch;
        }
    }
}
void
SimpleCSSReader::processToken(std::string token)
{
    const char* data = token.c_str();
    if (myState != READ_COMMENT &&
//        token.startsWith("/*")
        strncmp(token.c_str(), "/*", 2)==0
        ) {
        mySavedState = myState;
        myState = READ_COMMENT;
        return;
    }
    
    switch (myState) {
        case READ_COMMENT:
            if (//token.endsWith("*/")
                token.size()>=2 &&
                data[token.size()-2] == '*' &&
                data[token.size()-1] == '/'
                ) {
                myState = mySavedState;
            }
            break;
        case EXPECT_SELECTOR:
            mySelector = token;
            myState = EXPECT_OPEN_BRACKET;
            break;
        case EXPECT_OPEN_BRACKET:
            if (
                //"{".equals(token)
                token.size()==1 &&
                data[0] == '{'
                ) {
               // myCurrentMap = new HashMap<String,String>();
                myCurrentMap = new std::map<std::string,std::string>();
                myState = EXPECT_NAME;
            }
            break;
        case EXPECT_NAME:
            if (
                //"}".equals(token)
                token.size()==1 &&
                data[0] == '}'
                ) {
                if (//mySelector != null
                    !mySelector.empty()
                    ) {
                    int value =0;
                    std::string v = myCurrentMap[0]["fbreader-id"];
                    value = atoi(v.c_str());
                        myDescriptionMap.insert(std::make_pair(value,
                                  new ZLTextNGStyleDescription(mySelector, myCurrentMap)));
                    //try {
                       // myDescriptionMap.put(
                         //                    Integer.valueOf(myCurrentMap.get("fbreader-id")),
                              //               new ZLTextNGStyleDescription(mySelector, myCurrentMap)
                                     //        );
                   // } catch (Exception e) {
                        // ignore
                   // }
                }
                myState = EXPECT_SELECTOR;
            } else {
                myName = token;
                myState = EXPECT_VALUE;
            }
            break;
        case EXPECT_VALUE:
            if (
                myCurrentMap != 0
                &&
                //myName != null
                !myName.empty()
                ) {
               // myCurrentMap.put(myName, token);
                myCurrentMap->insert(std::make_pair(myName,
                                                    token));
            }
            myState = EXPECT_NAME;
            break;
    }
}
void
SimpleCSSReader::read(std::string path)
{
    ZLFile f(path,"css");
    
    shared_ptr<ZLInputStream> inuput = f.inputStream();
    if(inuput->open())
    {
        std::string res="";
        while(true) {
            readLine(*inuput,res);
            if(res.empty()) {
                break;
            }
            //res = "13124435544646abcd13124435544646bbbb13124435544646";
            std::vector<std::string> list = IOSUtil::EvaluateString(res);
            for(int i=0;i<list.size();i++){
                processToken(list[i]);
            }
            /*for (String token : MiscUtil.smartSplit(line)) {
                processToken(token);
            }*/
        }
        inuput->close();
    }
    
}
std::vector<ZLTextNGStyleDescription*>
SimpleCSSReader::getDescriptionList()
{
    
    std::vector<ZLTextNGStyleDescription*> ret;
    for(std::map<int,ZLTextNGStyleDescription*>::const_iterator it=myDescriptionMap.begin();
        it!=myDescriptionMap.end();++it)
    {
        ZLTextNGStyleDescription* item = it->second;
        ret.insert(ret.end(), item);
    }
    return ret;
}


