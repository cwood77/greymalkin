#include "../cui/api.hpp"
#include "../tcatlib/api.hpp"
#include "api-i.hpp"

namespace content {
namespace impl {
namespace {

class content : public contentBase {
public:
   content() : m_x(0), m_y(0) {}

   virtual void provide(cui::keyMap& m)
   {
      m.map(cui::keystroke('j'),[&](auto& d){ m_y++; onChanged(); });
      m.map(cui::keystroke('k'),[&](auto& d){ m_y--; onChanged(); });
      m.map(cui::keystroke('l'),[&](auto& d){ m_x++; onChanged(); });
      m.map(cui::keystroke('h'),[&](auto& d){ m_x++; onChanged(); });
   }

protected:
   virtual void redraw(cui::iPort& p)
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

      cui::autoColor<cui::bgcol::type> _c(p,cui::bgcol::kBrightBlue);
      p << cui::relLoc(m_x,m_y);
      p.writeTruncate(" ");
   }

private:
   size_t m_x;
   size_t m_y;
};

class expert : public window::iContentExpert {
public:
   virtual bool supports(const std::string& text) const { return text == "<cmd>"; }
   virtual window::iContent& create(const std::string&) const { return *new content; }
};

tcatExposeTypeAs(expert,window::iContentExpert);

} // anonymous namespace
} // namespace impl
} // namespace content
