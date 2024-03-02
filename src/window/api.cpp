#include "../cui/api.hpp"
#include "../tcatlib/api.hpp"
#include "api.hpp"
#include <memory>
#include <set>

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

   virtual void handleMessage(message& m) {}

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

class window;

class iLayoutInternal {
public:
   virtual void closeWindow(window& w) = 0;
};

class window : public iWindow {
public:
   window(iLayoutInternal& l, cui::iPort& p, size_t w, size_t h)
   : m_layout(l), m_port(p), m_w(w), m_h(h), m_pContent(NULL)
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
      m.map(cui::keystroke::esc(),[&](auto&){ m_layout.closeWindow(*this); });
      m_pCursor->provide(m);
      m_pContent->provide(m);
   }

   virtual void handleMessage(message& m)
   {
      m_pCursor->handleMessage(m);
      if(m.handled)
         return;
      m_pContent->handleMessage(m);
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
   iLayoutInternal& m_layout;
   cui::iPort& m_port;
   size_t m_w;
   size_t m_h;
   std::unique_ptr<iCursor> m_pCursor;
   iContent *m_pContent;
};

class layout : public iLayout, private iLayoutInternal {
public:
   explicit layout(cui::screenBuffer& screen)
   : m_screen(screen)
   , m_pActive(NULL)
   , m_pKd(NULL)
   {
      m_wnds.insert(
         new window(
            *this,
            m_screen.createPort(0,0,screen.getWidth(),screen.getHeight()),
            screen.getWidth(),
            screen.getHeight()));
      m_pActive = &getIth(0);
      m_pActive->onActivate(true);
   }

   virtual iWindow& getIth(size_t i)
   {
      return **m_wnds.begin();
   }

   virtual void draw()
   {
      for(auto *pWnd : m_wnds)
         pWnd->draw();
   }

   virtual void provide(cui::keyDispatcher& d)
   {
      m_pKd = &d;

      auto *pMap = new cui::keyMap();
      m_pKd->push(*pMap);

      for(auto *pWnd : m_wnds)
         pWnd->provide(*pMap);
   }

   virtual void handleMessage(message& m)
   {
      if(m.broadcast)
      {
         for(auto *pWnd : m_wnds)
            if(!m.handled)
               pWnd->handleMessage(m);
      }
      else
         m_pActive->handleMessage(m);
   }

private:
   virtual void closeWindow(window& w)
   {
      m_pKd->pop();

      m_wnds.erase(&w);
      delete &w;

      if(m_wnds.size())
         provide(*m_pKd);
   }

   cui::screenBuffer& m_screen;
   std::set<window*> m_wnds;
   iWindow *m_pActive;
   cui::keyDispatcher *m_pKd;
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
