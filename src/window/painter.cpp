#include "../cmn/service.hpp"
#include "../cui/api.hpp"
#include "../shared/context.hpp"
#include "../shared/iCommandProvider.hpp"
#include "../tcatlib/api.hpp"
#include "api.hpp"

namespace window {
namespace {

class modelPainter : public iModelPainter {
public:
   modelPainter()
   {
      tcat::typePtr<cmn::serviceManager> svcMan;
      m_pCtxt = &svcMan->demand<shared::context>();
   }

   virtual void draw(cui::iPort& p)
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
      p << cui::relLoc(m_pCtxt->cX,m_pCtxt->cY);
      p.writeTruncate(" ");
   }

private:
   shared::context *m_pCtxt;
};

tcatExposeTypeAs(modelPainter,iModelPainter);

class thingee : public shared::iCommandProvider {
public:
   virtual void provide(shared::context& ctxt)
   {
      ctxt.getRootMap().map(cui::keystroke('j'),[&](auto& d){ ctxt.cY++; ctxt.pW->invalidate(); });
      ctxt.getRootMap().map(cui::keystroke('k'),[&](auto& d){ ctxt.cY--; ctxt.pW->invalidate(); });
      ctxt.getRootMap().map(cui::keystroke('l'),[&](auto& d){ ctxt.cX++; ctxt.pW->invalidate(); });
      ctxt.getRootMap().map(cui::keystroke('h'),[&](auto& d){ ctxt.cX++; ctxt.pW->invalidate(); });
   }
};

tcatExposeTypeAs(thingee,shared::iCommandProvider);

} // anonymous namespace
} // namespace window
