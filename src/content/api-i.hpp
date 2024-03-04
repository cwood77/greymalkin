#ifndef ___content_api_i___
#define ___content_api_i___

#include "../window/api.hpp"
#include "vPort-i.hpp"
#include <map>

namespace content {
namespace impl {

class contentBase : public window::iContent {
public:
   ~contentBase();

   virtual void addBinding(window::iWindow& wnd);
   virtual void rmBinding(window::iWindow& wnd);
   virtual window::iCanvas& redraw(window::iWindow& wnd, std::string& title);

protected:
   virtual void _redraw(cui::iPort& p, std::string& title) = 0;
   void onChanged();

private:
   std::map<window::iWindow*,virtualPort*> m_pWindows;
};

} // namespace impl
} // namespace content

#endif // ___content_api_i___
