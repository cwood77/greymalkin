inline context::context(config& c, cui::renderer& r, cui::iKeyDispatcher& kd)
: m_bottomWidth(c.getScreenWidth())
, m_bottomOffset(c.getStatusHeight()+1)
, m_bottomHeight(c.getScreenHeight()-1-c.getStatusHeight())
, m_screen(c.getScreenWidth(),c.getScreenHeight())
, m_pTPane(&m_screen.createPort(
   0,0,
   c.getScreenWidth(),c.getStatusHeight()))
, m_pMPane(&m_screen.createPort(
   0,c.getStatusHeight(),
   c.getScreenWidth(),1))
, m_pBPane(&m_screen.createPort(
   0,c.getStatusHeight()+1,
   c.getScreenWidth(),m_bottomHeight))
, m_dispatcher(kd)
, m_pRootMap(new cui::keyMap())
{
   r.activate(&m_screen);

   m_pTPane->fill(' ');
   m_pMPane->fill(cui::fgcol::kBrightBlue,cui::bgcol::kBlack,'=');

   m_dispatcher.push(*m_pRootMap);
}

inline cui::iPort& context::getHitPane(size_t i)
{
   if(i >= m_hitPanes.size())
      throw cmn::error(cdwHere,"hit pane index out of bounds")
         .with("i",i)
         .with("size",m_hitPanes.size())
         .raise();
   return *m_hitPanes[i];
}

inline void context::resizeHitPanes(size_t radius)
{
   const size_t paneSize = radius*2+1;
   const size_t numPanes = m_bottomHeight / paneSize;

   clearHitPanes();
   m_pBPane->fill(' ');

   for(size_t i=0;i<numPanes;i++)
   {
      m_hitPanes.push_back(&m_screen.createPort(
         0,i*paneSize+m_bottomOffset,
         m_bottomWidth,paneSize
      ));
   }

   if(m_hitPanes.size() > 1)
      m_hitPanes[1]->fill(cui::fgcol::kWhite,cui::bgcol::kBrightBlack,' ');
   if(m_hitPanes.size() > 3)
      m_hitPanes[3]->fill(cui::fgcol::kWhite,cui::bgcol::kBrightBlack,' ');
}

inline cui::keyMap& context::getRootMap()
{
   auto *pCurMap = m_dispatcher.head();
   if(!pCurMap)
      cmn::error(cdwHere,"accessing root map when there is none")
         .raise();
   return *m_pRootMap;
}

inline void context::clearHitPanes()
{
   for(auto *pPort : m_hitPanes)
      delete pPort;
   m_hitPanes.clear();
}
