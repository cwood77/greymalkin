#include "../cmn/service.hpp"
#include "../shared/context.hpp"
#include "../tcatlib/api.hpp"
#include "api.hpp"
#include "vPort-i.hpp"

namespace window {
namespace {

class window : public iWindow {
public:
   window(cui::iPort& p, size_t w, size_t h)
   : m_port(p), m_w(w), m_h(h) {}

   virtual void bind(iModelPainter& b)
   {
      m_pPainter = &b;

      tcat::typePtr<cmn::serviceManager> svcMan;
      svcMan->demand<shared::context>().pW = this;
   }

   virtual void invalidate()
   {
      impl::virtualPort vp;
      m_pPainter->draw(vp);
      vp.draw(m_port,0,0,m_w,m_h);
   }

private:
   cui::iPort& m_port;
   size_t m_w;
   size_t m_h;
   iModelPainter *m_pPainter;
};

class layout : public iLayout {
public:
   explicit layout(cui::screenBuffer& screen)
   : m_screen(screen)
   , m_wnd(m_screen.createPort(0,0,screen.getWidth(),screen.getHeight()),screen.getWidth(),screen.getHeight())
   {
   }

   virtual iWindow& getIth(size_t i)
   {
      return m_wnd;
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
