#include "../tcatlib/api.hpp"
#include "cpp.trans.hpp"
#include <sstream>

namespace content {
namespace impl {

std::string xyTransaction::toString() const
{
   std::stringstream stream;
   stream << x << ":" << y << ":" << targetName;
   return stream.str();
}

void xyTransaction::fromString(const std::string& s)
{
   int a,b,n;
   ::sscanf(s.c_str(),"%d:%d:%n",&a,&b,&n);

   targetName = s.c_str() + n;
   x = a;
   y = b;
}

void xyTransaction::cloneInto(xyTransaction& other) const
{
   other.targetName = targetName;
   other.x = x;
   other.y = y;
}

cdwImplTransInv(splitLine,joinLine)

namespace {

cdwPublishTrans(splitLine);
cdwPublishTrans(joinLine);

} // anonymous namespace
} // namespace impl
} // namespace content
