#include "../console/arg.hpp"
#include "../console/log.hpp"
#include "../tcatlib/api.hpp"

namespace {

console::helpVerbs gHelp([](console::iLog& l)
{
   l.writeLnInfo("");
   l.writeLnInfo("an anachronistic console text editor, in 2024");
   l.writeLnInfo("");
   l.writeLnInfo("verbs and switches");
});

} // anonymous namespace
