#ifndef ___shared_context___
#define ___shared_context___

#include "../cmn/error.hpp"
#include "../cui/api.hpp"
#include "config.hpp"
#include <memory>
#include <vector>

namespace shared {

class context {
public:
   context(config& c, cui::renderer& r, cui::iKeyDispatcher& kd);
   ~context() { clearHitPanes(); }

   cui::iPort& getTopPane() { return *m_pTPane; }
   cui::iPort& getMiddlePane() { return *m_pMPane; }
   cui::iPort& getBottomPane() { return *m_pBPane; }

   cui::iPort& getHitPane(size_t i);
   size_t getNumHitPanes() const { return m_hitPanes.size(); }
   void resizeHitPanes(size_t radius);

   cui::iKeyDispatcher& getDispatcher() { return m_dispatcher; }
   cui::keyMap& getRootMap();

private:
   void clearHitPanes();

   const size_t m_bottomWidth;
   const size_t m_bottomOffset;
   const size_t m_bottomHeight;
   cui::screenBuffer m_screen;
   std::unique_ptr<cui::iPort> m_pTPane;
   std::unique_ptr<cui::iPort> m_pMPane;
   std::unique_ptr<cui::iPort> m_pBPane;
   std::vector<cui::iPort*> m_hitPanes;
   cui::iKeyDispatcher& m_dispatcher;
   cui::keyMap *m_pRootMap;
};

#include "context.ipp"

} // namespace shared

#endif // ___shared_context___
