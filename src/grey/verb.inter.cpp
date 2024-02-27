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
#include "../shared/context.hpp"
#include "../shared/criteria.hpp"
#include "../shared/iCommandProvider.hpp"
#endif

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

#if 1
   l.writeLnVerbose("loading config");
   cmn::autoReleasePtr<file::iSstFile> pFigFile(&fMan->bindFile<file::iSstFile>(
      (oNotesPath + "\\.notenav.sst").c_str(),
      file::iFileManager::kReadOnly
   ));
   pFigFile->tie(l);
   shared::config config(pFigFile->dict());
   cmn::autoService<shared::config> _c2(*svcMan,config);

   shared::criteriaManager critMan;
   critMan.write([&](auto& c){ c.radius = config.getHitRadius(); });
   cmn::autoService<shared::criteriaManager> _critManSvc(*svcMan,critMan);

   cui::renderer R;
   cui::keyDispatcher kRootD;
   shared::context ctxt(config,R,kRootD);
   cmn::autoService<shared::context> _c3(*svcMan,ctxt);

   tcat::typeSet<shared::iCommandProvider> commands;
   for(size_t i=0;i<commands.size();i++)
      commands[i]->provide(ctxt);

   critMan.write([&](auto& c){ c.radius = config.getHitRadius(); });

   cui::keySource kSrc;
   kSrc.loop(ctxt.getDispatcher());
#endif
}

} // anonymous namespace
