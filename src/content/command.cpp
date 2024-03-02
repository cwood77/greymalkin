#include "../cui/api.hpp"
#include "../tcatlib/api.hpp"
#include "api-i.hpp"
#include "api.hpp"

namespace content {
namespace impl {
namespace {

class content : public contentBase {
public:
   virtual void provide(cui::keyMap& m)
   {
      m.map(cui::keystroke::esc(),[&](auto& d){ d.pop(); });
   }

protected:
   virtual void _redraw(cui::iPort& p)
   {
      const char *pText1 = "the ";
      const char *pText2 = "quick";
      const char *pText3 = " brown fox jumped over the lazy dog";

      for(size_t i=0;i<100;i++)
      {
         p << cui::relLoc(i,i);
         {
            cui::autoColor<cui::bgcol::type> _c(p,cui::bgcol::kBrightBlack);
            p.writeTruncate(std::string(i,' '));
         }
         p.writeTruncate(pText1);
         {
            cui::autoColor<cui::fgcol::type> _c(p,cui::fgcol::kBrightYellow);
            p.writeTruncate(pText2);
         }
         p.writeTruncate(pText3);
      }
   }
};

class expert : public iContentExpert {
public:
   virtual bool supports(const std::string& text) const { return text == "<cmd>"; }
   virtual window::iContent& create(const std::string&) const { return *new content; }
};

tcatExposeTypeAs(expert,iContentExpert);

} // anonymous namespace
} // namespace impl
} // namespace content
