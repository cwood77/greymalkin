#include "../trans/api.hpp"

namespace content {
namespace impl {

class xyTransaction : public trans::iTransaction {
public:
   virtual std::string getTargetName() const { return targetName; }
   virtual std::string toString() const;
   virtual void fromString(const std::string& s);

   std::string targetName;
   size_t x;
   size_t y;

protected:
   xyTransaction(trans::iTransactionTarget& t, size_t x, size_t y)
   : targetName(t.getTargetName()), x(x), y(y) {}
   xyTransaction() : x(0), y(0) {}

   virtual void cloneInto(xyTransaction& other) const;
};

class splitLineTransaction : public xyTransaction {
public:
   static const char *kId() { return "split"; }

   splitLineTransaction(trans::iTransactionTarget& t, size_t x, size_t y)
   : xyTransaction(t,x,y) {}
   splitLineTransaction() {}

   virtual std::string getId() const { return kId(); }
   virtual trans::iTransaction& createInverse() const;
};

class joinLineTransaction : public xyTransaction {
public:
   static const char *kId() { return "join"; }

   joinLineTransaction(trans::iTransactionTarget& t, size_t x, size_t y)
   : xyTransaction(t,x,y) {}
   joinLineTransaction() {}

   virtual std::string getId() const { return kId(); }
   virtual trans::iTransaction& createInverse() const;
};

} // namespace impl
} // namespace content
