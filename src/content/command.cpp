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
      m_log.push_back("booted");
   }

   virtual void provide(cui::keyMap& m)
   {
      m.map(cui::keystroke('f'),[&](auto&)
      {
         window::message m("save");
         sendMessage(m);
         logSinglecastMessageResult(m);
      });
      m.map(cui::keystroke('F'),[&](auto&)
      {
         window::message m("saveAll",/*broadcast*/true);
         sendMessage(m);
         logMulticastMessageResult(m);
      });
   }

   virtual void handleMessage(window::message& m)
   {
      if(m.key == "save" && !m.handled)
      {
         m.sResult = "ok";
         m.handled = true;
      }
      else if(m.key == "saveAll")
      {
         m.iResult++;
         m.handled = true;
      }
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
   void sendMessage(window::message& m)
   {
      tcat::typePtr<cmn::serviceManager> svcMan;
      auto& l = svcMan->demand<window::iLayout>();
      l.handleMessage(m);
   }

   void logSinglecastMessageResult(window::message& m)
   {
      std::stringstream stream;
      stream << m.key << ": " << m.sResult;
      m_log.push_back(stream.str());
      onChanged();
   }

   void logMulticastMessageResult(window::message& m)
   {
      std::stringstream stream;
      stream << m.key << ": " << m.iResult << " done";
      m_log.push_back(stream.str());
      onChanged();
   }

   std::list<std::string> m_log;
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
