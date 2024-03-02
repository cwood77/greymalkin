#ifndef ___content_api_i___
#define ___content_api_i___

#include "../window/api.hpp"
#include "vPort-i.hpp"
#include <memory>
#include <set>

namespace content {
namespace impl {

class contentBase : public window::iContent {
public:
   virtual void addBinding(window::iWindow& wnd);
   virtual void rmBinding(window::iWindow& wnd);
   virtual window::iCanvas& getCanvas();

protected:
   virtual void redraw(cui::iPort& p) = 0;
   void onChanged();

private:
   std::unique_ptr<virtualPort> m_pCanvas;
   std::set<window::iWindow*> m_pWindows;
};

} // namespace impl
} // namespace content

#endif // ___content_api_i___
