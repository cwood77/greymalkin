#ifndef ___content_syntax_i___
#define ___content_syntax_i___

#include <set>
#include <string>

namespace cui { class iPort; }

namespace content {
namespace impl {

// ------------------------ interfaces

class iSyntaxColorer {
public:
   virtual void writeLine(const std::string& text, cui::iPort& p) = 0;
};

class iCharSyntaxColorer {
public:
   virtual void writeText(const char *pText, cui::iPort& p) = 0;

   iCharSyntaxColorer *pNext;

protected:
   iCharSyntaxColorer() : pNext(NULL) {}
};

class iDictSyntaxColorer {
public:
   virtual bool writeWord(const std::string& word, cui::iPort& p) const = 0;
};

// ------------------------ building blocks

class dictSyntaxColorerAdapter : public iCharSyntaxColorer, private iDictSyntaxColorer {
public:
   virtual void writeText(const char *pText, cui::iPort& p);

   std::list<iDictSyntaxColorer*> layers;

private:
   virtual bool writeWord(const std::string& word, cui::iPort& p) const;
};

class layeredSyntaxColorer : public iSyntaxColorer {
public:
   virtual void writeLine(const std::string& text, cui::iPort& p)
   { pNext->writeText(text.c_str(),p); }

   iCharSyntaxColorer *pNext;

protected:
   layeredSyntaxColorer() : pNext(NULL) {}
};

class dictSyntaxColorerBase : public iDictSyntaxColorer {
public:
   virtual bool writeWord(const std::string& word, cui::iPort& p) const;

protected:
   void addWord(const std::string& word) { m_words.insert(word); }

private:
   std::set<std::string> m_words;
};

// ------------------------ specific impls

class quoteSyntaxColorer : public iCharSyntaxColorer {
public:
   quoteSyntaxColorer() : m_inQuote(false) {}

   virtual void writeText(const char *pText, cui::iPort& p);

private:
   void _writeText(const std::string& text, bool quoted, cui::iPort& p);

   bool m_inQuote;
};

class cCommentSyntaxColorer : public iCharSyntaxColorer {};
class cppCommentSyntaxColorer : public iCharSyntaxColorer {};
class trailingSpaceSyntaxColorer : public iCharSyntaxColorer {};
class trailingForwardSlashSyntaxColorer : public iCharSyntaxColorer {};

class cKeywordSyntaxColorer : public dictSyntaxColorerBase {
public:
   cKeywordSyntaxColorer();
};

class cppKeywordSyntaxColorer : public dictSyntaxColorerBase {
public:
   cppKeywordSyntaxColorer();
};

} // namespace impl
} // namespace content

#endif // ___content_syntax_i___
