#ifndef ___trans_api___
#define ___trans_api___

#include "../cmn/error.hpp"
#include <memory>
#include <string>

namespace trans {

// supplied by clients externally
class iTransaction {
public:
   virtual ~iTransaction() {}
   virtual std::string getTargetName() const = 0;
   virtual std::string getId() const = 0;
   virtual std::string toString() const = 0;
   virtual void fromString(const std::string& s) = 0;
   virtual iTransaction& createInverse() const = 0;
};

// something that is acted upon by a transaction
// typically a window
class iTransactionTarget {
public:
   virtual std::string getTargetName() const = 0;
   virtual void run(iTransaction& trans)
   {
      cmn::error(cdwHere,"target doesn't understand transaction")
         .with("transaction id",trans.getId())
         .with("target name",getTargetName())
         .raise();
   }
};

// look-up provided by grey
class iTransactionTargetProvider {
public:
   virtual iTransactionTarget& getTarget(const std::string& name) = 0;
};

// top-level expert
class iTransactionManager {
public:
   virtual ~iTransactionManager() {}
   virtual void initialize(iTransactionTargetProvider& prov) = 0;
   virtual size_t load(const std::string& path) = 0;
   virtual void add(iTransaction& t) = 0;
   virtual bool undo() = 0;
};

// factory plugged-in by clients
class iTransactionInfo {
public:
   virtual ~iTransactionInfo() {}
   virtual std::string getId() const = 0;
   virtual iTransaction& create() const = 0;
};

class iTransactionCatalog {
public:
   virtual iTransaction& create(const std::string& id, const std::string& payload) const = 0;
};

template<class T>
class transactionInfoBase : public trans::iTransactionInfo {
public:
   virtual std::string getId() const { return m_pId; }
   virtual trans::iTransaction& create() const { return *new T(); }

protected:
   explicit transactionInfoBase(const char *pId) : m_pId(pId) {}

private:
   const char *m_pId;
};

#define cdwPublishTrans(__prefix__) \
class __prefix__##TransactionInfo \
   : public trans::transactionInfoBase<__prefix__##Transaction> { \
public: \
   __prefix__##TransactionInfo() \
   : transactionInfoBase(__prefix__##Transaction::kId()) {} \
}; \
 \
tcatExposeTypeAs(__prefix__##TransactionInfo,trans::iTransactionInfo);

#define cdw_ImplTransInv(__type1__,__type2__) \
trans::iTransaction& __type1__::createInverse() const \
{ \
   std::unique_ptr<__type2__> pOther(new __type2__()); \
   cloneInto(*pOther); \
   return *pOther.release(); \
}

#define cdwImplTransInv(__type1__,__type2__) \
cdw_ImplTransInv(__type1__##Transaction,__type2__##Transaction) \
cdw_ImplTransInv(__type2__##Transaction,__type1__##Transaction)

} // namespace trans

#endif // ___trans_api___
