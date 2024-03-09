#include "../cmn/service.hpp"
#include "../cui/api.hpp"
#include "../tcatlib/api.hpp"
#include "api-i.hpp"
#include "api.hpp"
#include "cpp.trans.hpp"
#include "syntax-i.hpp"
#include <fstream>

namespace content {
namespace impl {
namespace {

class syntaxColorer : public layeredSyntaxColorer {
public:
   syntaxColorer()
   {
      pNext = &m_q;
      m_q.pNext = &m_adapter;
      m_adapter.layers.push_back(&m_c);
      m_adapter.layers.push_back(&m_cpp);
   }

private:
   quoteSyntaxColorer m_q;
   dictSyntaxColorerAdapter m_adapter;
   cKeywordSyntaxColorer m_c;
   cppKeywordSyntaxColorer m_cpp;
};

class editMenu : public cui::iKeyMenu {
public:
   editMenu(trans::iTransactionTarget& tt, std::vector<std::string>& lines, size_t x, size_t y)
   : m_tt(tt), m_lines(lines), m_x(x), m_y(y) {}

   virtual void onActive(bool active) {}
   virtual void onKey(cui::keystroke k, cui::iKeyDispatcher& d)
   {
      if(k == cui::keystroke::esc())
      {
         d.pop();
      }
      else if(k == cui::keystroke::enter())
      {
         tcat::typePtr<cmn::serviceManager> svcMan;
         auto& tMan = svcMan->demand<trans::iTransactionManager>();
         tMan.add(*new splitLineTransaction(m_tt,m_x,m_y));

         window::message m("moveCursor");
         m.iResult = 1;
         auto& l = svcMan->demand<window::iLayout>();
         l.handleMessage(m);
         m_y++;
         m_x = 0;
      }
      else if(k == cui::keystroke::backspace())
      {
         auto& line = m_lines[m_y];
         std::string before(line.c_str(),m_x-1);
         std::string after(line.c_str()+m_x);
         line = before;
         line += after;

         window::message m("moveCursor");
         m.iResult2 = -1;
         tcat::typePtr<cmn::serviceManager> svcMan;
         auto& l = svcMan->demand<window::iLayout>();
         l.handleMessage(m);
         m_x--;
      }
      // delete
      else // other
      {
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
   }

private:
   trans::iTransactionTarget& m_tt;
   std::vector<std::string>& m_lines;
   size_t m_x;
   size_t m_y;
};

class content : public contentBase, private trans::iTransactionTarget {
public:
   content()
   : m_fileName("src\\shared\\context.hpp")
   {
      load();
   }

   virtual void provide(bool active, cui::keyMap& m) { }

   virtual void handleMessage(bool active, window::message& m)
   {
      if(active && m.key == "save" && !m.handled)
      {
         save();
         m.sResult = "saved";
         m.handled = true;
      }
      else if(m.key == "saveAll")
      {
         save();
         m.iResult++;
      }
      else if(m.key == "beginInput" && !m.handled)
      {
         adjustCoordsToNearestChar(m.iResult,m.iResult2);
         tcat::typePtr<cmn::serviceManager> svcMan;
         auto& d = svcMan->demand<cui::iKeyDispatcher>();
         d.push(*new editMenu(*this,m_lines,m.iResult,m.iResult2));
         m.handled = true;
      }
      else if(m.key == "findTransactionTarget" && !m.handled)
      {
         if(m.sResult == getTargetName())
         {
            m.pPtr = (trans::iTransactionTarget*)this;
            m.handled = true;
         }
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
   virtual std::string getTargetName() const { return HACKPATH(); }

   virtual void run(trans::iTransaction& trans)
   {
      if(auto *t = dynamic_cast<splitLineTransaction*>(&trans))
      {
         auto& line = m_lines[t->y];
         std::string before(line.c_str(),t->x);
         std::string after(line.c_str()+t->x);
         line = before;
         m_lines.insert(m_lines.begin()+1+t->y,after);
      }
      else if(auto *t = dynamic_cast<joinLineTransaction*>(&trans))
      {
         auto& line = m_lines[t->y];
         auto after = m_lines[t->y+1];
         line += after;
         m_lines.erase(std::next(m_lines.begin(),t->y+1));
      }
      else
         return iTransactionTarget::run(trans);
   }

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

   void save()
   {
      std::ofstream file(HACKPATH().c_str());
      bool first = true;
      for(auto& line : m_lines)
      {
         if(first)
            first = false;
         else
            file << std::endl;
         file << line;
      }
   }

   std::string HACKPATH() const
   {
      return std::string("C:\\cygwin64\\home\\chris\\dev\\greymalkin\\") + m_fileName;
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
