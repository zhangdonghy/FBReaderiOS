//
//  ZLTextStyleCollection.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/9/3.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLTextStyleCollection_hpp
#define ZLTextStyleCollection_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include "ZLTextBaseStyle.hpp"

class ZLTextNGStyleDescription;
class ZLTextStyleCollection
{
    private:
    //ZLTextBaseStyle*  myBaseStyle;
    shared_ptr<ZLTextStyle> myBaseStyle;
    std::string myDefaultStyleCSS;
     std::string myDefaultStyleXML;
    std::vector<ZLTextNGStyleDescription*> myDescriptionList;
    ZLTextNGStyleDescription* myDescriptionMap[256];// 256 pointer;
public:
    std::string Screen;
    ZLTextStyleCollection(std::string screen) ;//{
       // Screen = screen;
        /*Map<Integer,ZLTextNGStyleDescription> descriptions =
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
                             );*/
   // }
     shared_ptr<ZLTextStyle> getBaseStyle() {
        return myBaseStyle;
    }
    std::vector<ZLTextNGStyleDescription*> getDescriptionList(){
        return myDescriptionList;
    }
    ZLTextNGStyleDescription* getDescription(uint8_t kind){
        return myDescriptionMap[kind&0xff];
    }
};
#endif /* ZLTextStyleCollection_hpp */
