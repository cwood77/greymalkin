#include "../cmn/service.hpp"
#include "../cui/api.hpp"
#include "../tcatlib/api.hpp"
#include "../trans/api.hpp"
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

   virtual void provide(bool active, cui::keyMap& m)
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
      m.map(cui::keystroke('u'),[&](auto&)
      {
         tcat::typePtr<cmn::serviceManager> svcMan;
         auto& tMan = svcMan->demand<trans::iTransactionManager>();
         bool ok = tMan.undo();
         m_log.push_back(ok ? "last change undone" : "nothing to undo");
         svcMan->demand<window::iLayout>().draw();
      });
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
