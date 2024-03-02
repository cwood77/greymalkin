#ifndef ___window_api___
#define ___window_api___

#include <cstddef>
#include <string>

namespace cui { class iPort; }
namespace cui { class keyMap; }
namespace cui { class screenBuffer; }

namespace window {

class iCommandProvider {
public:
   virtual ~iCommandProvider() {}
   virtual void provide(cui::keyMap& m) = 0;
};

class iCanvas {
public:
   virtual void drawInto(cui::iPort& p, size_t x, size_t y, size_t w, size_t h) = 0;
};

class iWindow;

// writes entire model to the (v)port everytime!
// owned by windows
class iContent : public iCommandProvider {
public:
   virtual void addBinding(iWindow& wnd) = 0;
   virtual void rmBinding(iWindow& wnd) = 0;
   virtual iCanvas& getCanvas() = 0;
};

class iContentExpert {
public:
   virtual ~iContentExpert() {}
   virtual bool supports(const std::string& text) const = 0;
   virtual iContent& create(const std::string& text) const = 0;
};

class iContentManager {
public:
   virtual ~iContentManager() {}
   virtual iContent& create(const std::string& text) = 0;
};

// windows can find each other, know their dims and topRow (scrolling)
class iWindow : public iCommandProvider {
public:
   virtual void onActivate(bool active) = 0;
   virtual void bind(iContent& b) = 0;
   virtual void draw() = 0;
};

// collection of windows such that each addressable spot belongs to a window
class iLayout : public iCommandProvider {
public:
   virtual ~iLayout() {}
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
