//
//  ZLApplication.hpp
//  ZDStudent
//
//  Created by zhangdong on 16/8/31.
//  Copyright © 2016年 Eagley. All rights reserved.
//

#ifndef ZLApplication_hpp
#define ZLApplication_hpp

#include "shared_ptr.h"
//
#include <string>
#include <map>
class ZLView;
class ZLApplicationWindow;
class ZLViewWidget;

class ZLAction
{
    public:
    virtual void run(void* data)=0; // TODO: zhangdong
};
class ZLApplication
{
private:
    static ZLApplication* ourInstance;
    shared_ptr<ZLView> myView;
    shared_ptr<ZLApplicationWindow> myWindow;
    std::map<std::string,shared_ptr<ZLAction>> myIdToActionMap;
protected:
    ZLApplication();
    void setView(shared_ptr<ZLView> view);
public:
    static ZLApplication* Instance();
    shared_ptr<ZLView> getCurrentView() ;
    void runAction(std::string actionId,void* data);
    void setWindow(shared_ptr<ZLApplicationWindow> window);
    void initWindow();
    shared_ptr<ZLViewWidget> getViewWidget();
    
    void addAction(std::string actionId, shared_ptr<ZLAction> action);
};
inline shared_ptr<ZLView> ZLApplication::getCurrentView() {
    return myView;
}
#endif /* ZLApplication_hpp */
