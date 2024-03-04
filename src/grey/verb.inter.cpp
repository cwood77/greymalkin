#include "../cmn/autoPtr.hpp"
#include "../cmn/service.hpp"
#include "../console/arg.hpp"
#include "../console/log.hpp"
#include "../file/api.hpp"
#include "../file/manager.hpp"
#include "../tcatlib/api.hpp"
#include <memory>

#if 1
#include "../cui/api.hpp"
#include "../shared/config.hpp"
//#include "../shared/context.hpp"
//#include "../shared/iCommandProvider.hpp"
#endif

#include "../window/api.hpp"
#include "../content/api.hpp"

namespace {

class command : public console::iCommand {
public:
   command() : oConfigPath("."), oNotesPath(".") {}

   std::string oConfigPath;
   std::string oNotesPath;

protected:
   virtual void run(console::iLog& l);
};

class myVerb : public console::globalVerb {
public:
   virtual void dumpDocs(console::iLog& l)
   {
      l.writeLnInfo("--inter [<path-to-config>] [<path-to-notes>]");
      l.writeLnInfo("   Interactively navigate notes");
   }

protected:
   virtual console::verbBase *inflate()
   {
      std::unique_ptr<console::verbBase> v(
         new console::verb<command>("--inter"));

      v->addParameter(
         console::stringParameter::optional(offsetof(command,oConfigPath)));
      v->addParameter(
         console::stringParameter::optional(offsetof(command,oNotesPath)));

      return v.release();
   }
} gVerb;

void command::run(console::iLog& l)
{
   tcat::typePtr<file::iFileManager> fMan;
   l.writeLnDebug("loading config settings (optional)");
   cmn::autoReleasePtr<file::iSstFile> pFile(&fMan->bindFile<file::iSstFile>(
      file::iFileManager::kExeAdjacent,
      "config.sst",
      file::iFileManager::kReadOnly
   ));
   pFile->tie(l);
   l.configure(pFile->dict());

   l.writeLnVerbose("compiling services");
   tcat::typePtr<cmn::serviceManager> svcMan;
   cmn::autoService<console::iLog> _l(*svcMan,l);
   cmn::autoService<sst::dict> _c(*svcMan,pFile->dict(),"_config");
   cmn::autoService<std::string> _pI(*svcMan,oConfigPath,"oConfigPath");
   cmn::autoService<std::string> _pO(*svcMan,oNotesPath,"oNotesPath");

   l.writeLnVerbose("inputPath is %s",oConfigPath.c_str());
   l.writeLnVerbose("outputPath is %s",oNotesPath.c_str());

   l.writeLnVerbose("loading config");
   cmn::autoReleasePtr<file::iSstFile> pFigFile(&fMan->bindFile<file::iSstFile>(
      (oNotesPath + "\\.grey.sst").c_str(),
      file::iFileManager::kReadOnly
   ));
   pFigFile->tie(l);
   shared::config config(pFigFile->dict());
   cmn::autoService<shared::config> _c2(*svcMan,config);

   l.writeLnVerbose("create screen");
   cui::screenBuffer screen(config.getScreenWidth(),config.getScreenHeight());
   cui::renderer R;
   R.activate(&screen);

   // create initial window
   tcat::typePtr<window::iManager> pWMan;
   std::unique_ptr<window::iLayout> pLayout(
      &pWMan->create(screen));
   cmn::autoService<window::iLayout> _pL(*svcMan,*pLayout);

   // bind command window
   tcat::typePtr<content::iContentManager> pConMan;
   cmn::autoService<content::iContentManager> _cm(*svcMan,*pConMan);
   pLayout->getIth(0).bind(pConMan->create("<cmd>"));
   pLayout->getIth(1).bind(pConMan->create("<C++>"));
   pLayout->draw();

   // collect commands
   cui::keyDispatcher kRootD;
   //shared::context ctxt(config,R,kRootD);
   //cmn::autoService<shared::context> _c3(*svcMan,ctxt);
   //pLayout->provide(ctxt.getRootMap());
   pLayout->provide(kRootD);

   // run command loop
   cui::keySource kSrc;
   kSrc.loop(kRootD);
}

// TODO... maybe?
   // initialize history service from journalfile
   //
   // paint global frame and set global keybindings
   //
   // send "path" to file identifying expert
   //   expert paints screenBuffer and sets keybindings

   //tcat::typeSet<shared::iCommandProvider> commands;
   //for(size_t i=0;i<commands.size();i++)
   //   commands[i]->provide(ctxt);

} // anonymous namespace
