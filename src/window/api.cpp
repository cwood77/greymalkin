#include "../cmn/error.hpp"
#include "../cui/api.hpp"
#include "../tcatlib/api.hpp"
#include "api.hpp"

namespace window {
namespace {

class contentManager : public iContentManager {
public:
   virtual iContent& create(const std::string& text)
   {
      return m_pExperts[findExpert(text)]->create(text);
   }

private:
   size_t findExpert(const std::string& text)
   {
      int rval = -1;

      for(size_t i=0;i<m_pExperts.size();i++)
      {
         if(m_pExperts[i]->supports(text))
         {
            if(rval == -1)
               rval = i;
            else
               cmn::error(cdwHere,"multiple experts support file type!")
                  .with("pattern",text)
                  .with("# experts",m_pExperts.size())
                  .raise();
         }
      }

      if(rval == -1)
         cmn::error(cdwHere,"no expert support file type!")
            .with("pattern",text)
            .with("# experts",m_pExperts.size())
            .raise();

      return rval;
   }

   tcat::typeSet<iContentExpert> m_pExperts;
};

tcatExposeTypeAs(contentManager,iContentManager);

class window : public iWindow {
public:
   window(cui::iPort& p, size_t w, size_t h)
   : m_port(p), m_w(w), m_h(h), m_pContent(NULL) {}

   ~window()
   {
      if(m_pContent)
         m_pContent->rmBinding(*this);
   }

   virtual void provide(cui::keyMap& m)
   {
      m_pContent->provide(m);
   }

   virtual void onActivate(bool active)
   {
   }

   virtual void bind(iContent& b)
   {
      if(m_pContent)
         m_pContent->rmBinding(*this);
      m_pContent = &b;
      m_pContent->addBinding(*this);
   }

   virtual void draw()
   {
      m_pContent->getCanvas().drawInto(m_port,0,0,m_w,m_h);
   }

private:
   cui::iPort& m_port;
   size_t m_w;
   size_t m_h;
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
