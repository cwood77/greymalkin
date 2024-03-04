#include "../cmn/service.hpp"
#include "../cui/api.hpp"
#include "../tcatlib/api.hpp"
#include "api-i.hpp"
#include "api.hpp"
#include <fstream>

namespace content {
namespace impl {
namespace {

class content : public contentBase {
public:
   content()
   : m_fileName("cpp.cpp")
   {
      load();
   }

   virtual void provide(bool active, cui::keyMap& m)
   {
   }

   virtual void handleMessage(bool active, window::message& m)
   {
      if(active && m.key == "save" && !m.handled)
      {
         m.sResult = "unimplemented";
         m.handled = true;
      }
      else if(m.key == "saveAll")
         m.iResult++;
   }

protected:
   virtual void _redraw(cui::iPort& p, std::string& title)
   {
      title = m_fileName;

      size_t i=0;
      for(auto it=m_lines.begin();it!=m_lines.end();++it,i++)
      {
         p << cui::relLoc(0,i);
         p.writeTruncate(*it);
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

   std::string m_fileName;
   std::list<std::string> m_lines;
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
