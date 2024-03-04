#include "../cmn/service.hpp"
#include "../cui/api.hpp"
#include "../tcatlib/api.hpp"
#include "api-i.hpp"
#include "api.hpp"

namespace content {
namespace impl {
namespace {

class content : public contentBase {
public:
   content()
   {
   }

   virtual void provide(bool active, cui::keyMap& m)
   {
   }

   virtual void handleMessage(bool active, window::message& m)
   {
      if(active && m.key == "save" && !m.handled)
      {
         m.sResult = "ok";
         m.handled = true;
      }
      else if(m.key == "saveAll")
         m.iResult++;
   }

protected:
   virtual void _redraw(cui::iPort& p, std::string& title)
   {
      title = "greymalkin";

      size_t i=0;
      for(auto it=m_log.begin();it!=m_log.end();++it,i++)
      {
         p << cui::relLoc(0,i);
         p.writeTruncate(*it);
      }
   }

private:
   std::list<std::string> m_log;
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
