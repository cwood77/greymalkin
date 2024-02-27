#ifndef ___shared_config___
#define ___shared_config___

#include "../file/api.hpp"

namespace shared {

class config {
public:
   explicit config(sst::dict& d) : m_d(d) {}

   // screen size
   size_t getScreenWidth() const  { return m_d.getOpt<sst::mint>("screen-width",155); }
   size_t getScreenHeight() const { return m_d.getOpt<sst::mint>("screen-height",38); }

   // status size
   // search>
   // radius:-4+ options: [ case:insens word:partial ]
   // reload exit
   size_t getStatusHeight() const { return m_d.getOpt<sst::mint>("status-height",3); }

   // actor names
   // meeting formats
   // file title formats

   size_t getHitRadius() const { return m_d.getOpt<sst::mint>("hit-radius",2); }

private:
   sst::dict& m_d;
};

} // namespace shared

#endif // ___shared_config___
