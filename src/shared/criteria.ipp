inline void criteriaManager::write(std::function<void(criteria&)> f)
{
   f(m_c);

   std::set<iCriteriaObserver*> copy = m_obs;
   for(auto *pObs : copy)
      pObs->onCriteriaChanged(m_c);
}
