#include "../cmn/error.hpp"
#include "../tcatlib/api.hpp"
#include "api-i.hpp"
#include "api.hpp"

namespace content {
namespace impl {

contentBase::~contentBase()
{
   for(auto it=m_pWindows.begin();it!=m_pWindows.end();++it)
      delete it->second;
}

void contentBase::addBinding(window::iWindow& wnd)
{
   m_pWindows[&wnd] = NULL;
}

void contentBase::rmBinding(window::iWindow& wnd)
{
   delete m_pWindows[&wnd];
   m_pWindows.erase(&wnd);
   if(m_pWindows.size() == 0)
      delete this;
}

window::iCanvas& contentBase::redraw(window::iWindow& wnd, std::string& title)
{
   virtualPort*& pCanvas = m_pWindows[&wnd];
   delete pCanvas;
   pCanvas = new virtualPort();
   _redraw(*pCanvas,title);
   return *pCanvas;
}

void contentBase::onChanged()
{
   for(auto it=m_pWindows.begin();it!=m_pWindows.end();++it)
      it->first->draw();
}

} // namespace impl

class contentManager : public iContentManager {
public:
   virtual window::iContent& create(const std::string& text)
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

} // namespace content
