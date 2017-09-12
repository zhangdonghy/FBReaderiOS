//
//  Config.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/14.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "Config.hpp"


Config* Config::ourInstance = 0;
Config*
Config::Instance()
{
    return  ourInstance;
}
std::string Config::myNullString = "__NULL__";


std::string
Config::getValue(StringPair* id,std::string defaultValue)
{
    std::string s = "";//myCache[id];
    //std::map<int,shared_ptr<ZLTextParagraphCursor>>::const_iterator it=lruCache.begin()
    std::map<StringPair*, std::string>::const_iterator it = myCache.find(id);
    if(it == myCache.end()) {
        if(myCacheGroups.find(id->Group) != myCacheGroups.end()) {
            s = myNullString;
        } else {
            return  defaultValue;
        }
        myCache.insert(std::make_pair(id,s));
        /*if(myCacheGroups.){
            
        }*/
        
    } else {
        s =  it->second;
        if(s == myNullString){
            return  defaultValue;
        }
        return  s;
    }
    return s;
    /*
     String value = myCache.get(id);
     if (value == null) {
     if (myCachedGroups.contains(id.Group)) {
     value = myNullString;
     } else {
     try {
					value = getValueInternal(id.Group, id.Name);
     } catch (NotAvailableException e) {
					return defaultValue;
     }
     if (value == null) {
					value = myNullString;
     }
     }
     myCache.put(id, value);
     }
     return value != myNullString ? value : defaultValue;
     */
}
void
Config::setValue(StringPair*id,std::string value)
{
    std::map<StringPair*, std::string>::const_iterator it = myCache.find(id);
    if(it != myCache.end()) {
        std::string s = it->second;
        if (s == value) {
            return;
        } else {
            myCache.erase(it);
        }
    }
    myCache.insert(std::make_pair(id,value));
//setValueInternal(id.Group, id.Name, value);
}
void
Config::unsetValue(StringPair* id) {
    //myCache.put(id, myNullString);
    myCache.erase(id);
    myCache.insert(std::make_pair(id,myNullString));
   // unsetValueInternal(id.Group, id.Name);
}
void
Config::setToCache(std::string group, std::string name, std::string value) {
   // myCache.put(new StringPair(group, name), value != null ? value : myNullString);
    myCache.insert(std::make_pair(new StringPair(group, name),value));
    
}
void
Config::requestAllValuesForGroup(std::string group)
{
    if(myCacheGroups.find(group) != myCacheGroups.end()) {
        return;
    }
    myCacheGroups.insert(myCacheGroups.end(),group);
    /*synchronized (myCachedGroups)
    {
        if (myCachedGroups.contains(group)) {
            return;
        }
        final Map<String,String> values;
        try {
            values = requestAllValuesForGroupInternal(group);
        } catch (NotAvailableException e) {
            return;
        }
        for (Map.Entry<String,String> entry : values.entrySet()) {
            setToCache(group, entry.getKey(), entry.getValue());
        }
        myCachedGroups.add(group);
    }
     */
    
}
