#ifndef ___content_api___
#define ___content_api___

#include <string>

namespace window { class iContent; }

namespace content {

class iContentExpert {
public:
   virtual ~iContentExpert() {}
   virtual bool supports(const std::string& text) const = 0;
   virtual window::iContent& create(const std::string& text) const = 0;
};

class iContentManager {
public:
   virtual ~iContentManager() {}
   virtual window::iContent& create(const std::string& text) = 0;
};

} // namespace content

#endif // ___content_api___
