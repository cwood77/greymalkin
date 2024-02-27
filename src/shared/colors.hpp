#ifndef ___shared_colors___
#define ___shared_colors___

#include "../cui/api.hpp"

namespace shared {

class autoHilight {
public:
   autoHilight(cui::iPort& p, bool really = true)
   : m_aF(p,cui::fgcol::kBrightYellow,really)
   , m_aB(p,cui::bgcol::kBlue,really)
   {
   }

private:
   cui::autoColor<cui::fgcol::type> m_aF;
   cui::autoColor<cui::bgcol::type> m_aB;
};

} // namespace shared

#endif // ___shared_colors___
