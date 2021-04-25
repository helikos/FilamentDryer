#ifndef _MAIN_MENU_H
#define _MAIN_MENU_H

#include <menu.h>

void encoderTimer();
result systemExit();
result MyIdleOn(menuOut& o,idleEvent e);

//customizing a menu prompt look
class confirmExit:public menu {
public:
  confirmExit(constMEM menuNodeShadow& shadow):menu(shadow) {}
  Used printTo(navRoot &root,bool sel,menuOut& out, idx_t idx,idx_t len,idx_t p) override {
    return idx<0?//idx will be -1 when printing a menu title or a valid index when printing as option
      menu::printTo(root,sel,out,idx,len,p)://when printing title
      out.printRaw((constText*)F("Exit"), len);//when printing as regular option
  }
};

class confirmSave:public menu {
public:
  confirmSave(constMEM menuNodeShadow& shadow):menu(shadow) {}
  Used printTo(navRoot &root,bool sel,menuOut& out, idx_t idx,idx_t len,idx_t p) override {
    return idx<0?//idx will be -1 when printing a menu title or a valid index when printing as option
      menu::printTo(root,sel,out,idx,len,p)://when printing title
      out.printRaw((constText*)F("Save"), len);//when printing as regular option
  }
};

class confirmRestoreDefault : public menu {
public:
  confirmRestoreDefault(constMEM menuNodeShadow& shadow):menu(shadow) {}
  Used printTo(navRoot &root,bool sel,menuOut& out, idx_t idx,idx_t len,idx_t p) override {
    return idx<0?//idx will be -1 when printing a menu title or a valid index when printing as option
      menu::printTo(root,sel,out,idx,len,p)://when printing title
      out.printRaw((constText*)F("Restore Default"), len);//when printing as regular option
  }
};

#endif
