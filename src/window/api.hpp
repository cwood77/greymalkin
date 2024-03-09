#ifndef ___window_api___
#define ___window_api___

#include "../cui/api.hpp"
#include <cstddef>
#include <string>

namespace window {

class message {
public:
   explicit message(const std::string& key, bool broadcast = false)
   : key(key), broadcast(broadcast), iResult(0), iResult2(0), handled(false), pPtr(NULL) {}
   std::string key;
   bool broadcast;
   int iResult;
   int iResult2;
   std::string sResult;
   bool handled;
   void *pPtr;
};

class iCommandProvider {
public:
   virtual ~iCommandProvider() {}
   virtual void provide(bool active, cui::keyMap& m) = 0;
   virtual void handleMessage(bool active, message& m) = 0;
};

class iCanvas {
public:
   virtual void backlight(size_t x, size_t y, cui::bgcol::type bg) = 0;
   virtual void drawInto(cui::iPort& p, size_t x, size_t y, size_t w, size_t h) = 0;
};

class iWindow;

// writes entire model to the (v)port everytime!
// owned by windows
class iContent : public iCommandProvider {
public:
   virtual void addBinding(iWindow& wnd) = 0;
   virtual void rmBinding(iWindow& wnd) = 0;
   virtual iCanvas& redraw(iWindow& wnd, std::string& title) = 0;
};

class iCursor : public iCommandProvider {
public:
   virtual void redraw(bool active, iCanvas& c) = 0;
};

// windows can find each other, know their dims and topRow (scrolling)
class iWindow {
public:
   virtual ~iWindow() {}

   virtual void provide(cui::keyMap& m) = 0;
   virtual void handleMessage(message& m) = 0;

   virtual iCursor& getCursor() = 0;

   virtual void bind(iContent& b) = 0;

   virtual void onActivate(bool active) = 0;
   virtual void draw() = 0;
};

// collection of windows such that each addressable spot belongs to a window
class iLayout {
public:
   virtual ~iLayout() {}

   virtual void provide(cui::keyDispatcher& d) = 0;
   virtual void handleMessage(message& m) = 0;

   virtual iWindow& getIth(size_t i) = 0;

   virtual void draw() = 0;
};

class iManager {
public:
   virtual ~iManager() {}
   virtual iLayout& create(cui::screenBuffer& screen) = 0;
};

} // namespace window

#endif // ___window_api___
