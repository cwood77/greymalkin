#include "vPort-i.hpp"

namespace content {
namespace impl {

void virtualPort::writeTruncate(const std::string& text)
{
   for(size_t i=0;i<text.length();i++)
      write(text.c_str()[i]);
}

void virtualPort::drawInto(cui::iPort& p, size_t x, size_t y, size_t w, size_t h)
{
   for(size_t j=0;j<h;j++)
   {
      for(size_t i=0;i<w;i++)
      {
         auto& cInfo = m_yxMap[y+j][x+i];
         p << cInfo.fg;
         p << cInfo.bg;
         p.writeWrap(std::string(1,cInfo.c));
      }
   }
}

void virtualPort::write(char c)
{
   auto& cInfo = m_yxMap[m_y][m_x++];
   cInfo.fg = m_fg;
   cInfo.bg = m_bg;
   cInfo.c = c;
}

} // namespace impl
} // namespace content
