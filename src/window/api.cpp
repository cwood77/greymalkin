#include "../cui/api.hpp"
#include "../tcatlib/api.hpp"
#include "api.hpp"
#include <memory>

namespace window {
namespace {

class pntCursor : public iCursor {
public:
   explicit pntCursor(iWindow& owner) : x(0), y(0), m_wnd(owner) {}

   virtual void provide(cui::keyMap& m)
   {
      m.map(cui::keystroke('j'),[&](auto& d){ y++; m_wnd.draw(); });
      m.map(cui::keystroke('k'),[&](auto& d){ y--; m_wnd.draw(); });
      m.map(cui::keystroke('l'),[&](auto& d){ x++; m_wnd.draw(); });
      m.map(cui::keystroke('h'),[&](auto& d){ x++; m_wnd.draw(); });
   }

   virtual void redraw(cui::iPort& p)
   {
      cui::autoColor<cui::bgcol::type> _c(p,cui::bgcol::kBrightBlue);
      p << cui::relLoc(x,y);
      p.writeTruncate(" ");
   }

   size_t x;
   size_t y;

private:
   iWindow& m_wnd;
};

class window : public iWindow {
public:
   window(cui::iPort& p, size_t w, size_t h)
   : m_port(p), m_w(w), m_h(h), m_pContent(NULL)
   {
      m_pCursor.reset(new pntCursor(*this));
   }

   ~window()
   {
      if(m_pContent)
         m_pContent->rmBinding(*this);
   }

   virtual void provide(cui::keyMap& m)
   {
      m_pCursor->provide(m);
      m_pContent->provide(m);
   }

   virtual iCursor& getCursor() { return *m_pCursor; }

   virtual void bind(iContent& b)
   {
      if(m_pContent)
         m_pContent->rmBinding(*this);
      m_pContent = &b;
      m_pContent->addBinding(*this);
   }

   virtual void onActivate(bool active)
   {
   }

   virtual void draw()
   {
      auto& canvas = m_pContent->redraw(*this);
      m_pCursor->redraw(canvas.annotate());
      canvas.drawInto(m_port,0,0,m_w,m_h);
   }

private:
   cui::iPort& m_port;
   size_t m_w;
   size_t m_h;
   std::unique_ptr<iCursor> m_pCursor;
   iContent *m_pContent;
};

class layout : public iLayout {
public:
   explicit layout(cui::screenBuffer& screen)
   : m_screen(screen)
   , m_wnd(m_screen.createPort(0,0,screen.getWidth(),screen.getHeight()),screen.getWidth(),screen.getHeight())
   {
      m_wnd.onActivate(true);
   }

   virtual void provide(cui::keyMap& m)
   {
      m_wnd.provide(m);
   }

   virtual iWindow& getIth(size_t i)
   {
      return m_wnd;
   }

   virtual void draw()
   {
      m_wnd.draw();
   }

private:
   cui::screenBuffer& m_screen;
   window m_wnd;
};

class manager : public iManager {
public:
   virtual iLayout& create(cui::screenBuffer& screen)
   {
      return *new layout(screen);
   }
};

tcatExposeTypeAs(manager,iManager);

} // anonymous namespace
} // namespace window
