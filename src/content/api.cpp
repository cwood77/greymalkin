#include "api-i.hpp"

namespace content {
namespace impl {

void contentBase::addBinding(window::iWindow& wnd)
{
   m_pWindows.insert(&wnd);
}

void contentBase::rmBinding(window::iWindow& wnd)
{
   m_pWindows.erase(&wnd);
   if(m_pWindows.size() == 0)
      delete this;
}

window::iCanvas& contentBase::getCanvas()
{
   if(m_pCanvas.get() == NULL)
   {
      m_pCanvas.reset(new virtualPort());
      redraw(*m_pCanvas);
   }
   return *m_pCanvas;
}

void contentBase::onChanged()
{
   m_pCanvas.reset(NULL);

   for(auto *pWnd : m_pWindows)
      pWnd->draw();
}

} // namespace impl
} // namespace content
