//
//  ZLTextStyleCollection.cpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/3.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#include "ZLTextStyleCollection.hpp"
#include "ZLFile.h"
#include "IOSUtil.h"
#include "ZLInputStream.h"
#include "SimpleCSSReader.hpp"
#include "ZLTextNGStyleDescription.hpp"

ZLTextStyleCollection::ZLTextStyleCollection(std::string screen){
     Screen = screen;
    // zhangdong, not read from file  just reset
    
    //myBaseStyle = new ZLTextBaseStyle(Screen,"Asia",20);
    myDefaultStyleCSS = IOSUtil::GetContentForResource("styles", "css");
    myDefaultStyleXML = IOSUtil::GetContentForResource("styles", "xml");
    
    std::string csspath = IOSUtil::GetFullPathForResource("styles", "", "css");
    SimpleCSSReader* reader = new SimpleCSSReader();
    reader->read(csspath);
    myDescriptionList = reader->getDescriptionList();
    std::map<int,ZLTextNGStyleDescription*>  map = reader->getDescriptionMap();
    
    for(std::map<int,ZLTextNGStyleDescription*>::const_iterator it=map.begin();
        it!=map.end();++it)
    {
        ZLTextNGStyleDescription* item = it->second;
        int index = it->first;
        myDescriptionMap[index&0xff] = item;
    }
    if(reader!=0){
        delete  reader;
    }
    // default styles.xml just has short value, give default
    /*
     <?xml version="1.0" encoding="UTF-8"?>
     <DefaultStyles>
     <base screen="Base" family="sans-serif" fontSize="18"/>
     <base screen="Yota" family="PT Sans" fontSize="10"/>
     </DefaultStyles>
     */
    myBaseStyle = new ZLTextBaseStyle("Base","sans-serif",18);
    
    
    // ZLFile file(path,"css");
    //shared_ptr<ZLInputStream> xx = file.inputStream();
    
    /*
     Screen = screen;
     final Map<Integer,ZLTextNGStyleDescription> descriptions =
     new SimpleCSSReader().read(ZLResourceFile.createResourceFile("default/styles.css"));
     myDescriptionList = Collections.unmodifiableList(
     new ArrayList<ZLTextNGStyleDescription>(descriptions.values())
     );
     for (Map.Entry<Integer,ZLTextNGStyleDescription> entry : descriptions.entrySet()) {
     myDescriptionMap[entry.getKey() & 0xFF] = entry.getValue();
     }
     XmlUtil.parseQuietly(
     ZLResourceFile.createResourceFile("default/styles.xml"),
     new TextStyleReader()
     );
     */
    
}