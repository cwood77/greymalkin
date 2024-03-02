#include "../cmn/error.hpp"
#include "../cui/api.hpp"
#include "../window/api.hpp"
#include <map>

namespace content {
namespace impl {

struct cellInfo {
   cellInfo()
   : fg(cui::fgcol::kWhite), bg(cui::bgcol::kBlack), c(' ') {}

   cui::fgcol::type fg;
   cui::bgcol::type bg;
   char c;
};

class virtualPort : public cui::iPort, public window::iCanvas {
public:
   virtualPort() : m_x(0), m_y(0), m_fg(cui::fgcol::kWhite), m_bg(cui::bgcol::kBlack) {}

   virtual void fill(cui::fgcol::type f, cui::bgcol::type b, char c)
   { cmn::unimplemented(cdwHere).raise(); }

   virtual void writeWrap(const std::string& text) { cmn::unimplemented(cdwHere).raise(); }
   virtual void writeTruncate(const std::string& text);

   virtual void insert(const cui::relLoc& l) { m_x=l.x; m_y=l.y; }
   virtual void insert(cui::fgcol::type t) { m_fg = t; }
   virtual void insert(cui::bgcol::type t) { m_bg = t; }

   virtual cui::fgcol::type swapColor(cui::fgcol::type t)
   { auto rval = m_fg; insert(t); return rval; }
   virtual cui::bgcol::type swapColor(cui::bgcol::type t)
   { auto rval = m_bg; insert(t); return rval; }

   virtual void drawInto(cui::iPort& p, size_t x, size_t y, size_t w, size_t h);

private:
   void write(char c);

   size_t m_x;
   size_t m_y;
   cui::fgcol::type m_fg;
   cui::bgcol::type m_bg;

   std::map<size_t,std::map<size_t,cellInfo> > m_yxMap;
};

} // namespace impl
} // namespace content
