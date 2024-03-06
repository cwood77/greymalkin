#include "../cui/api.hpp"
#include "syntax-i.hpp"

namespace content {
namespace impl {

void dictSyntaxColorerAdapter::writeText(const char *pText, cui::iPort& p)
{
   const char *pStart = pText;
   const char *pThumb = pStart;
   while(true)
   {
      for(;*pThumb!=' '&&*pThumb!='\0';++pThumb);
      std::string word(pStart,pThumb-pStart);
      writeWord(word,p);

      bool EoL = (*pThumb == '\0');
      if(EoL)
         break;

      writeWord(" ",p);
      pThumb++;
      pStart = pThumb;
   }
}

bool dictSyntaxColorerAdapter::writeWord(const std::string& word, cui::iPort& p) const
{
   for(auto *pLayer : layers)
      if(pLayer->writeWord(word,p))
         return true;

   p.writeTruncate(word);
   return true;
}

bool dictSyntaxColorerBase::writeWord(const std::string& word, cui::iPort& p) const
{
   auto it = m_words.find(word);
   bool found = (it != m_words.end());
   if(found)
   {
      cui::autoColor<cui::fgcol::type> _c(p,cui::fgcol::kYellow);
      p.writeTruncate(word);
   }
   return found;
}

cKeywordSyntaxColorer::cKeywordSyntaxColorer()
{
   addWord("#include");
}

cppKeywordSyntaxColorer::cppKeywordSyntaxColorer()
{
   addWord("bool");
   addWord("class");
   addWord("namespace");
   addWord("private");
   addWord("private:");
   addWord("public");
   addWord("public:");
   addWord("virtual");
}

} // namespace impl
} // namespace content
