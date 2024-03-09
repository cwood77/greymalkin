#include "../tcatlib/api.hpp"
#include "api.hpp"

namespace trans {
namespace {

class transactionManager : public iTransactionManager {
public:
   transactionManager() : m_pProv(NULL) {}

   ~transactionManager()
   {
      for(auto *pTrans : m_trans)
         delete pTrans;
   }

   virtual void initialize(iTransactionTargetProvider& prov) { m_pProv = &prov; }

   virtual size_t load(const std::string& path) { throw 3.14; }

   virtual void add(iTransaction& t)
   {
      auto& tgt = m_pProv->getTarget(t.getTargetName());
      tgt.run(t);
      m_trans.push_back(&t);
   }

   virtual bool undo()
   {
      if(m_trans.size() == 0)
         return false;
      auto& t = (*(--m_trans.end()))->createInverse();
      add(t);
      return true;
   }

private:
   iTransactionTargetProvider *m_pProv;
   std::list<iTransaction*> m_trans;
};

tcatExposeTypeAs(transactionManager,iTransactionManager)

} // anonymous namespace
} // namespace trans
