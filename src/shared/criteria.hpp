#ifndef ___shared_criteria___
#define ___shared_criteria___

#include <functional>
#include <set>
#include <string>

namespace shared {

class criteria {
public:
   criteria() : radius(0) {}

   std::string pattern;
   //bool caseInsens;
   //bool wholeWord;
   size_t radius;
};

class iCriteriaObserver {
public:
   virtual void onCriteriaChanged(const criteria& c) = 0;
};

class criteriaManager {
public:
   const criteria& read() const { return m_c; }
   void write(std::function<void(criteria&)> f);

   void install(iCriteriaObserver& o) { m_obs.insert(&o); }
   void uninstall(iCriteriaObserver& o) { m_obs.erase(&o); }

private:
   criteria m_c;
   std::set<iCriteriaObserver*> m_obs;
};

#include "criteria.ipp"

} // namespace shared

#endif // ___shared_criteria___
