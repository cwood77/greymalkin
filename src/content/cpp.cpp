#include "../cmn/service.hpp"
#include "../cui/api.hpp"
#include "../tcatlib/api.hpp"
#include "api-i.hpp"
#include "api.hpp"
#include "syntax-i.hpp"
#include <fstream>

namespace content {
namespace impl {
namespace {

class syntaxColorer : public layeredSyntaxColorer {
public:
   syntaxColorer()
   {
      pNext = &m_adapter;
      m_adapter.layers.push_back(&m_c);
      m_adapter.layers.push_back(&m_cpp);
   }

private:
   dictSyntaxColorerAdapter m_adapter;
   cKeywordSyntaxColorer m_c;
   cppKeywordSyntaxColorer m_cpp;
};

class editMenu : public cui::iKeyMenu {
public:
   editMenu(std::vector<std::string>& lines, size_t x, size_t y)
   : m_lines(lines), m_x(x), m_y(y) {}

   virtual void onActive(bool active) {}
   virtual void onKey(cui::keystroke k, cui::iKeyDispatcher& d)
   {
      if(k == cui::keystroke::esc())
      {
         d.pop();
         return;
      }

      auto& line = m_lines[m_y];
      std::string before(line.c_str(),m_x);
      std::string after(line.c_str()+m_x);
      line = before;
      line += std::string(1,k.getChar());
      line += after;

      window::message m("moveCursor");
      m.iResult2 = 1;
      tcat::typePtr<cmn::serviceManager> svcMan;
      auto& l = svcMan->demand<window::iLayout>();
      l.handleMessage(m);
      m_x++;
   }

private:
   std::vector<std::string>& m_lines;
   size_t m_x;
   size_t m_y;
};

class content : public contentBase {
public:
   content()
   : m_fileName("cpp.cpp")
   {
      load();
   }

   virtual void provide(bool active, cui::keyMap& m) { }

   virtual void handleMessage(bool active, window::message& m)
   {
      if(active && m.key == "save" && !m.handled)
      {
         m.sResult = "unimplemented";
         m.handled = true;
      }
      else if(m.key == "saveAll")
         m.iResult++;
      else if(m.key == "beginInput" && !m.handled)
      {
         adjustCoordsToNearestChar(m.iResult,m.iResult2);
         tcat::typePtr<cmn::serviceManager> svcMan;
         auto& d = svcMan->demand<cui::iKeyDispatcher>();
         d.push(*new editMenu(m_lines,m.iResult,m.iResult2));
         m.handled = true;
      }
   }

protected:
   void adjustCoordsToNearestChar(int& x, int& y)
   {
      if((size_t)y >= m_lines.size())
         y = m_lines.size() ? (m_lines.size() - 1) : 0;
      if((size_t)x >= m_lines[y].length())
         x = m_lines[y].length() ? m_lines[y].length() : 0;
   }

   virtual void _redraw(cui::iPort& p, std::string& title)
   {
      title = m_fileName;

      size_t i=0;
      for(auto it=m_lines.begin();it!=m_lines.end();++it,i++)
      {
         p << cui::relLoc(0,i);
         m_syntax.writeLine(*it,p);
      }
   }

private:
   void load()
   {
      std::ifstream file(HACKPATH().c_str());
      while(file.good())
      {
         std::string line;
         std::getline(file,line);
         m_lines.push_back(line);
      }
   }

   std::string HACKPATH() const
   {
      return std::string("C:\\cygwin64\\home\\chris\\dev\\greymalkin\\src\\content\\") + m_fileName;
   }

   syntaxColorer m_syntax;
   std::string m_fileName;
   std::vector<std::string> m_lines;
};

class expert : public iContentExpert {
public:
   virtual bool supports(const std::string& text) const { return text == "<C++>"; }
   virtual window::iContent& create(const std::string&) const { return *new content; }
};

tcatExposeTypeAs(expert,iContentExpert);

} // anonymous namespace
} // namespace impl
} // namespace content
