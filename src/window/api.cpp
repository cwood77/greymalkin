#include "../cmn/service.hpp"
#include "../cui/api.hpp"
#include "../tcatlib/api.hpp"
#include "api.hpp"
#include <memory>
#include <set>

namespace window {
namespace {

class iCursorDest {
public:
   virtual void cursorCoordToAbsCoord(size_t& x, size_t& y) = 0;
   virtual void onCursorMoved() = 0;
};

class pntCursor : public iCursor {
public:
   explicit pntCursor(iCursorDest& owner) : x(0), y(0), m_wnd(owner) {}

   virtual void provide(bool active, cui::keyMap& m)
   {
      if(active)
      {
         m.map(cui::keystroke('j'),[&](auto& d){ y++; m_wnd.onCursorMoved(); });
         m.map(cui::keystroke('k'),[&](auto& d){ y--; m_wnd.onCursorMoved(); });
         m.map(cui::keystroke('l'),[&](auto& d){ x++; m_wnd.onCursorMoved(); });
         m.map(cui::keystroke('h'),[&](auto& d){ x--; m_wnd.onCursorMoved(); });

         m.map(cui::keystroke('i'),[&](auto& d)
         {
            window::message m("beginInput");
            size_t _x = x;
            size_t _y = y;
            m_wnd.cursorCoordToAbsCoord(_x,_y);
            m.iResult = _x;
            m.iResult2 = _y;
            tcat::typePtr<cmn::serviceManager> svcMan;
            auto& l = svcMan->demand<window::iLayout>();
            l.handleMessage(m);
            x = m.iResult;
            y = m.iResult2;
            m_wnd.onCursorMoved();
         });
      }
   }

   virtual void handleMessage(bool active, message& m)
   {
      if(active && m.key == "moveCursor" && !m.handled)
      {
         if(m.iResult)
         {
            y += m.iResult;
            x = 0;
         }
         else
         {
            x += m.iResult2;
         }
         m.handled = true;
         m_wnd.onCursorMoved();
      }
   }

   virtual void redraw(bool active, cui::iPort& p)
   {
      if(active)
      {
         cui::autoColor<cui::bgcol::type> _c(p,cui::bgcol::kBrightBlue);
         p << cui::relLoc(x,y);
         p.writeTruncate(" ");
      }
   }

   size_t x;
   size_t y;

private:
   iCursorDest& m_wnd;
};

class window;

class iLayoutInternal {
public:
   virtual void closeWindow(window& w) = 0;
};

class window : public iWindow, private iCursorDest {
public:
   window(iLayoutInternal& l, cui::iPort& p, size_t w, size_t h)
   : m_layout(l), m_pOPort(NULL), m_pIPort(NULL), m_w(0), m_h(0)
   , m_pContent(NULL), m_active(false)
   {
      resize(p,w,h);
      m_pCursor.reset(new pntCursor(*this));
   }

   ~window()
   {
      if(m_pContent)
         m_pContent->rmBinding(*this);
      delete m_pOPort;
      delete m_pIPort;
   }

   virtual void provide(cui::keyMap& m)
   {
      m.map(cui::keystroke::esc(),[&](auto&){ m_layout.closeWindow(*this); });
      m_pCursor->provide(m_active,m);
      m_pContent->provide(m_active,m);
   }

   virtual void handleMessage(message& m)
   {
      m_pCursor->handleMessage(m_active,m);
      if(m.handled)
         return;
      m_pContent->handleMessage(m_active,m);
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
      bool changed = (m_active != active);
      if(!changed)
         return;

      m_active = active;
      draw();
   }

   virtual void draw()
   {
      if(!m_pContent)
         return;

      std::string title;
      auto& canvas = m_pContent->redraw(*this,title);
      m_pCursor->redraw(m_active,canvas.annotate());
      drawBox(*m_pOPort,title);
      canvas.drawInto(*m_pIPort,0,0,m_w-1,m_h-1);
   }

   void resize(cui::iPort& p, size_t w, size_t h)
   {
      if(m_pOPort)
         delete m_pOPort;
      if(m_pIPort)
         delete m_pIPort;
      m_pOPort = &p;
      m_w = w;
      m_h = h;
      m_pIPort = &p.createSubPort(1,1,w-1,h-1);
   }

private:
   virtual void cursorCoordToAbsCoord(size_t& x, size_t& y) {}

   virtual void onCursorMoved() { draw(); }

   void drawBox(cui::iPort& p, const std::string& title)
   {
      cui::autoColor<cui::bgcol::type> _c(p,cui::bgcol::kBrightBlack,m_active);
      p << cui::relLoc(0,0);
      p.writeTruncate("+--");
      p.writeTruncate(title);
      p.writeTruncate(std::string(m_w-3-title.length(),'-'));
      for(size_t y=1;y<m_h;y++)
      {
         p << cui::relLoc(0,y);
         p.writeTruncate("|");
      }
   }

   iLayoutInternal& m_layout;
   cui::iPort *m_pOPort;
   cui::iPort *m_pIPort;
   size_t m_w;
   size_t m_h;
   std::unique_ptr<iCursor> m_pCursor;
   iContent *m_pContent;
   bool m_active;
};

class layout : public iLayout, private iLayoutInternal {
public:
   explicit layout(cui::screenBuffer& screen)
   : m_screen(screen)
   , m_pActive(NULL)
   , m_pKd(NULL)
   {
      // H = 10
      // 0 C++
      // 1 C++
      // 2 C++
      // 3 C++
      // 4 C++
      // 5 C++
      // 6 C++
      // 7 status
      // 8 status
      // 9 status

      m_wnds.push_back(
         new window(
            *this,
            m_screen.createPort(0,screen.getHeight()-3,screen.getWidth(),3),
            screen.getWidth(),
            3));
      m_wnds.push_back(
         new window(
            *this,
            m_screen.createPort(0,0,screen.getWidth(),screen.getHeight()-3),
            screen.getWidth(),
            screen.getHeight()-3));

      m_pActive = &getIth(1);
      m_pActive->onActivate(true);
   }

   virtual iWindow& getIth(size_t i)
   {
      return *m_wnds[i];
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

      for(auto it=m_wnds.begin();it!=m_wnds.end();++it)
      {
         if(*it == &w)
         {
            m_wnds.erase(it);
            break;
         }
      }

      delete &w;

      if(m_wnds.size() > 1)
         provide(*m_pKd);
   }

   cui::screenBuffer& m_screen;
   std::vector<window*> m_wnds;
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
